---
baslik: "ADR-0004 — Vertex üzerinde bağımsız test yürütme"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0004 — Vertex üzerinde bağımsız test yürütme

**Vertex, testi ClusterPilot’a muhtaç olmadan çalıştıracak.** Senaryo adımlarını ve geçişlerini kendi firmware’i yönetecek. Her adımda sunucudan yeni komut ya da onay beklemeyecek.

ClusterPilot’un işi veriyi toplamak, komutları yönlendirmek ve sistemi izlemek. Bağlantı kesildi diye testin de durmasını istemiyoruz. Tabii cihazın korumaları ve senaryonun kendi durdurma koşulları yine geçerli.

Bu yüzden senaryoyu cihaza aktarmakla senaryoyu adım adım çalıştırmayı ayrı düşünüyoruz. Aktarım, doğrulama ve başlatma mesajlarını daha tasarlayacağız. Firmware’de bütün senaryo türlerini ve kesinti durumlarını test etmiş değiliz; bu kayıt hedeflediğimiz davranışı anlatıyor.

## Kalan işler

- Senaryoyu nasıl aktaracağımız, doğrulayacağımız ve başlatacağımız.
- Bağlantı gelince test durumunun nasıl eşitleneceği ve bekleyen verinin nasıl aktarılacağı.
- Güç kesilince ya da Vertex yeniden başlayınca senaryo ve ilerlemenin korunup korunmayacağı.
- Kısa kesintiler için seçtiğimiz [dairesel tamponun](ADR-0006-vertex-ring-buffer.md) kapasitesi, belleği ve hangi kayıtları tutacağı.

**Kayıt:** 2026-09-18 · Kabul edildi. Bu karar alınırken kod değişmedi.

[Genel yapı](ADR-0003-system-overview.md) · [Mimari notlar](../03-software/architecture-notes.md) · [Haberleşme notları](../03-software/communication-notes.md)
