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

Mesaj türünü uzunluktan değil tür alanından okuyoruz. Genel durum **11 bayt**: oynatıcı ve charger modu ayrı birer bayt, akım `int16_t` mA, pil ve ortam sıcaklığı ayrı `int16_t` °C × 10. Charger modu idle/şarj/deşarj olarak gidiyor. Firmware hazır, derleme ve bilgisayardaki paket kontrolleri geçti; alıcı güncellemesi ve kart testi kaldı. [ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md), ADR-0008’in yerine geçmiştir.

16 Vertex’in hepsi 100 ms’de bir telemetri gönderirken, genel durum ve heartbeat dahil hat yükü yaklaşık **%11,7–14,3** (500 kbit/s). [Hesabın ayrıntıları](../03-software/communication-notes.md).

Telemetri artık **13 bayt**: tür, gerilim, akım ve Unix ms zamanı. Sıcaklıklar yalnız 11 baytlık genel durum mesajında. Ayrı sıcaklık mesajını kaldırdık; [ADR-0015](../07-decisions/ADR-0015-remove-temperature-message.md). Sıra numarası ve ring buffer henüz yok.

TSphere’de zaman serileri için InfluxDB kullanacağız. Sürüm ve tüketici servis henüz seçilmedi; kayıt zamanı, tekrar gönderim ve veri şeması için [bir taslak var](../03-software/tsphere-timeseries.md). [ADR-0016](../07-decisions/ADR-0016-tsphere-influxdb.md).

## İki BeagleBone ile yedeklilik

ClusterPilot’u iki BeagleBone ile yedeklemek istiyoruz. Aktif/yedek roller, CAN gönderme yetkisi, devralma ve SQLite kuyruğunun durumu için [bir taslak hazırladık](../03-software/clusterpilot-failover.md). Uygulama yöntemi henüz seçilmedi.

## Açık konular

| Kimlik | Konu | Durum | Kaynak |
|---|---|---|---|
| OPEN-001 | Mevcut mimari belgeleri ile güncel bileşenlerin eşleştirilmesi | Doğrulama bekliyor | 2026-09-18 dokümantasyon incelemesi |
| OPEN-002 | Haberleşme hatlarının ve mesaj biçimlerinin ayrıntılandırılması | Davranış taslağı hazır; paket ve konu şemaları açık | 2026-09-18 proje notları |

## Kaynak ve kapsam

Bu defterin başlangıcı 2026-09-18 notları. Vertex’in mesajlaşma ve RTC kodunu inceledik. Diğer bileşenlerin tamamını henüz uçtan uca kontrol etmedik; başka yerde kalan notları da buraya geldikçe ekleyeceğiz.
