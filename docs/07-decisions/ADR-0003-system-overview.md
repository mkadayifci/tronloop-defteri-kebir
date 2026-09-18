---
baslik: "ADR-0003 — Cluster, Vertex ve ClusterPilot genel yapısı"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0003 — Cluster, Vertex ve ClusterPilot genel yapısı

Tronloop bir pil test sistemi. Pilleri belli senaryolara göre şarj ve deşarj ediyor, verileri bulutta saklıyoruz.

Sisteme Cluster’lar bağlanıyor. Her Cluster’ın içinde **Vertex** dediğimiz test birimleri var. **ClusterPilot** ise Linux üzerinde çalışan sunucu: Vertex’lerden veriyi alıyor, **TSphere** üzerindeki MQTT’ye gönderiyor. Gönderemezse veriyi SQLite’ta biriktirip bağlantı düzelince tekrar gönderiyor.

Komutlar da panelden MQTT’ye geliyor. ClusterPilot komutu doğru Vertex’e iletiyor, cihazın yanıtını aynı yoldan geri gönderiyor. Böylece yerel cihazlarla bulut arasındaki iletişimi tek yerde topluyoruz.

Bu kayıt genel yapıyı anlatıyor. MQTT konuları, teslim onayları, kuyrukta hangi kayıtların tutulacağı ve bulut veritabanı burada seçilmedi. Vertex’in bağımsız çalışması [ADR-0004](ADR-0004-autonomous-vertex.md) ile, CAN/ISO-TP ayrıntıları da [haberleşme notlarında](../03-software/communication-notes.md) ele alınıyor. Bütün sistemi uçtan uca kod üzerinden henüz kontrol etmedik.

**Kayıt:** 2026-09-18 · Kabul edildi.

[Mimari notlar](../03-software/architecture-notes.md)
