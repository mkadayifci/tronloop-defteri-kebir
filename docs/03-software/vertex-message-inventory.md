---
baslik: "Vertex — Mevcut Mesaj Envanteri"
kategori: "03-software"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Vertex — Mevcut Mesaj Envanteri

**Son Güncelleme:** 2026-09-18

## Kodda hangi mesajlar var?

CAN/ISO-TP’yi koruyoruz, mesajları ise ihtiyaçlarımıza göre düzenliyoruz. Bu sayfa 2026-09-18’de firmware’de gördüğümüz durumu anlatıyor. Telemetri 13, genel durum 11 bayt. Kart üzerinde uçtan uca test henüz yapılmadı.

Kaynak yolları çalışma alanı köküne göredir:

- `tronloop-vertex-firmware/Core/Inc/tl_can.h`
- `tronloop-vertex-firmware/Core/Src/tl_can.c`
- `tronloop-vertex-firmware/Core/Inc/tl_dispatcher.h`
- `tronloop-vertex-firmware/Core/Src/tl_dispatcher.c`
- `tronloop-vertex-firmware/Core/Src/tl_command_parser.c`

## Taşıma

CAN/ISO-TP bağlantısı, sabit `0x100` cihaz kimliği ve ayrı ayrı 1024 bayt gönderme/alma tamponları kullanıyor. Tamamlanan ISO-TP alımları komut ayrıştırıcısına aktarılıyor. Çoklu Vertex adresleme planı bu değer üzerinden kesinleştirilmedi.

## Vertex → ClusterPilot: periyodik mesajlar

| Güncel C yapı adı | Tür sabiti | Kod | Hedef aralık | İçerik |
|---|---|---|---|---|
| `VertexTelemetryPayload` | `PAYLOAD_TYPE_FAST_TELEMETRY` | `0x01` | 100 ms | Pil gerilimi `uint16_t` mV, akımı `int16_t` mA, `uint64_t` Unix ms paket zamanı (yalnız RUNNING) |
| `HeartbeatPayload` | `PAYLOAD_TYPE_HEARTBEAT` | `0x02` | 500 ms | Context içindeki senaryo oynatıcı durumu |
| `VertexStatusPayload` | `PAYLOAD_TYPE_GENERAL_STATUS` | `0x03` | 3000 ms | Pil gerilimi, oynatıcı durumu, charger modu (0 idle, 1 şarj, 2 deşarj), ölçülen pil akımı `int16_t` mA, ayrı int16_t pil/ortam sıcaklığı °C × 10 |

Yapı adlarını yeniledik ama tür sabitlerini aynı bıraktık. Kodda `FAST_TELEMETRY` ve `GENERAL_STATUS` görmemizin nedeni bu. `HeartbeatPayload` için henüz başka bir ad seçmedik. ClusterPilot tarafında ise eski `FastTelemetryPayload` hâlâ duruyor; alıcı güncellemesi bekliyor.

Bunlar hedef gönderim aralıkları. ISO-TP meşgulse telemetri ve heartbeat o tur atlanıyor. Genel durum bağlantının boşalmasını bekliyor. Önce genel durum, ardından hızlı telemetri ve heartbeat deneniyor.

Telemetri 13, genel durum 11 bayt. Genel durumun [alanları ve kodları](general-status-message.md) ayrı sayfada. Telemetride türü açıkça `uint8_t` tuttuk. Heartbeat ise hâlâ C enum kullanıyor; `packed` yazması tek başına alanın bir bayt olduğunu söylemiyor. Boyuta yorumdan değil derleyicinin ürettiği düzenden bakıyoruz.

Telemetriden sabit `state` alanını çıkardık. Heartbeat durumu context’ten, genel durum ise doğrudan senaryo oynatıcısından alıyor; bu iki kaynağın aynı kaldığını henüz doğrulamadık. Charger modu da context bayraklarından geliyor, donanım geri okuması değil. Gerilim alanı pakette var ama okuması henüz bağlı değil.

## ClusterPilot → Vertex: mevcut komutlar

Başlık dört adet `uint8` alanından oluşur: `command`, `version`, `sequence`, `flags`. Ayrıştırıcı sürüm olarak `0x01` bekler.

| Kod | Komut | Ayrıştırılan ek veri | Mevcut davranış |
|---|---|---|---|
| `0x70` | PING | Yok | Log |
| `0x21` | CHARGER_ENABLE | Yok | Log; uygulama çağrısı yorum satırında |
| `0x22` | CHARGER_DISABLE | Yok | Log; uygulama çağrısı yorum satırında |
| `0x10` | SCRIPT_START | Bir bayt script kimliği | Log; uygulama çağrısı yorum satırında |
| `0x11` | SCRIPT_STOP | Yok | Log; uygulama çağrısı yorum satırında |
| `0x23` | SET_CHARGE_LIMITS | `uint16` hedef mV + `int16` akım mA; little-endian | Log; uygulama çağrıları yorum satırında |

İncelenen ayrıştırıcı ağ üzerinden komut yanıtı üretmiyor. `sequence` loglanıyor; `flags` için davranış tanımlanmamış. Başlık ve gerekli en küçük veri uzunlukları kontrol ediliyor.

## Ayrı/eski tanımlar

- Ham CAN alım yolunda `0x01` + dört bayt little-endian Unix zamanı biçiminde RTC ayarlama işlemi de var; bu, ISO-TP üzerindeki dört baytlık komut başlığından farklı bir yol.
- `TlpStatusPacket` tanımı mevcut; incelenen dispatcher gönderimlerinde kullanılmıyor.

## Yeni tasarım için öneriler — henüz karar değil

| Mesaj ailesi | Önerilen amaç |
|---|---|
| Ölçüm | Teste ait ölçümler, ölçüm zamanı ve geçerlilik bilgisi |
| Durum | Cihazın ve yürüyen testin güncel durumu |
| Olay | Adım geçişi, test bitişi veya hata gibi değişiklikler |
| Komut | Cihazdan yapılması istenen işlem |
| Komut sonucu | Komutun kabul/ret bilgisi ve uygulanma sonucu |

Sonraki tasarımda komutun alınması ile uygulanması, mesajların kimlikleri, ölçüm ve durum ayrımı, olayların saklanması, bağlantı kesintisi ve yinelenen komut davranışı netleştirilecek. Yukarıdaki mevcut tür kodları ve uygulanmış `VertexStatusPayload` yerleşimi geçerlidir; yeni mesaj aileleri ve henüz uygulanmamış alanlar için kod/yerleşim kararları ayrıca alınacak.

## Yeni tasarımda tür alanı ve uzunluk doğrulaması

[ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md), ADR-0008'in yerine geçmiştir. Tür, mesajın tür alanından belirlenir; farklı türler aynı uzunlukta olabilir. Uzunluk, seçilen türe göre doğrulanır. Genel durumdaki iki şarj bayrağı tek idle/şarj/deşarj çalışma moduyla değiştirildi. Mod ve oynatıcı durumu ayrı birer bayttır; akım `int16_t` mA olarak 2 bayt, iki sıcaklık alanıyla toplam 11 bayttır. Bu değişiklik firmware’de uygulandı; alıcı uyarlaması henüz yapılmadı.

## Güncel 13 baytlık telemetri

| Bayt | Alan | Tür / birim |
|---|---|---|
| 0 | `payload_type` | uint8_t, 0x01 |
| 1–2 | `battery_voltage_mv` | uint16_t, mV |
| 3–4 | `battery_current_ma` | int16_t, mA |
| 5–12 | `measurement_time_ms` | uint64_t, Unix ms |

Sıcaklıklar yalnız [genel durum mesajında](general-status-message.md) taşınıyor. Ayrı sıcaklık mesajı kaldırıldı. Telemetri 13 bayt; zaman payload oluştururken RTC’den alınıyor. Sıra numarası ve ring buffer henüz yok.

[Telemetri ayrıntıları](vertex-telemetry-message.md) · [Yeni karar](../07-decisions/ADR-0013-separate-temperature-message.md)
