---
baslik: "Genel Durum Mesajı — 11 Bayt"
kategori: "03-software"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Genel Durum Mesajı — 11 Bayt

**Son Güncelleme:** 2026-09-18

## Paketin son hali

Genel durumu **11 baytlık `VertexStatusPayload`** ile gönderiyoruz. Pil ve ortam sıcaklığını paketin sonuna ekledik. Önceki 7 baytlık biçim [ADR-0014](../07-decisions/ADR-0014-status-temperatures.md) ile değişti. ClusterPilot alıcısını daha uyarlamadık. TSphere tarafındaki MQTT konusu ve içerik biçimi de henüz belli değil.

| Özellik | Değer |
|---|---|
| Yön | Vertex → ClusterPilot |
| Taşıma | CAN/ISO-TP, 2 veri çerçevesi + 1 Flow Control |
| Uygulama verisi | **11 bayt** |
| Tür | `0x03` (`PAYLOAD_TYPE_GENERAL_STATUS`) |
| Çok baytlı alan sırası | Little-endian |
| Hedef gönderim aralığı | 3000 ms |

## Alanlar ve bayt yerleşimi

Baytları sıfırdan sayıyoruz. Tabloya ISO-TP başlığını dahil etmiyoruz.

| Bayt | Alan | Tür / boyut | Birim veya anlam |
|---|---|---|---|
| 0 | `payload_type` | `uint8_t` / 1 bayt | `0x03` |
| 1–2 | `battery_voltage_mv` | `uint16_t` / 2 bayt | Pil gerilimi, mV |
| 3 | `scenario_player_state` | `uint8_t` / 1 bayt | Oynatıcı durumu |
| 4 | `charger_mode` | `uint8_t` / 1 bayt | Idle / şarj / deşarj |
| 5–6 | `battery_current_ma` | `int16_t` / 2 bayt | Pil akımı, mA; pozitif şarj, negatif deşarj |
| 7–8 | `battery_temperature_dc` | `int16_t` / 2 bayt | Pil sıcaklığı, °C × 10 |
| 9–10 | `ambient_temperature_dc` | `int16_t` / 2 bayt | Ortam sıcaklığı, °C × 10 |

Toplam **1 + 2 + 1 + 1 + 2 + 2 + 2 = 11 bayt**. Boyut ve alan yerleri derlemede kontrol ediliyor. Oynatıcıyla charger modunu ayrı baytlarda tutuyoruz. Bu pakette zaman, sıra numarası, SOC/SOH, test kimliği veya hata ayrıntısı yok.

## Durum kodları

| Oynatıcı değeri | Anlam |
|---|---|
| 0 | Idle |
| 1 | Running |
| 2 | Paused |
| 3 | Stopped |
| 4 | Error |

| Charger modu | Sembol | Üretildiği context durumu |
|---|---|---|
| 0 | `TL_CHARGER_MODE_IDLE` | Reverse ve enabled kapalı |
| 1 | `TL_CHARGER_MODE_CHARGING` | Reverse kapalı, enabled açık |
| 2 | `TL_CHARGER_MODE_DISCHARGING` | Reverse açık; enabled değerinden bağımsız |

Deşarj sırasında senaryo kodu `enabled=false`, `is_reverse_mode=true` kullanıyor. Bu yüzden önce reverse’e bakıyoruz. İki bayrak birden açık olsa da pakette deşarj görünüyor.

## Veri kaynakları ve sınırlar

- Oynatıcı durumu doğrudan `g_scenario_player.playerState` üzerinden alınır.
- Charger modu context’teki kontrol bayraklarından geliyor. Donanımdan geri okuyup doğruladığımız bir durum değil.
- Gerilim `g_tl_context.dut.voltage_mV` alanından geliyor. Şu an sıfırla başlıyor ve güncellemesi yorum satırında; paket alanı hazır ama gerçek gerilim okuması bağlı değil.
- Akım `g_tl_context.dut.current_mA` üzerinden alınır; bu alan fuel gauge ölçümünden güncellenir. Dahili context alanı `int32_t` kalmıştır.
- Akım −32768…+32767 mA dışındaysa **genel durum paketi o tur gönderilmez**, hata loglanır ve sonraki deneme 3 saniye sonra yapılır. Değer kırpılmaz veya taşarak farklı bir ölçüm gibi gönderilmez. Bu koruma genel durum paketine aittir; diğer telemetri davranışları değiştirilmedi.

Pil sıcaklığı `g_tl_context.dut.temperature_dC`, ortam sıcaklığı `g_tl_context.ambient_temperature_dC` alanından geliyor. İkisi de °C × 10; −32768 ölçüm yok demek. Sensör okumaları henüz bağlı değil, bu yüzden başlangıçta geçersiz değer taşınıyor.

## Gönderim ve ayrıştırma

Üç saniye dolduğunda ISO-TP meşgulse bekliyoruz. Bağlantı boşalınca önce genel durumu göndermeyi deniyoruz. Başlatma hatası olursa loga yazıp sonraki denemeyi üç saniye sonra yapıyoruz. Yani bu süre gönderim hedefi; her paketin teslim edildiğini söylemiyor.

Alıcı, ISO-TP başlığını ayırdıktan sonra uygulama tür alanını okumalı ve `0x03` için 11 bayt beklemelidir. Farklı mesaj türlerinin aynı uzunlukta olması mümkündür; uzunluk tür seçmek için kullanılmaz.

Genel durum artık tek CAN çerçevesine sığmıyor. ISO-TP ilk çerçevede 6, devam çerçevesinde 5 bayt taşıyor. Alıcının bir Flow Control çerçevesiyle toplam üç CAN çerçevesi oluşuyor. Devam çerçevesinde iki dolgu baytı var.

Genel durum yine testin çalışıp çalışmadığına bakmadan 3 saniyede bir deneniyor. Ayrı sıcaklık mesajı kaldırıldı; test sırasında da ayrıca gönderilmiyor. Böylece test durmuşken sıcaklıkları genel durumdan görebiliyoruz.

## Örnek paket

Alanların nasıl yerleştiğini görmek için bir örnek. Bu veri cihazdan alınmadı; gerilim okumasının çalıştığını göstermiyor.

```text
Uygulama verisi: 03 74 0E 01 02 18 FC FD 00 83 FF
İlk çerçeve:    10 0B 03 74 0E 01 02 18
Flow Control:   30 00 00 00 00 00 00 00  (alıcıdan örnek)
Devam çerçeve:  21 FC FD 00 83 FF 00 00
```

Tür `0x03`, gerilim 3700 mV, oynatıcı running (`1`), charger deşarj (`2`), akım −1000 mA (`0xFC18`), pil 25,3 °C (`0x00FD`), ortam −12,5 °C (`0xFF83`).

## Kontroller ve kaynak kod

Debug derlemesi geçti. Gerçek dispatcher ve ISO-TP kodunu bilgisayarda çalıştırıp 11 baytlık alan yerleşimini, iki veri çerçevesini, dolguyu, pozitif/negatif sıcaklıkları ve −32768 işaretini kontrol ettik. Test durmuşken genel durumun gönderildiğini de denedik. Telemetri kontrolleri ve ayrı sıcaklık gönderiminin kaldırıldığını doğrulayan kontrol de geçti. Kart testi yapılmadı.

Kaynaklar `tronloop-vertex-firmware/` deposuna göredir:

- `Core/Inc/tl_dispatcher.h`: şema, mod kodları ve boyut/ofset kontrolleri.
- `Core/Src/tl_dispatcher.c`: alan kaynakları, mod eşlemesi, aralık kontrolü ve gönderim.
- `Core/Inc/tl_context.h`, `Core/Src/tl_context.c`: durum ve ölçüm alanları.
- `Core/Src/tl_scenario_player.c`: şarj/deşarj bayraklarının kullanımı.
- `Middlewares/isotp-c/isotp.c`, `isotp_config.h`: taşıma ve dolgu davranışı.

[Mesajlaşma protokolü](communication-notes.md) · [Vertex mesaj envanteri](vertex-message-inventory.md)
