---
baslik: "Vertex — Mevcut Mesaj Envanteri"
kategori: "03-software"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Vertex — Mevcut Mesaj Envanteri

**Son Güncelleme:** 2026-09-18

## Kapsam ve kanıt

Kullanıcı CAN/ISO-TP temelini koruyarak mesaj tiplerini birlikte yeniden değerlendirmek istiyor. Aşağıdakiler 2026-09-18 tarihinde yerel çalışma ağacından okunan mevcut davranıştır; yeni protokol kararı veya donanım üzerinde test sonucu değildir. Genel durum paketi kullanıcının talebiyle 7 bayta güncellendi; diğer mesajlar mevcut kaynak gözlemleridir.

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
| `VertexTelemetryPayload` | `PAYLOAD_TYPE_FAST_TELEMETRY` | `0x01` | 100 ms | Pil gerilimi `uint16_t` mV, akımı `int16_t` mA, sıcaklığı `int16_t` 0,1 °C, durum `uint8_t` |
| `HeartbeatPayload` | `PAYLOAD_TYPE_HEARTBEAT` | `0x02` | 500 ms | Context içindeki senaryo oynatıcı durumu |
| `VertexStatusPayload` | `PAYLOAD_TYPE_GENERAL_STATUS` | `0x03` | 3000 ms | Pil gerilimi, oynatıcı durumu, charger modu (0 idle, 1 şarj, 2 deşarj), ölçülen pil akımı `int16_t` mA |

**Adlandırma notu:** Yapı adları güncel kodla eşleştirildi. Tür sabitlerinin adları değiştirilmedi; bu nedenle kodda `FAST_TELEMETRY` ve `GENERAL_STATUS` hâlâ bulunur. Bunlar eski yapı adı değil, mevcut tür sabitleridir. `HeartbeatPayload` için yeni ad henüz seçilmedi.

Aralıklar hedef deneme zamanlarıdır; teslim garantisi değildir. `VertexTelemetryPayload` ve `HeartbeatPayload` ISO-TP meşgulse o tur gönderilmez. `VertexStatusPayload` meşgul bağlantının boşalmasını bekler ve diğer periyodik paketlerden önce denenir.

`VertexStatusPayload` sabit 7 bayttır. [Alanlar, bayt yerleşimi ve durum kodları](general-status-message.md) ayrı belgede açıklanmıştır. Diğer iki pakette tür alanı C enum olarak tanımlanmıştır; `packed` olması bu alanın tek bayt olduğunu kanıtlamaz. Tel üzerindeki boyutları yalnızca yorumlardan çıkarmamak gerekir.

`VertexTelemetryPayload` içindeki `state` sabit `1` gönderilir. `HeartbeatPayload` context durumunu, `VertexStatusPayload` ise doğrudan senaryo oynatıcısının durumunu kullanır. `VertexStatusPayload` içindeki charger modu context bayraklarından üretilir, donanım geri okuması değildir. Gerilim ölçümünün güncellenmesi henüz uygulanmamıştır. Ölçüm alanının pakette bulunması, geçerli ölçüm üretildiği anlamına gelmez.

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

[ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md), ADR-0008'in yerine geçmiştir. Tür, mesajın tür alanından belirlenir; farklı türler aynı uzunlukta olabilir. Uzunluk, seçilen türe göre doğrulanır. Genel durumdaki iki şarj bayrağı tek idle/şarj/deşarj çalışma moduyla değiştirildi. Mod ve oynatıcı durumu ayrı birer bayttır; akım `int16_t` mA olarak 2 bayt, toplam 7 bayttır. Bu değişiklik firmware’de uygulandı; alıcı uyarlaması henüz yapılmadı.
