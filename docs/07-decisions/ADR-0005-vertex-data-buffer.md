---
baslik: "ADR-0005 — Vertex üzerinde test verilerini biriktirme"
kategori: "07-decisions"
durum: "yerine-yeni-karar-geldi"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0005 — Vertex üzerinde test verilerini biriktirme

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Yerine yeni karar geldi — [ADR-0006](ADR-0006-vertex-ring-buffer.md)
- **Kararı veren:** Kullanıcı
- **Kaynak:** Bu görüşmede Vertex'in test verilerini biriktirip problem çözülünce aktaracağı yönündeki kullanıcı yanıtı.

## Sonraki açıklama

Kullanıcı, bu kaydın kayıpsız veri saklama amacı taşıdığı yorumunu düzeltti. Kısa kesintiler için sınırlı dairesel tampon kullanılacak; dolduğunda en eski kayıtların üzerine yazılacak. Aşağıdaki ilk kayıt tarihçe olarak korunmuştur; geçerli davranış ADR-0006’dadır.

## Bağlam ve karar

Vertex, ClusterPilot'a veri aktaramadığı sırada testi bağımsız yürütmeye devam eder ve test verilerini kendi üzerinde biriktirir. Bağlantı sorunu giderildiğinde birikmiş verileri ClusterPilot'a aktarır.

Bu davranış, ClusterPilot'un buluta gönderemediği verileri SQLite'ta biriktirmesinden ayrı bir katmandır:

| Kesinti | Veriyi biriktiren | Bağlantı düzelince hedef |
|---|---|---|
| Vertex → ClusterPilot | Vertex; depolama ortamı henüz seçilmedi | ClusterPilot |
| ClusterPilot → Bulut | ClusterPilot; SQLite | Buluttaki MQTT üzerinden veri aktarımı |

## Gerekçe ve alternatif

Vertex–ClusterPilot bağlantısının kesildiği aralıkta test verilerinin kaybolmasını kabul etmek yerine yerel biriktirme ve sonradan aktarım seçildi.

## Protokole etkisi

Birikmiş verinin tanımlanması, aktarımın kaldığı yerden sürdürülmesi ve kayıtların ne zaman silinebileceği mesaj sözleşmesinde ele alınmalıdır. Bunların teknik yöntemi bu kararla kesinleştirilmemiştir.

## Açık konular

- Vertex'in depolama ortamı, kapasitesi ve hedeflenen kesinti süresi.
- Depolama dolduğunda testin ve veri kaydının davranışı.
- Güç kesintisinde birikmiş verilerin korunma gereksinimi.
- Test verisi kapsamındaki kayıtlar; olay ve komut sonuçlarının dahil olup olmayacağı.
- Kayıt kimliği, ölçüm zamanı, sıra, teslim onayı, silme ve tekrar aktarım kuralları.
- Bağlantı sonrası birikmiş kayıtlarla canlı ölçümlerin aktarım önceliği.

## Uygulama durumu

Hedef davranış kaydedildi. Firmware'de bu biriktirme mekanizmasının uygulanmış olduğu doğrulanmadı; kod değiştirilmedi.

## İlişkili belgeler

[Bağımsız test yürütme](ADR-0004-autonomous-vertex.md) · [Mimari notlar](../03-software/architecture-notes.md) · [Haberleşme notları](../03-software/communication-notes.md)
