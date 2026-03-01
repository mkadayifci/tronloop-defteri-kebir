# Yol Haritası

Proje iki ana faza ayrılmıştır. İlk faz iklimlendirme olmadan temel test altyapısını kurar ve doğrular; ikinci faz iklim kontrol sistemini entegre ederek çok değişkenli test senaryolarını etkinleştirir.

---

## Faz 1 — Temel Test Sistemi (İklimlendirmesiz)

> **Hedef:** Şarj/deşarj döngüsü, kapasite ölçümü ve CAN haberleşmesinin oda sıcaklığında kararlı çalıştığını doğrulamak.

| Milestone | Durum | Açıklama |
|-----------|-------|----------|
| Node donanım tasarımı | ⚪ Başlamadı | BQ25756 + BQ34Z100 + STM32L476, KiCad şematik |
| Node PCB üretimi | ⚪ Başlamadı | İlk prototip, 1–3 node |
| Şarj/deşarj firmware | ⚪ Başlamadı | STM32 üzerinde temel döngü kontrolü |
| CAN haberleşme | ⚪ Başlamadı | Node ↔ BeagleBone protokolü |
| BeagleBone veri toplama | ⚪ Başlamadı | mdadm RAID1, veri loglama |
| İlk test protokolü | ⚪ Başlamadı | 1C şarj/deşarj, kapasite ölçümü |
| Faz 1 doğrulama | ⚪ Başlamadı | 3 node ile kararlı uzun süreli çalışma |

---

## Faz 2 — İklim Kontrol Entegrasyonu

> **Hedef:** Faz 1 sistemi üzerine iklimlendirme katmanı ekleyerek sıcaklık kontrollü test senaryolarını çalıştırmak.

| Milestone | Durum | Açıklama |
|-----------|-------|----------|
| Peltier + soğutma tasarımı | ⚪ Başlamadı | Kapalı döngü, kondenzasyon tuzağı, hot side ısı eşanjörü |
| PTC trim ısıtıcı entegrasyonu | ⚪ Başlamadı | 50W, 24V, PWM kontrol |
| Manifold + boru sistemi | ⚪ Başlamadı | ASA/PETG 3D baskı, Armaflex yalıtım |
| Nem ve sıcaklık kontrolü | ⚪ Başlamadı | TMP117 + PID + valf kontrolü (-20°C – +40°C) |
| İklim firmware | ⚪ Başlamadı | Kapalı döngü PID, basınç tahliyesi |
| 50 node ölçekleme | ⚪ Başlamadı | Alt manifold sistemi, tam kapasite test |
| Faz 2 doğrulama | ⚪ Başlamadı | Çok sıcaklıklı eş zamanlı senaryo testi |

---

**Durum Simgeleri:** ⚪ Başlamadı → 🟡 Devam Ediyor → 🔵 İncelemede → ✅ Tamamlandı → ❌ Engellendi

---

**İlgili Dosyalar:** [Proje Tanımı](project-definition.md) · [Yayınlar](../05-research/publications.md)
