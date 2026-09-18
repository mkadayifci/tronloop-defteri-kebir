---
baslik: "ADR-0006 — Vertex'te kısa kesintiler için dairesel tampon"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0006 — Vertex'te kısa kesintiler için dairesel tampon

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Kabul edildi
- **Kaynak:** 2026-09-18 tarihli proje notları.
- **Yerine geçtiği kayıt:** [ADR-0005](ADR-0005-vertex-data-buffer.md).

## Bağlam ve karar

Normal çalışma varsayımı Vertex–ClusterPilot bağlantısının sürekli açık olmasıdır. Vertex test verilerini sınırlı kapasiteli dairesel tamponda (ring buffer) tutar. Amaç kısa gönderim kesintilerini karşılamaktır; tüm verilerin kayıpsız korunması garanti edilmez.

Gönderim kesilse de test ve kayıt devam eder. Tampon dolunca yeni kayıtlar en eski kayıtların üzerine yazılır; henüz gönderilmemiş eski kayıtlar da kaybolabilir. Bağlantı düzeldiğinde tamponda hâlâ bulunan, aktarılmayı bekleyen veriler gönderilir. Üzerine yazılmış kayıtlar geri getirilemez.

## Gerekçe ve sonuçlar

Sürekli bağlantı beklenen sistemde kısa kesintilere tolerans sağlanır. Tamponun karşılayabildiği süreden uzun kesintilerde veri kaybı kabul edilir. Tampon doluluğu, kayıt veya test yürütmesini durdurma koşulu değildir.

ClusterPilot'un buluta gönderilemeyen verileri SQLite'ta biriktirmesi ayrı davranıştır ve bu kararla değiştirilmemiştir.

## Teslim onayı önerisinin durumu

“ClusterPilot kalıcı kaydı onaylayana kadar Vertex veriyi korusun” seçeneği benimsenmedi. Onay gelene kadar eski verinin mutlaka korunması yeni dairesel tampon davranışının şartı olmayacaktır. Gerekirse kullanılacak aktarım onayı veya tekrar mekanizması ayrıca tasarlanır; üzerine yazma davranışını engellemez.

## Açık konular

- Tampon kapasitesi, kayıt sıklığı ve karşılanması hedeflenen kısa kesinti süresi.
- Bellek/depolama ortamı; RAM veya başka ortam henüz seçilmedi. Güç kesintisinde korunma garantisi verilmedi.
- Ölçüm zamanı ve sıra numarası kullanımı [ADR-0007](ADR-0007-measurement-time-sequence.md) ile kesinleşti; milisaniye çözünürlüğü seçildi; sıra numarası test başında sıfırlama şartı olmayan artan bir ayırt edicidir. Alan boyutları, yeniden başlama ve taşma davranışları açık.
- Gönderme/okuma konumu, aktarımın tamamlandığının nasıl belirleneceği ve tekrarların davranışı.
- Üzerine yazılan kayıtların veya veri boşluklarının ClusterPilot'a bildirilme biçimi.
- Birikmiş veri aktarılırken canlı kayıtların aktarım sırası ve önceliği.

## Uygulama durumu

Bu bir hedef davranış kararıdır; kod değiştirilmedi ve firmware'de uygulanmış olduğu doğrulanmadı.

## İlişkili belgeler

[Bağımsız test yürütme](ADR-0004-autonomous-vertex.md) · [Mimari notlar](../03-software/architecture-notes.md) · [Haberleşme notları](../03-software/communication-notes.md)
