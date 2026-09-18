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

Kullanıcı CAN/ISO-TP temelini koruyarak mesaj tiplerini birlikte yeniden değerlendirmek istiyor. Aşağıdakiler 2026-09-18 tarihinde yerel çalışma ağacından okunan mevcut davranıştır; yeni protokol kararı veya donanım üzerinde test sonucu değildir. Firmware kodu değiştirilmedi.

Kaynak yolları çalışma alanı köküne göredir:

- `tronloop-vertex-firmware/Core/Inc/tl_can.h`
- `tronloop-vertex-firmware/Core/Src/tl_can.c`
- `tronloop-vertex-firmware/Core/Inc/tl_dispatcher.h`
- `tronloop-vertex-firmware/Core/Src/tl_dispatcher.c`
- `tronloop-vertex-firmware/Core/Src/tl_command_parser.c`

## Taşıma

CAN/ISO-TP bağlantısı, sabit `0x100` cihaz kimliği ve ayrı ayrı 1024 bayt gönderme/alma tamponları kullanıyor. Tamamlanan ISO-TP alımları komut ayrıştırıcısına aktarılıyor. Çoklu Vertex adresleme planı bu değer üzerinden kesinleştirilmedi.

## Vertex → ClusterPilot: periyodik mesajlar

| Tür | Mevcut kod | Hedef aralık | İçerik |
|---|---|---|---|
| Hızlı telemetri | `0x01` | 100 ms | Pil gerilimi `uint16` mV, akımı `int16` mA, sıcaklığı `int16` 0,1 °C, durum `uint8` |
| Heartbeat | `0x02` | 500 ms | Context içindeki senaryo oynatıcı durumu |
| Genel durum | `0x03` | 3000 ms | Pil gerilimi, oynatıcı durumu, charger enabled/reverse bayrakları, ölçülen pil akımı `int32` mA |

Aralıklar hedef deneme zamanlarıdır; teslim garantisi değildir. Hızlı telemetri ve heartbeat ISO-TP meşgulse o tur gönderilmez. Genel durum meşgul bağlantının boşalmasını bekler ve diğer periyodik mesajlardan önce denenir.

Genel durum paketi sabit 10 bayttır. Diğer iki pakette tür alanı C enum olarak tanımlanmıştır; `packed` olması bu alanın tek bayt olduğunu kanıtlamaz. Tel üzerindeki boyutları yalnızca yorumlardan çıkarmamak gerekir.

Hızlı telemetride `state` sabit `1` gönderilir. Heartbeat context durumunu, genel durum ise doğrudan senaryo oynatıcısının durumunu kullanır. Genel durumdaki charger bayrakları donanım geri okuması değildir; başlık açıklaması gerilim ölçümünün henüz güncellenmediğini belirtir. Ölçüm alanının pakette bulunması, geçerli ölçüm üretildiği anlamına gelmez.

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

Sonraki tasarımda komutun alınması ile uygulanması, mesajların kimlikleri, ölçüm ve durum ayrımı, olayların saklanması, bağlantı kesintisi ve yinelenen komut davranışı netleştirilecek. Sayısal mesaj kodları ve alan yerleşimleri henüz seçilmedi.
