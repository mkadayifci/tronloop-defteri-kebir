---
baslik: "Genel Durum Mesajı — 7 Bayt"
kategori: "03-software"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Genel Durum Mesajı — 7 Bayt

**Son Güncelleme:** 2026-09-18

## Uygulama durumu

[ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md) ile belirlenen yapı 2026-09-18 tarihinde Vertex firmware'ine uygulandı. Önceki 10 baytlık biçim yerine **7 baytlık VertexStatusPayload** gönderilir. Alıcı yazılımın yeni yerleşime uyarlanması ayrıca gerekir; bu çalışma kapsamında alıcı kodu değiştirilmedi. TSphere üzerindeki MQTT konu ve içerik şeması henüz seçilmedi.

| Özellik | Değer |
|---|---|
| Yön | Vertex → ClusterPilot |
| Taşıma | CAN/ISO-TP, tek çerçeve |
| Uygulama verisi | **7 bayt** |
| Tür | `0x03` (`PAYLOAD_TYPE_GENERAL_STATUS`) |
| Çok baytlı alan sırası | Little-endian |
| Hedef gönderim aralığı | 3000 ms |

## Alanlar ve bayt yerleşimi

Konumlar uygulama verisinde sıfırdan başlar; ISO-TP başlığı dahil değildir.

| Bayt | Alan | Tür / boyut | Birim veya anlam |
|---|---|---|---|
| 0 | `payload_type` | `uint8_t` / 1 bayt | `0x03` |
| 1–2 | `battery_voltage_mv` | `uint16_t` / 2 bayt | Pil gerilimi, mV |
| 3 | `scenario_player_state` | `uint8_t` / 1 bayt | Oynatıcı durumu |
| 4 | `charger_mode` | `uint8_t` / 1 bayt | Idle / şarj / deşarj |
| 5–6 | `battery_current_ma` | `int16_t` / 2 bayt | Pil akımı, mA; pozitif şarj, negatif deşarj |

Toplam: **1 + 2 + 1 + 1 + 2 = 7 bayt**. Paket boyutu ve alan ofsetleri derleme zamanı kontrolleriyle sabitlenmiştir. Ölçüm zamanı, sıra numarası, sıcaklık, SOC/SOH, test kimliği ve hata ayrıntısı bu pakette yoktur. Oynatıcı ve charger modu ayrı baytlardır; ortak bit alanı kullanılmaz.

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

Mod kodları uygulama sırasında seçilmiştir. Mevcut senaryo kodu deşarjda `enabled=false`, `is_reverse_mode=true` kullanır; bu nedenle reverse kontrolü önceliklidir. Her iki bayrak da açıksa deşarj kodlanır.

## Veri kaynakları ve sınırlar

- Oynatıcı durumu doğrudan `g_scenario_player.playerState` üzerinden alınır.
- Charger modu yazılımın context kontrol durumudur; donanımdan doğrulanmış fiziksel çalışma bilgisi değildir.
- Gerilim `g_tl_context.dut.voltage_mV` üzerinden alınır. Mevcut `tl_context.c` gerilimi sıfırla başlatır ve ölçüm güncellemesi yorum satırındadır; alanın varlığı ölçümün uygulandığı anlamına gelmez.
- Akım `g_tl_context.dut.current_mA` üzerinden alınır; bu alan fuel gauge ölçümünden güncellenir. Dahili context alanı `int32_t` kalmıştır.
- Akım −32768…+32767 mA dışındaysa **genel durum paketi o tur gönderilmez**, hata loglanır ve sonraki deneme 3 saniye sonra yapılır. Değer kırpılmaz veya taşarak farklı bir ölçüm gibi gönderilmez. Bu koruma genel durum paketine aittir; diğer telemetri davranışları değiştirilmedi.

## Gönderim ve ayrıştırma

Gönderim zamanı geldiğinde ISO-TP meşgulse paket bekler; uygun olduğunda diğer periyodik paketlerden önce denenir. Gönderim başlatma hatası loglanır ve sonraki deneme 3 saniye sonradır. Bu aralık teslim garantisi değildir.

Alıcı, ISO-TP başlığını ayırdıktan sonra uygulama tür alanını okumalı ve `0x03` için 7 bayt beklemelidir. Farklı mesaj türlerinin aynı uzunlukta olması mümkündür; uzunluk tür seçmek için kullanılmaz.

```text
CAN veri alanı = [07: ISO-TP Single Frame başlığı] [7 bayt uygulama verisi]
Toplam         = 8 bayt
```

Mevcut ISO-TP kütüphanesinin tek çerçeve sınırı 7 bayt uygulama verisidir. Dolgu ayarı etkin olsa da bu pakette ek dolgu baytı yoktur.

## Örnek paket

Alan yerleşimi örneğidir; cihazdan alınmış ölçüm değildir. Gerilim ölçümünün çalıştığına ilişkin kanıt olarak kullanılmaz.

```text
Uygulama verisi: 03 74 0E 01 02 18 FC
CAN veri alanı:  07 03 74 0E 01 02 18 FC
```

Tür `0x03`, gerilim 3700 mV, oynatıcı running (`1`), charger deşarj (`2`), akım −1000 mA (`0xFC18`).

## Doğrulama ve kaynaklar

Debug firmware derlemesi başarılı. Gerçek dispatcher ve ISO-TP kütüphanesi, donanım çağrıları taklit edilerek bilgisayarda çalıştırıldı: tel üzerindeki 8 bayt, üç mod, reverse önceliği, pozitif/negatif akım, `int16_t` sınırları, sınır dışı kayıt, meşgul ISO-TP bağlantısı ve gönderim hatası sonrası deneme zamanı doğrulandı. Kart üzerinde test yapılmadı.

Kaynaklar `tronloop-vertex-firmware/` deposuna göredir:

- `Core/Inc/tl_dispatcher.h`: şema, mod kodları ve boyut/ofset kontrolleri.
- `Core/Src/tl_dispatcher.c`: alan kaynakları, mod eşlemesi, aralık kontrolü ve gönderim.
- `Core/Inc/tl_context.h`, `Core/Src/tl_context.c`: durum ve ölçüm alanları.
- `Core/Src/tl_scenario_player.c`: şarj/deşarj bayraklarının kullanımı.
- `Middlewares/isotp-c/isotp.c`, `isotp_config.h`: taşıma ve dolgu davranışı.

[Mesajlaşma protokolü](communication-notes.md) · [Vertex mesaj envanteri](vertex-message-inventory.md)
