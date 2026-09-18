---
baslik: "ADR-0009 — Bulut sunucusunun adı TSphere"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0009 — Bulut sunucusunun adı TSphere

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Kabul edildi
- **Kararı veren / kaynak:** Kullanıcının bu görüşmedeki sunucuyu TSphere olarak adlandırma talebi.

## Karar ve kapsam

Bulut sunucusu dokümantasyonda **TSphere** adıyla anılacak. MQTT bir sunucu adı değil, TSphere üzerindeki haberleşme hizmeti/protokolü olarak belirtilecek. ClusterPilot, Vertex verilerini ve yanıtlarını TSphere'e MQTT üzerinden iletir; panel kaynaklı komutları aynı yol üzerinden alır.

Bu adlandırma mevcut MQTT akışlarını değiştirmez; yeni bir sunucu kurulumu, DNS adresi, MQTT konu adı veya bulut depolama teknolojisi seçimi değildir.

## Uygulama durumu

Belge metinleri, diyagramlar ve kalıcı adlandırma kuralları güncellendi. Kod değiştirilmedi.

[Adlandırma rehberi](../01-project-general/terminology.md) · [Mesajlaşma protokolü](../03-software/communication-notes.md)
