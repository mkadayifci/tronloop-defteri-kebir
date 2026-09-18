---
baslik: "ADR-0003 — Cluster, Vertex ve ClusterPilot genel yapısı"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0003 — Cluster, Vertex ve ClusterPilot genel yapısı

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Kabul edildi

## Bağlam ve karar

Tronloop, pilleri senaryolara göre şarj/deşarj eder ve verileri bulutta saklar. Sisteme bağlanan Cluster'ların içinde Vertex’ler bulunur. Linux sunucusu ClusterPilot, Vertex verilerini TSphere üzerindeki MQTT'ye gönderir. Gönderemediği verileri yerel SQLite'ta biriktirir ve sonradan gönderir.

Kullanıcı panellerinden başlatılan komutlar MQTT'ye ulaşır. ClusterPilot bunları okuyup doğru cihazlara iletir ve cihaz yanıtlarını yine MQTT'ye gönderir.

## Gerekçe ve sonuçlar

SQLite biriktirme davranışı, buluta gönderim yapılamayan dönemde verilerin daha sonra gönderilebilmesini sağlar. Veri ve komut akışlarında ClusterPilot yönlendirme sorumluluğu taşır. Diğer teknoloji seçimlerinin gerekçeleri ve alternatifleri bu görüşmede açıklanmadı.

## Kesinleşmeyen ayrıntılar

Yerel Vertex protokolü, MQTT konu/mesaj şeması, teslim güvenceleri, kuyruk kapsamı, komut yaşam döngüsü, senaryo yürütme sorumluluğu ve bulut depolama bileşenleri bu kayıtla karara bağlanmaz.

## Uygulama durumu

Bu kayıt hedef mimariyi anlatıyor; kodun tamamı bu yapı açısından henüz incelenmedi.

## İlişkili belgeler

[Mimari notlar](../03-software/architecture-notes.md) · [Haberleşme notları](../03-software/communication-notes.md)

