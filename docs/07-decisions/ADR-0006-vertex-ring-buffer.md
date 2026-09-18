---
baslik: "ADR-0006 — Vertex'te kısa kesintiler için dairesel tampon"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0006 — Vertex'te kısa kesintiler için dairesel tampon

Bağlantının normalde açık kalacağını varsayıyoruz. Vertex’teki **ring buffer**, kısa kesintilerde aradaki ölçümleri tutmak için var. Bütün veriyi ne olursa olsun saklayan bir arşiv yapmıyoruz.

Gönderim kesilince test ve kayıt devam edecek. Tampon dolarsa yeni kayıt en eski kaydın üzerine yazılacak; o eski kayıt henüz gönderilmemiş de olabilir. Bağlantı gelince elde kalan bekleyen kayıtlar aktarılacak. Üzerine yazılanlar kaybolmuş olacak.

Burada öncelik testi sürdürmek. Tampon doldu diye testi veya ölçümü durdurmuyoruz. “ClusterPilot kalıcı kaydı onaylayana kadar Vertex hiçbir şeyi silmesin” yaklaşımını bu yüzden kullanmıyoruz. Sonradan teslim onayı ya da tekrar gönderme eklersek de tamponun dönmesini engellemeyecek.

ClusterPilot’un buluta gönderemediği verileri SQLite’ta tutması ayrı konu; onun davranışı bu kararla değişmiyor.

## Henüz belirlemediklerimiz

- Tampon ne kadar büyük olacak, kaç saniyelik kesintiyi karşılayacak?
- RAM mi başka bir bellek mi kullanacağız? Güç kesilince koruma şartı seçmedik.
- Okuma/gönderme konumu ne zaman ilerleyecek, tekrar gelen kayıt ne olacak?
- Üzerine yazılan kayıtların oluşturduğu boşluğu ClusterPilot’a nasıl bildireceğiz?
- Birikmiş veriyle canlı ölçüm hangi sırayla gidecek?

Zaman ve artan sıra numarası [ADR-0007](ADR-0007-measurement-time-sequence.md) içinde. Zamanın paket biçimi sonradan belirlendi; sayacın genişliği, taşması ve yeniden başlama davranışı hâlâ açık.

**Kayıt:** 2026-09-18 · Kabul edildi; [ADR-0005](ADR-0005-vertex-data-buffer.md) yerine geçti. Ring buffer henüz firmware’de yok.

[Mimari notlar](../03-software/architecture-notes.md) · [Haberleşme notları](../03-software/communication-notes.md)
