---
baslik: "Genel Durum Mesajı — 7 Bayt"
kategori: "03-software"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Genel Durum Mesajı — 7 Bayt

**Son Güncelleme:** 2026-09-18

## Paketin son hali

Genel durumu **7 baytlık `VertexStatusPayload`** ile gönderiyoruz. Önceki 10 baytlık yapı değişti; firmware 2026-09-18’de [bu karara](../07-decisions/ADR-0010-message-type-and-operation-mode.md) göre güncellendi. ClusterPilot alıcısını daha uyarlamadık. TSphere tarafındaki MQTT konusu ve içerik biçimi de henüz belli değil.

| Özellik | Değer |
|---|---|
| Yön | Vertex → ClusterPilot |
| Taşıma | CAN/ISO-TP, tek çerçeve |
| Uygulama verisi | **7 bayt** |
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

Toplam **1 + 2 + 1 + 1 + 2 = 7 bayt**. Boyut ve alan yerleri derlemede kontrol ediliyor. Oynatıcıyla charger modunu ayrı baytlarda tutuyoruz. Bu pakette zaman, sıra numarası, sıcaklık, SOC/SOH, test kimliği veya hata ayrıntısı yok.

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

## Gönderim ve ayrıştırma

Üç saniye dolduğunda ISO-TP meşgulse bekliyoruz. Bağlantı boşalınca önce genel durumu göndermeyi deniyoruz. Başlatma hatası olursa loga yazıp sonraki denemeyi üç saniye sonra yapıyoruz. Yani bu süre gönderim hedefi; her paketin teslim edildiğini söylemiyor.

Alıcı, ISO-TP başlığını ayırdıktan sonra uygulama tür alanını okumalı ve `0x03` için 7 bayt beklemelidir. Farklı mesaj türlerinin aynı uzunlukta olması mümkündür; uzunluk tür seçmek için kullanılmaz.

```text
CAN veri alanı = [07: ISO-TP Single Frame başlığı] [7 bayt uygulama verisi]
Toplam         = 8 bayt
```

Mevcut ISO-TP kütüphanesinin tek çerçeve sınırı 7 bayt uygulama verisidir. Dolgu ayarı etkin olsa da bu pakette ek dolgu baytı yoktur.

## Örnek paket

Alanların nasıl yerleştiğini görmek için bir örnek. Bu veri cihazdan alınmadı; gerilim okumasının çalıştığını göstermiyor.

```text
Uygulama verisi: 03 74 0E 01 02 18 FC
CAN veri alanı:  07 03 74 0E 01 02 18 FC
```

Tür `0x03`, gerilim 3700 mV, oynatıcı running (`1`), charger deşarj (`2`), akım −1000 mA (`0xFC18`).

## Kontroller ve kaynak kod

Debug derlemesi geçti. Donanım çağrılarını taklit ederek gerçek dispatcher ve ISO-TP kodunu bilgisayarda çalıştırdık. CAN’deki 8 baytı, üç modu, reverse önceliğini, akımın işaretini ve sınırlarını kontrol ettik. Aralık dışı akımda, meşgul bağlantıda ve gönderim hatasında ne olduğunu da denedik. Kart üzerinde test henüz yapılmadı.

Kaynaklar `tronloop-vertex-firmware/` deposuna göredir:

- `Core/Inc/tl_dispatcher.h`: şema, mod kodları ve boyut/ofset kontrolleri.
- `Core/Src/tl_dispatcher.c`: alan kaynakları, mod eşlemesi, aralık kontrolü ve gönderim.
- `Core/Inc/tl_context.h`, `Core/Src/tl_context.c`: durum ve ölçüm alanları.
- `Core/Src/tl_scenario_player.c`: şarj/deşarj bayraklarının kullanımı.
- `Middlewares/isotp-c/isotp.c`, `isotp_config.h`: taşıma ve dolgu davranışı.

[Mesajlaşma protokolü](communication-notes.md) · [Vertex mesaj envanteri](vertex-message-inventory.md)
