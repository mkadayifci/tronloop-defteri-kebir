---
baslik: "ADR-0004 — Vertex üzerinde bağımsız test yürütme"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0004 — Vertex üzerinde bağımsız test yürütme

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Kabul edildi
- **Kararı veren:** Kullanıcı
- **Kaynak:** Senaryonun Vertex üzerinde yürütülmesi sorusuna kullanıcının “Evet testi firmware pilota muhtaç olmadan çalıştırabilecek” yanıtı.

## Bağlam ve karar

Test senaryosunu Vertex firmware'i yürütür. Vertex, testi sürdürmek için ClusterPilot'tan sürekli komut veya adım onayı beklemez. Senaryo yürütme sorumluluğu ve adım geçişleri Vertex üzerindedir. ClusterPilot veri aktarımı, komut yönlendirme ve izleme sorumluluklarını sürdürür.

## Gerekçe ve etkiler

Testin yürütülmesi ClusterPilot'un sürekli erişilebilir olmasına bağlı olmayacaktır. ClusterPilot bağlantısının kesilmesi tek başına test yürütmenin durmasını gerektirmez. Bu karar, diğer test durdurma koşullarını veya cihaz korumalarını kaldırmaz.

Protokol tasarımında senaryo aktarımı ve test kontrolü, adım adım yürütmeden ayrılmalıdır. Aktarım biçimi, komut isimleri ve mesaj kodları henüz seçilmedi.

## Alternatifler

ClusterPilot'un her adımı yönettiği, testin sürekli bağlantı gerektirdiği yaklaşım yerine Vertex üzerinde bağımsız yürütme seçildi.

## Uygulama durumu

Bu kayıt hedef davranış kararıdır. Mevcut firmware'in bütün senaryo türlerini veya kesinti durumlarını eksiksiz desteklediği doğrulanmış değildir. Kod değiştirilmedi.

## Açık konular

- Senaryo aktarımı, doğrulama ve başlatma sözleşmesi.
- Vertex kısa kesintiler için dairesel tampon kullanır; dolunca en eski kayıtların üzerine yazar. Geçerli karar [ADR-0006](ADR-0006-vertex-ring-buffer.md); kapasite, bellek ortamı ve olay kapsamı açık.
- Bağlantı geri geldiğinde test durumunun eşleştirilmesi ve birikmiş verinin aktarım sözleşmesi.
- Vertex yeniden başladığında veya güç kesildiğinde senaryonun ve ilerlemenin korunması; yeniden başlatma/devam politikası. Bağımsız yürütme kararı bunları otomatik olarak belirlemez.

## İlişkili belgeler

[Genel yapı kararı](ADR-0003-system-overview.md) · [Mimari notlar](../03-software/architecture-notes.md) · [Haberleşme notları](../03-software/communication-notes.md)
