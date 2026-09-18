---
baslik: "Batarya Kapasite Kayıp İzleme Sistemi"
kategori: "01-project-general"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

![TRONLOOP](docs/01-project-general/assets/logo-small.png)

# Batarya Kapasite Kayıp İzleme Sistemi

> Cluster’lar içindeki Vertex birimleriyle pilleri senaryolara göre test eden; ClusterPilot üzerinden test verilerini buluta taşıyan araştırma platformu.

**Durum:** 🟡 Faz 1 — Kurulum Aşaması &nbsp;|&nbsp; **Başlangıç:** 2026 &nbsp;|&nbsp; **Son Güncelleme:** 2026-09-18

| | Faz 1 | Faz 2 |
|--|-------|-------|
| **Kapsam** | Temel şarj/deşarj + kapasite ölçümü | + İklim kontrol entegrasyonu |
| **İklimlendirme** | ❌ Yok — oda sıcaklığı | ✅ -20°C – +40°C kontrollü |
| **Durum** | 🟡 Aktif | ⚪ Faz 1 sonrası başlar |

---

## Hızlı Erişim

- [Karar ve Mimari Defteri](docs/01-project-general/decision-hub.md) — kararlar, haberleşme notları ve açık konular

- [Proje Tanımı & Hedefler](docs/01-project-general/project-definition.md)
- [Ekip & Roller](docs/01-project-general/team.md)
- [Yol Haritası & Milestone'lar](docs/01-project-general/roadmap.md)
- [ClusterPilot Tasarımı](docs/02-hardware/main-unit.md)
- [Vertex Tasarımı](docs/02-hardware/node-design.md)
- [İklim İzleme](docs/02-hardware/climate-monitoring.md)
- [Malzeme Listesi (BOM)](docs/02-hardware/bill-of-materials.md)
- [Yazılım Mimarisi](docs/03-software/architecture.md)
- [Mesajlaşma Protokolü — Çalışma Taslağı](docs/03-software/communication-notes.md)
- [Telemetri Mesajı — Sıcaklıklar ve Ölçüm Zamanı](docs/03-software/vertex-telemetry-message.md)
- [Genel Durum Mesajı — Alanlar ve Bayt Yerleşimi](docs/03-software/general-status-message.md)
- [Test Protokolü](docs/04-tests/test-protocol.md)
- [Literatür Taraması](docs/05-research/literature.md)
- [Yayın Planı](docs/05-research/publications.md)
- [AI Asistan Konfig](CLAUDE.md)
- [Değişiklik Geçmişi](CHANGELOG.md)

---

## Sistem Mimarisi (Özet)

```mermaid
flowchart LR
    subgraph CLUSTER["Cluster"]
        CP["ClusterPilot<br/>Linux · Veri Toplama · Komut Yönlendirme"]
        V1["Vertex 1<br/>Bağımsız Test Yürütme"]
        VN["Vertex N<br/>Bağımsız Test Yürütme"]
        CP <-->|CAN/ISO-TP| V1
        CP <-->|CAN/ISO-TP| VN
    end
    CP <-->|Veri · Komut · Yanıt| MQTT["TSphere<br/>MQTT"]
```

| Ad | Anlamı |
|---|---|
| **Cluster** | Vertex birimlerini ve bunların ortak altyapısını içeren test grubu |
| **ClusterPilot** | Vertex’lerle bulut arasındaki iletişimi yöneten Linux sunucusu |
| **Vertex** | Pili test eden, senaryoyu kendi firmware’i üzerinde bağımsız yürüten birim |
| **TSphere** | MQTT haberleşme hizmetini barındıran bulut sunucusu |

Ayrıntılar: [Bileşen adlandırması](docs/01-project-general/terminology.md) · [Güncel mimari notları](docs/03-software/architecture-notes.md).


---

## Proje Hakkında

Bu proje, bataryaların uzun vadeli kapasite kayıp davranışlarını sistematik olarak ölçmek ve belgelemek amacıyla tasarlanmıştır. Her Vertex bağımsız şarj/deşarj döngüleri yürütürken iklim parametrelerini (sıcaklık, nem, vb.) eş zamanlı kaydeder. Toplanan veriler analiz edilerek akademik yayınlara dönüştürülür.

---

## Katkıda Bulunma

1. İlgili `docs/` alt klasörüne git
2. Değişikliğini yap, front matter'daki `son_guncelleme` ve `guncelleyen` alanlarını güncelle
3. `CHANGELOG.md` dosyasına kısa bir not ekle
4. Commit mesajı formatı: `docs(konu): kısa açıklama`

Detaylı talimatlar için [CLAUDE.md](CLAUDE.md) dosyasına bakınız.
