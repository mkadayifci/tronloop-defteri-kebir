---
baslik: "ADR-0005 — Vertex üzerinde test verilerini biriktirme"
kategori: "07-decisions"
durum: "yerine-yeni-karar-geldi"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0005 — Vertex üzerinde test verilerini biriktirme

**Eski karar. Güncel davranış [ADR-0006](ADR-0006-vertex-ring-buffer.md) içinde.** İlk fikir veriyi bağlantı gelene kadar biriktirmekti. Sonrasında amacın kayıpsız saklama değil, kısa kesintileri karşılamak olduğunu netleştirdik: tampon dolunca eski kaydın üzerine yazılacak.

İlk tasarımda Vertex, ClusterPilot’a ulaşamadığında testi sürdürüyor ve veriyi kendi üzerinde biriktiriyordu. Bağlantı düzelince bu veriler ClusterPilot’a aktarılacaktı. Bu, bulut bağlantısı kesildiğinde ClusterPilot’un SQLite’a yazmasından ayrı bir katman.

| Kesinti | Veriyi tutan | Sonraki hedef |
|---|---|---|
| Vertex → ClusterPilot | Vertex; bellek türü o sırada seçilmemişti | ClusterPilot |
| ClusterPilot → TSphere | ClusterPilot, SQLite | TSphere üzerindeki MQTT |

Bu aşamada kapasite, dolunca ne olacağı, güç kesintisinde verinin korunması ve olay/komut sonuçlarının da tutulup tutulmayacağı açık kaldı. Kayıt kimliği, zaman, sıra, teslim onayı, silme, yeniden gönderme ve canlı veriye öncelik verme konularını da henüz belirlememiştik. Sonraki kararları okurken bu listeyi güncel bir yapılacaklar listesi gibi almamak gerekiyor.

**Kayıt:** 2026-09-18 · Yerine ADR-0006 geçti. Bu aşamada firmware’e tampon eklenmedi.

[Bağımsız test yürütme](ADR-0004-autonomous-vertex.md) · [Haberleşme notları](../03-software/communication-notes.md)
