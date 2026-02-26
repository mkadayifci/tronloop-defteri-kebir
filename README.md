# Batarya Kapasite Kayıp İzleme Sistemi

> Ana ünite ve node'lardan oluşan, bataryaları sürekli şarj/deşarj döngüsüne alarak iklim koşullarıyla birlikte kapasite kayıplarını uzun vadeli izleyen araştırma platformu.

**Durum:** 🟡 Kurulum Aşaması &nbsp;|&nbsp; **Başlangıç:** 2026 &nbsp;|&nbsp; **Son Güncelleme:** 2026-02-26

---

## Hızlı Erişim

| Alan | Dosya |
|------|-------|
| 📋 Proje Tanımı & Hedefler | [docs/01-project-general/project-definition.md](docs/01-project-general/project-definition.md) |
| 👥 Ekip & Roller | [docs/01-project-general/team.md](docs/01-project-general/team.md) |
| 🗺️ Yol Haritası & Milestone'lar | [docs/01-project-general/roadmap.md](docs/01-project-general/roadmap.md) |
| 🔌 Ana Ünite Tasarımı | [docs/02-hardware/main-unit.md](docs/02-hardware/main-unit.md) |
| 🔋 Node Tasarımı | [docs/02-hardware/node-design.md](docs/02-hardware/node-design.md) |
| 🌡️ İklim İzleme | [docs/02-hardware/climate-monitoring.md](docs/02-hardware/climate-monitoring.md) |
| 📦 Malzeme Listesi (BOM) | [docs/02-hardware/bill-of-materials.md](docs/02-hardware/bill-of-materials.md) |
| 💻 Yazılım Mimarisi | [docs/03-software/architecture.md](docs/03-software/architecture.md) |
| 🧪 Test Protokolü | [docs/04-tests/test-protocol.md](docs/04-tests/test-protocol.md) |
| 📚 Literatür Taraması | [docs/05-research/literature.md](docs/05-research/literature.md) |
| 📰 Yayın Planı | [docs/05-research/publications.md](docs/05-research/publications.md) |
| 🤖 AI Asistan Konfig | [CLAUDE.md](CLAUDE.md) |
| 📝 Değişiklik Geçmişi | [CHANGELOG.md](CHANGELOG.md) |

---

## Sistem Mimarisi (Özet)

```
┌─────────────────────────────────────────────────────────┐
│                      ANA ÜNİTE                          │
│   Koordinasyon · Veri Toplama · İletişim · Depolama     │
└───────────────────────┬─────────────────────────────────┘
                        │
          ┌─────────────┼─────────────┐
          ▼             ▼             ▼
    ┌──────────┐  ┌──────────┐  ┌──────────┐
    │  NODE 1  │  │  NODE 2  │  │  NODE N  │
    │ Şarj/    │  │ Şarj/    │  │ Şarj/    │
    │ Deşarj   │  │ Deşarj   │  │ Deşarj   │
    │ +İklim   │  │ +İklim   │  │ +İklim   │
    └──────────┘  └──────────┘  └──────────┘
          │             │             │
          └─────────────┼─────────────┘
                        ▼
              Kapasite Analizi &
              Akademik Yayınlar
```

---

## Proje Hakkında

Bu proje, bataryaların uzun vadeli kapasite kayıp davranışlarını sistematik olarak ölçmek ve belgelemek amacıyla tasarlanmıştır. Her node bağımsız şarj/deşarj döngüleri yürütürken iklim parametrelerini (sıcaklık, nem, vb.) eş zamanlı kaydeder. Toplanan veriler analiz edilerek akademik yayınlara dönüştürülür.

---

## Katkıda Bulunma

1. İlgili `docs/` alt klasörüne git
2. Değişikliğini yap, front matter'daki `son_guncelleme` ve `guncelleyen` alanlarını güncelle
3. `CHANGELOG.md` dosyasına kısa bir not ekle
4. Commit mesajı formatı: `docs(konu): kısa açıklama`

Detaylı talimatlar için [CLAUDE.md](CLAUDE.md) dosyasına bakınız.
