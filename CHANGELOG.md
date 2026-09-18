*🔋 TRONLOOP — Batarya Kapasite Kayıp İzleme Sistemi*

---

# Değişiklik Geçmişi

Tüm önemli değişiklikler bu dosyada belgelenir.
Format: `[v{YIL}.{AY}.{HAFTA}] - YYYY-MM-DD`

---

## [v2026.09.3] - 2026-09-18

### Güncellendi
- ADR-0007: Ölçüm zamanı, sıra numarası ve Linux/Unix zamanıyla periyodik STM32 RTC eşitleme kaydedildi; mevcut RTC/CAN kodu ile hedef protokol ayrıntıları ayrıldı.
- ADR-0006, ADR-0005’in yerine geçti: Vertex tamponu kısa kesintilere yönelik, sınırlı ve dolduğunda en eski kayıtların üzerine yazan dairesel tampon olarak netleştirildi; kayıpsız teslim varsayımı kaldırıldı.
- ADR-0005: Vertex’in bağlantı kesintisinde test verilerini biriktirmesi ve bağlantı düzelince aktarması kaydedildi; mimari diyagramı ve açık konular güncellendi.
- ADR-0004: Testin Vertex firmware’i üzerinde ClusterPilot’tan bağımsız yürütülmesi kararı ve ilişkili açık konular kaydedildi.
- Vertex firmware kaynaklarından mevcut mesaj envanteri çıkarıldı; CAN/ISO-TP temeli ile yeniden tasarlanacak mesaj tipleri ayrıldı.
- ADR-0003 ile kullanıcının genel sistem anlatımı kaydedildi; cluster/Vertex/ClusterPilot akış diyagramı ve MQTT/SQLite haberleşme envanteri eklendi. Protokol ayrıntıları açık konu olarak tutuldu.
- ADR-0002 ile Defteri Kebir ana dokümantasyon merkezi olarak kullanıcı tarafından kesinleştirildi.
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
