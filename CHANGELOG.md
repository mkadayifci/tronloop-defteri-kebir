---
baslik: "Değişiklik Geçmişi"
kategori: "01-project-general"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

*🔋 TRONLOOP — Batarya Kapasite Kayıp İzleme Sistemi*

---

# Değişiklik Geçmişi

**Son Güncelleme:** 2026-09-18

Önemli değişiklikleri burada kısa notlarla tutuyoruz. Aynı gün içindeki kayıtlar da en yeniden eskiye gidiyor; eski paket boyutları o sıradaki tasarımı anlatıyor.
Format: `[v{YIL}.{AY}.{HAFTA}] - YYYY-MM-DD`

---

## [v2026.09.3] - 2026-09-18

### Güncellendi

- TSphere zaman serileri için InfluxDB’yi seçtik (ADR-0016). Mimariye ekledik; MQTT tüketicisi, zaman damgaları, veri şeması ve saklama düzenini ayrı taslakta topladık. Sürüm ve servis seçimi açık, kurulum yapılmadı.

- Ana mimari sayfasını güncel Cluster–Vertex–ClusterPilot–TSphere yapısıyla yeniden yazdık. Mesajlar, kesinti davranışı, saat, iki BeagleBone hedefi ve kalan işler aynı sayfada. Eski veritabanı/RAID/Compose planını arşive taşıdık.

- Ayrı sıcaklık mesajını ve testteki 10 saniyelik gönderimini kaldırdık. Sıcaklıklar yalnız 11 baytlık genel durumda kaldı; telemetri 13 bayt. Derleme ve paket kontrolleri geçti. 16 Vertex hat hesabı %11,7–14,3.

- Genel duruma pil ve ortam sıcaklığını ekledik. Paket 11 bayt oldu; 3 saniyelik aralık ve testten bağımsız gönderim aynı. Ayrı sıcaklık mesajı korunuyor. Derleme ve ISO-TP paket kontrolleri geçti; 16 Vertex için hat hesabı %11,8–14,4.

- Sıcaklık gönderimini de testin RUNNING durumuna bağladık. Duraklatılmış veya durmuş testte sıcaklık paketi gönderilmiyor; 10 saniyelik aralık aynı.

- Sıcaklıkları hızlı telemetriden çıkardık. Telemetri 13 bayt; yeni VertexTemperaturePayload (0x04) pil/ortam sıcaklığını ve RTC zamanını 10 saniyede bir gönderiyor. Aynı boyuttaki mesajları tür alanı ayırıyor. Derleme ve bilgisayardaki paket/zamanlama kontrolleri geçti. 16 Vertex için bant hesabı %11,6–14,1 olarak güncellendi; alıcı ve sensör okumaları henüz hazır değil.

- İki BeagleBone ile ClusterPilot yedekliliği için taslak eklendi: aktif/yedek roller, fencing, ISO-TP Flow Control, yerel veri kuyruğu ve devralma testleri. Yöntem henüz seçilmedi; kod değişmedi.

- Bütün defteri dil açısından yeniden okuduk. Kararları daha kısa ve doğal anlattık; rapor gibi duran başlıkları azalttık. Eski planları güncel koddan ayırdık.
- Defteri Kebir’in dili sadeleştirildi; görüşme notları konu bazında toparlandı. Doğrudan, doğal Türkçeyle yazma ilkesi ana çalışma kurallarına eklendi. Eski kararların tarihçesi ve uygulama sınırları korundu.
- ADR-0012: Telemetri zamanı payload oluşturulurken doğrudan RTC’den okunuyor; context zaman alanı kaldırıldı. 17 baytlık tel biçimi değişmedi.
- CAN bant hesabı güncel 17 bayt telemetriye göre doğrulandı: 16 Vertex × 10 Hz, ISO-TP Flow Control ve periyodik diğer mesajlarla 75,184–91,440 kbit/s, 500 kbit/s hatta %15,037–18,288.
- Zaman alanını küçültme seçenekleri değerlendirildi; uint64_t Unix milisaniye ve 17 baytlık telemetri biçimi korundu.
- ADR-0011 uygulandı: telemetri sıcaklıkları ayrı int16_t °C×10, zaman uint64_t Unix ms oldu; paket 17 bayt. RTC subsecond API ve context ölçüm zamanı eklendi. Debug derlemesi, çok çerçeveli ISO-TP ve RTC kontrolleri başarılı.
- 16 Vertex ve 100 ms ölçüm aralığı için 500 kbit/s CAN kapasite hesabı eklendi: öneri 19 baytlık telemetri, heartbeat ve durum toplamı belirtilen varsayımlarla yaklaşık %15–18,3 hat yükü. Yeni mesaj biçimi henüz onaylanmadı.
- Telemetride tek CAN çerçevesi hedefi ile ölçüm zamanı/sıra bilgisi arasındaki tercih ve çok çerçeveli ISO-TP önerisi kaydedildi; yeni karar veya kod değişikliği yapılmadı.
- Telemetri sıcaklığı ayrı 1 bayt pil/ortam alanlarına ayrıldı: int8_t, tam °C, −127…+127; −128 geçersiz/ölçüm yok. Ortam context kaynağı eklendi, henüz uygulanmayan sıcaklık okumaları başlangıçta geçersiz işaretleniyor. Paket 7 bayt kaldı; derleme ve bilgisayarda paket/sınır kontrolleri geçti.
- `VertexTelemetryPayload` içindeki sabit `state` kaldırıldı; tür alanı uint8_t ve toplam boyut 7 bayt olarak sabitlendi. Debug derlemesi ve gerçek dispatcher/ISO-TP ile tek çerçeve doğrulaması başarılı.
- `VertexTelemetryPayload` boyutu mevcut Debug ARM derleyicisiyle 8 bayt olarak doğrulandı; alan yerleşimi envantere eklendi.
- Mesaj envanteri güncel C yapı adlarıyla düzenlendi; yapı adları ile değiştirilmemiş tür sabitleri ayrı sütunlarda gösterildi. Protokol taslağındaki adlar da eşitlendi.
- Telemetri yapı adı `VertexTelemetryPayload` olarak güncellendi; alanlar, tür kodu ve gönderim periyodu korunuyor.
- Genel durumun C yapı adı `VertexStatusPayload` olarak değiştirildi; tel biçimi, tür kodu ve gönderim davranışı korunuyor.
- 7 baytlık genel durum Vertex firmware’inde uygulandı: charger_mode 0/1/2, işaretli 2 bayt mA, boyut/ofset kontrolleri ve aralık dışı akımda paket atlama. Debug derlemesi ve gerçek dispatcher/ISO-TP koduyla bilgisayar üzerinde paket kontrolleri başarılı; alıcı ve kart doğrulaması yapılmadı.
- Genel durum hedefinde oynatıcı durumu ve charger çalışma modu ayrı birer bayta ayrıldı. Akım 2 bayt kaldı; toplam 7 bayt uygulama verisi ISO-TP başlığıyla tek CAN çerçevesine sığıyor.
- ISO-TP kütüphanesinde tek çerçeve uygulama verisi sınırı 7 bayt ve dolgu ayarı doğrulandı; hedef 6 baytlık genel durumun ISO-TP ile tek CAN çerçevesinde taşınabileceği belgelendi.
- Genel durum hedefindeki akım alanı mA cinsinden `int16_t` (2 bayt) olarak seçildi; birleşik durum baytıyla toplam hedef boyut 6 bayta güncellendi. Mevcut firmware kodu değişmedi.
- ADR-0010 netleştirildi: Oynatıcı durumu ve idle/şarj/deşarj çalışma modu aynı baytta ayrı bit alanları olarak taşınacak; genel durumun hedef boyutu 8 bayt oldu.
- ADR-0010, ADR-0008’in yerine geçti: tür alanıyla ayrıştırma, türe göre uzunluk doğrulama ve aynı uzunlukta farklı türlere izin verilmesi kararlaştırıldı. Genel durum için tek idle/şarj/deşarj çalışma modu seçildi; firmware değişmedi.
- Genel durum mesajının tek klasik CAN çerçevesine sığma değerlendirmesi eklendi; 8 bayta bit alanlarıyla indirgeme seçeneği yalnızca öneri olarak kaydedildi.
- Genel durum mesajının 10 baytlık alan yerleşimi, oynatıcı durum kodları, gönderim davranışı ve örnek çözümlemesi belgelendi.
- ADR-0009 ile bulut sunucusu TSphere olarak adlandırıldı; MQTT hizmetiyle sunucu adı ayrıldı.
- ADR-0008: CAN mesaj türlerinin `dataLength` ile ayrıştırılması nedeniyle farklı türlerin aynı veri uzunluğunda olamayacağı protokol ve çalışma kuralı olarak eklendi.
- Mesajlaşma diyagramlarında Mermaid tarafından komut ayırıcı olarak yorumlanan iki noktalı virgül kaldırıldı; ölçüm ve saat eşitleme diyagramlarının sözdizimi düzeltildi.
- Mesajlaşma notları kapsamlı protokol taslağına dönüştürüldü: ölçüm alanlarının anlamı, kesinti/tampon davranışı, saat eşitleme, komut/yanıt diyagramları ve açık paket/MQTT ayrıntıları bir araya getirildi.
- Eski dokümantasyon, başlıklar ve diyagramlar Cluster/ClusterPilot/Vertex adlandırmasına uyarlandı; adlandırma rehberi eklendi. Ortak fiziksel altyapı ile Linux sunucusu ayrıldı; teknik yollar ve şema adları korundu.
- ADR-0007: Sıra numarası test başında sıfırlama şartı olmayan, artan bir kayıt ayırt edicisi olarak belirlendi.
- ADR-0007 netleştirildi: Ölçüm zamanında milisaniye çözünürlüğü kabul edildi; mevcut RTC kodunun uyarlanması uygulama işi olarak kaydedildi.
- ADR-0007: Ölçüm zamanı, sıra numarası ve Linux/Unix zamanıyla periyodik STM32 RTC eşitleme kaydedildi; mevcut RTC/CAN kodu ile hedef protokol ayrıntıları ayrıldı.
- ADR-0006, ADR-0005’in yerine geçti: Vertex tamponu kısa kesintilere yönelik, sınırlı ve dolduğunda en eski kayıtların üzerine yazan dairesel tampon olarak belirlendi; kayıpsız teslim varsayımı kaldırıldı.
- ADR-0005: Vertex’in bağlantı kesintisinde test verilerini biriktirmesi ve bağlantı düzelince aktarması kaydedildi; mimari diyagramı ve açık konular güncellendi.
- ADR-0004: Testin Vertex firmware’i üzerinde ClusterPilot’tan bağımsız yürütülmesi kararı ve ilişkili açık konular kaydedildi.
- Vertex firmware kaynaklarından mevcut mesaj envanteri çıkarıldı; CAN/ISO-TP temeli ile yeniden tasarlanacak mesaj tipleri ayrıldı.
- ADR-0003 ile genel sistem yapısı belgelendi; Cluster/Vertex/ClusterPilot akış diyagramı ve MQTT/SQLite haberleşme envanteri eklendi. Protokol ayrıntıları açık konu olarak tutuldu.
- ADR-0002 ile Defteri Kebir ana dokümantasyon merkezi olarak belirlendi.
- `__` ile biten klasörler geçersiz olarak kaydedildi; güncel mimari kaynak kapsamından çıkarıldı.

### Eklendi

- Karar ve Mimari Defteri giriş sayfası ve açık konu takibi.
- ADR karar dizini, kayıt şablonu ve dokümantasyon düzeni kararı.
- Haberleşme ve mimari çalışma notları; doğrulanmamış eski bilgiler açıkça işaretlendi.
- Dokümantasyon düzenine ilişkin görüşme özeti ve çalışma alanında kalıcı asistan talimatları.

---

## [v2026.02.1] - 2026-02-26

### Eklendi

- Proje dokümantasyon yapısı kuruldu (wiki tarzı klasör sistemi)
- `CLAUDE.md` — AI asistan konfigürasyon dosyası oluşturuldu
- `README.md` — Ana proje giriş sayfası oluşturuldu
- `docs/01-proje-genel/` — Proje genel bilgileri için klasör ve şablon dosyaları
- `docs/02-donanim/` — Donanım dokümantasyonu için klasör ve şablon dosyaları
- `docs/03-yazilim/` — Yazılım dokümantasyonu için klasör ve şablon dosyaları
- `docs/04-testler/` — Test protokolü ve sonuç dosyaları için klasör yapısı
- `docs/05-arastirma/` — Literatür ve yayın takip dosyaları
- `docs/06-toplantılar/` — Toplantı notları klasörü

---

_Yeni değişiklikler en üste eklenir._
