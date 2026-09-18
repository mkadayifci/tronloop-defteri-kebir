---
baslik: "Tronloop — Karar ve Mimari Defteri"
kategori: "01-project-general"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Tronloop — Karar ve Mimari Defteri

**Son Güncelleme:** 2026-09-18

Bu alan, proje görüşmelerinin kalıcı ve izlenebilir özetidir. Konuşmaların tamamı yerine kararlar, gerekçeler, mimari bilgiler ve açık konular kaydedilir.

## Ana merkez ve kaynak kapsamı

Kullanıcının kararıyla tüm karar ve dokümantasyon yönetiminin ana merkezi **tronloop-defteri-kebir** deposudur. Diğer klasörler kod ve tasarım kaynaklarıdır. **Adı `__` ile biten klasörler geçersizdir** ve güncel mimari incelemelerinde kullanılmaz. [ADR-0002](../07-decisions/ADR-0002-documentation-scope.md).

## Adlandırma

Belgelerde **Cluster**, **ClusterPilot**, **Vertex** ve bulut sunucusu için **TSphere** adları kullanılır. [Adlandırma rehberi](terminology.md), eski terimlerin bağlama göre karşılıklarını ve korunacak teknik adları tanımlar.

## Kayıt alanları

| Alan | İçerik |
|---|---|
| [Karar kayıtları](../07-decisions/README.md) | Karar, gerekçe, alternatifler ve etkiler |
| [Mesajlaşma protokolü](../03-software/communication-notes.md) | Ölçüm, tampon, saat eşitleme, komut/yanıt akışları ve açık sözleşme ayrıntıları |
| [Mimari çalışma notları](../03-software/architecture-notes.md) | Sorumluluklar, bağımlılıklar ve açık mimari konular |
| [Görüşme kayıtları](../06-meetings/notes/2026-09-18-documentation.md) | Görüşme özeti ve ilgili kararlar |
| [Değişiklik geçmişi](../../CHANGELOG.md) | Belge değişikliklerinin özeti |

## Kayıt ilkeleri

- **Öneri:** Değerlendirilen seçenek; henüz karar değildir.
- **Kesinleşmiş karar:** Kullanıcının açıkça seçtiği veya kesinleştiğini belirttiği yaklaşım.
- **Doğrulanmış uygulama:** Kaynak dosya ve inceleme tarihiyle desteklenen mevcut davranış.
- **Açık konu:** Henüz yanıtlanmamış soru veya doğrulanmamış ayrıntı.

Her anlamlı görüşmeden sonra ilgili konu belgesi ve gerekiyorsa karar kaydı güncellenir. Karar değişirse eski kayıt korunur ve yeni kayda bağlantı verilir. Sorumlu veya hedef tarih belirtilmediyse uydurulmaz.

## Güncel sistem özeti

Tronloop pil test sisteminde Cluster’lar, Vertex’leri içerir. Linux sunucusu ClusterPilot, Vertex verilerini TSphere üzerindeki MQTT’ye taşır; gönderilemeyen verileri SQLite’ta biriktirir. Panel kaynaklı MQTT komutlarını cihazlara yönlendirir ve yanıtlarını MQTT’ye gönderir. [ADR-0003](../07-decisions/ADR-0003-system-overview.md).

Testi Vertex firmware’i ClusterPilot’a sürekli ihtiyaç duymadan yürütür; [ADR-0004](../07-decisions/ADR-0004-autonomous-vertex.md). Vertex kısa kesintiler için sınırlı dairesel tampon kullanır; dolduğunda en eski kayıtların üzerine yazar. Bağlantı düzelince elde kalan bekleyen kayıtlar aktarılır; kayıpsız teslim garantisi yoktur. [ADR-0006](../07-decisions/ADR-0006-vertex-ring-buffer.md), ADR-0005’in yerine geçmiştir. Kapasite, bellek ortamı ve yeniden başlama politikası açık konudur.

Milisaniye çözünürlüğünde ölçüm zamanı ve artan ayırt edici sıra numarası kayıtlarda yer alacak; sayaç için test başında sıfırlama şartı yoktur. STM32 saati Linux/Unix zamanı ile periyodik mesajlar üzerinden eşitlenecek. [ADR-0007](../07-decisions/ADR-0007-measurement-time-sequence.md).

Mesaj türü tür alanından belirlenir; farklı türler aynı uzunlukta olabilir. Uzunluk seçilen türe göre doğrulanır. Genel durumdaki şarj/ters mod bayrakları tek idle/şarj/deşarj çalışma moduyla değiştirilecek; bu mod ve oynatıcı durumu ayrı birer bayt olarak taşınacak. Akım mA cinsinden işaretli `int16_t` (2 bayt) olacak; hedef genel durum paketi 7 bayt. [ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md), ADR-0008’in yerine geçmiştir.

## Açık konular

| Kimlik | Konu | Durum | Kaynak |
|---|---|---|---|
| OPEN-001 | Mevcut mimari belgeleri ile güncel bileşenlerin eşleştirilmesi | Doğrulama bekliyor | 2026-09-18 dokümantasyon incelemesi |
| OPEN-002 | Haberleşme hatlarının ve mesaj sözleşmelerinin ayrıntılandırılması | Davranış taslağı hazır; paket ve konu şemaları açık | 2026-09-18 kullanıcı talebi |

## Kaynak ve kapsam

Başlangıç kaynağı: 2026-09-18 tarihli bu görüşme. Mevcut dokümantasyon incelendi; Vertex mesajlaşma ve RTC kaynakları incelendi; diğer bileşenlerin genel mimariye uyumu henüz koddan doğrulanmadı. Diğer görüşmeler ancak içeriklerine erişildiğinde kayda alınabilir.

