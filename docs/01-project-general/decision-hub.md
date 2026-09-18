---
baslik: "Tronloop — Karar ve Mimari Defteri"
kategori: "01-project-general"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Tronloop — Karar ve Mimari Defteri

**Son Güncelleme:** 2026-09-18

Burası Tronloop’un ana defteri. Sistemi nasıl kuruyoruz, neyi neden seçtik, hangi işler kaldı; hepsini burada topluyoruz. Geri dönüp okuduğumuzda yalnızca sonucu değil, o karara nasıl geldiğimizi de bulabilelim istiyoruz.

## Ana merkez ve kaynak kapsamı

Kararlar ve notlar **tronloop-defteri-kebir** içinde kalıyor. Diğer klasörlerde kod ve tasarım var. **Adı `__` ile biten klasörleri kullanmıyoruz**; onlar eski ve geçersiz. [ADR-0002](../07-decisions/ADR-0002-documentation-scope.md).

## Adlandırma

Test grubuna **Cluster**, Linux sunucusuna **ClusterPilot**, pil test birimine **Vertex**, bulut sunucusuna **TSphere** diyoruz. Eski adların karşılıkları [isimler sayfasında](terminology.md).

## Kayıt alanları

| Alan | İçerik |
|---|---|
| [Karar kayıtları](../07-decisions/README.md) | Karar, gerekçe, alternatifler ve etkiler |
| [Mesajlaşma protokolü](../03-software/communication-notes.md) | Ölçüm, tampon, saat eşitleme, komut/yanıt akışları ve kalan protokol ayrıntıları |
| [Mimari çalışma notları](../03-software/architecture-notes.md) | Sorumluluklar, bağımlılıklar ve açık mimari konular |
| [Görüşme kayıtları](../06-meetings/notes/2026-09-18-documentation.md) | Görüşme özeti ve ilgili kararlar |
| [Değişiklik geçmişi](../../CHANGELOG.md) | Belge değişikliklerinin özeti |

## Yazım dili

Burayı resmi bir rapor gibi değil, kendi proje defterimiz gibi yazıyoruz. Kısa, rahat okunan cümleler kullanıyoruz. Bir şey daha fikir aşamasındaysa onu da söylüyoruz; hazır olmayan işi hazırmış gibi anlatmıyoruz.

## Kayıt ilkeleri

- **Öneri:** Değerlendirilen seçenek; henüz karar değildir.
- **Kesinleşmiş karar:** Projede uygulanmasına karar verilen yaklaşım.
- **Doğrulanmış uygulama:** Kaynak dosya ve inceleme tarihiyle desteklenen mevcut davranış.
- **Açık konu:** Henüz yanıtlanmamış soru veya doğrulanmamış ayrıntı.

Bir konu netleşince ilgili sayfaya yazıyoruz. Karar değişirse eski kaydı silmiyoruz, yenisine bağlıyoruz. Tarih ya da sorumlu belli değilse boş bırakıyoruz.

## Güncel sistem özeti

Her Cluster’da Vertex’ler var. ClusterPilot bunlardan veriyi alıp TSphere’e MQTT üzerinden gönderiyor. Buluta ulaşamazsa kayıtlar SQLite’ta bekliyor. Panelden gelen komutları da doğru cihaza iletiyor ve yanıtı geri taşıyor. [ADR-0003](../07-decisions/ADR-0003-system-overview.md).

Testi Vertex kendi başına çalıştıracak; [ADR-0004](../07-decisions/ADR-0004-autonomous-vertex.md). Kısa kesintiler için bir ring buffer olacak. Dolunca en eski kaydın üzerine yazacak; bağlantı gelince elde kalan kayıtlar gönderilecek. Yani bütün veriyi kesin saklama iddiamız yok. [ADR-0006](../07-decisions/ADR-0006-vertex-ring-buffer.md), ADR-0005’in yerine geçmiştir. Kapasiteyi, bellek türünü ve yeniden başlama davranışını daha belirleyeceğiz.

Kayıtlarda milisaniye cinsinden zaman ve artan bir sıra numarası istiyoruz. Sayacı her testte sıfırlamaya gerek yok. STM32 saati de Linux zamanıyla, belli aralıklarla gönderilen mesajlarla eşitlenecek. [ADR-0007](../07-decisions/ADR-0007-measurement-time-sequence.md).

Mesaj türünü uzunluktan değil tür alanından okuyoruz. Genel durum **7 bayt**: oynatıcı ve charger modu ayrı birer bayt, akım `int16_t` mA. Charger modu idle/şarj/deşarj olarak gidiyor. Firmware hazır, derleme ve bilgisayardaki paket kontrolleri geçti; alıcı güncellemesi ve kart testi kaldı. [ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md), ADR-0008’in yerine geçmiştir.

Kapasite hesabının temeli: **yaklaşık 16 Vertex, 100 ms ölçüm aralığı**. [Mesajlaşma belgesindeki hesap](../03-software/communication-notes.md); güncel 17 baytlık telemetriyle heartbeat ve durum dahil hesaplanan hat yükü yaklaşık %15–18,3’tür (500 kbit/s ve belgelenen varsayımlarla).

Güncel telemetri firmware biçimi **17 bayt**: iki ayrı 2 bayt sıcaklık (°C × 10) ve 8 bayt Unix ms zamanı içerir. [ADR-0011](../07-decisions/ADR-0011-telemetry-time-temperature.md). Sıra numarası ve dairesel tampon henüz kodda uygulanmadı.

## Açık konular

| Kimlik | Konu | Durum | Kaynak |
|---|---|---|---|
| OPEN-001 | Mevcut mimari belgeleri ile güncel bileşenlerin eşleştirilmesi | Doğrulama bekliyor | 2026-09-18 dokümantasyon incelemesi |
| OPEN-002 | Haberleşme hatlarının ve mesaj biçimlerinin ayrıntılandırılması | Davranış taslağı hazır; paket ve konu şemaları açık | 2026-09-18 proje notları |

## Kaynak ve kapsam

Bu defterin başlangıcı 2026-09-18 notları. Vertex’in mesajlaşma ve RTC kodunu inceledik. Diğer bileşenlerin tamamını henüz uçtan uca kontrol etmedik; başka yerde kalan notları da buraya geldikçe ekleyeceğiz.
