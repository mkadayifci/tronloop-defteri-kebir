---
baslik: "ADR-0009 — Bulut sunucusunun adı TSphere"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0009 — Bulut sunucusunun adı TSphere

Bulut sunucusuna **TSphere** diyoruz. MQTT ise orada çalışan haberleşme hizmeti.

ClusterPilot verileri ve cihaz yanıtlarını MQTT üzerinden TSphere’e gönderiyor; panel komutlarını da aynı yoldan alıyor. Bu adlandırmayla akış değişmiyor. DNS adresi, MQTT konusu ya da veritabanı seçmiş olmuyoruz; sunucunun adını netleştiriyoruz.

Belgeler ve diyagramlar buna göre güncellendi. Kod değişikliği yok.

**Kayıt:** 2026-09-18 · Kabul edildi.

[İsimler](../01-project-general/terminology.md) · [Haberleşme notları](../03-software/communication-notes.md)
