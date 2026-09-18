---
baslik: "CLAUDE.md — AI Asistan Konfigürasyonu"
kategori: "01-project-general"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# CLAUDE.md — AI Asistan Konfigürasyonu

> Bu dosya, Claude'un bu projeyi nasıl anlayacağını ve dokümantasyonu nasıl sürdüreceğini tanımlar.
> Ekip üyeleri bu dosyayı güncel tutmalıdır.

---

## Proje Kimliği

| Alan | Bilgi |
|------|-------|
| **Proje Adı** | Batarya Kapasite Kayıp İzleme Sistemi |
| **Tür** | Uzun vadeli donanım + araştırma projesi |
| **Kurum** | Biruni Üniversitesi — Elektrik Elektronik Mühendisliği |
| **Başlangıç Tarihi** | 2026 |
| **Durum** | 🟡 Aktif — Kurulum Aşaması |

---

## Projenin Özeti

ClusterPilot ile dağıtık Vertex’ler arasında bataryaları **sürekli şarj/deşarj** döngülerine tabi tutarak kapasite kayıplarını izleyen, **iklim koşullarını** (sıcaklık, nem vb.) paralel olarak ölçen ve uzun vadede **akademik yayınlar** üretmeyi hedefleyen bir donanım araştırma sistemi.

**Adlandırma:** [Cluster, ClusterPilot ve Vertex](docs/01-project-general/terminology.md). Ortak fiziksel altyapı Cluster’a, sunucu rolü ClusterPilot’a aittir.

**Temel bileşenler:**
- Cluster (Vertex birimleri ve ortak test altyapısı)
- ClusterPilot (koordinasyon, veri toplama, iletişim)
- Vertex’ler (şarj/deşarj devresi + iklim sensörleri)
- Veri analiz yazılımı (kapasite hesabı, trend analizi)
- Yayın çıktıları (akademik makaleler, raporlar)

---

## Dokümantasyon Yapısı

```
/
├── README.md                        # Projeye giriş, hızlı erişim
├── CLAUDE.md                        # Bu dosya — AI konfig
├── CHANGELOG.md                     # Versiyon ve değişiklik geçmişi
└── docs/
    ├── 01-project-general/
    │   ├── project-definition.md    # Amaç, kapsam, motivasyon
    │   ├── team.md                  # Ekip üyeleri ve roller
    │   └── roadmap.md               # Milestone'lar, zaman çizelgesi
    ├── 02-hardware/
    │   ├── main-unit.md             # ClusterPilot tasarımı ve şeması
    │   ├── node-design.md           # Vertex devre tasarımı
    │   ├── climate-monitoring.md    # İklim kontrol/izleme sistemi
    │   └── bill-of-materials.md     # BOM (Bill of Materials)
    ├── 03-software/
    │   ├── architecture.md          # Yazılım mimarisi
    │   ├── data-collection.md       # Veri toplama protokolleri
    │   └── analysis.md              # Kapasite analiz algoritmaları
    ├── 04-tests/
    │   ├── test-protocol.md         # Test standartları ve prosedürler
    │   └── results/                 # Test sonuçları (YYYY-MM-DD_test-name.md)
    ├── 05-research/
    │   ├── literature.md            # İlgili yayınlar ve notlar
    │   └── publications.md          # Planlanan / tamamlanan yayınlar
    └── 06-meetings/
        ├── meeting-schedule.md      # Toplantı planı
        └── notes/                   # YYYY-MM-DD_meeting.md formatı
```

---

## Claude İçin Talimatlar

Bu projeye yardım ederken Claude şunları yapmalıdır:

### Belge Güncellemeleri
- Yeni içerik eklerken her dosyanın başındaki `**Son Güncelleme:**` alanını güncelle
- Donanım değişikliklerini `02-hardware/` altında ilgili dosyaya yansıt
- Test sonuçlarını `04-tests/results/` altına `YYYY-MM-DD_test-name.md` formatında kaydet
- `CHANGELOG.md` dosyasını her anlamlı değişiklikte güncelle

### Dil ve Format
- Tüm dokümantasyon **Türkçe** yazılır
- Teknik terimler Türkçe kullanılır, zorunlu hallerde parantez içinde İngilizce karşılığı verilir
- Tablolar, kod blokları ve başlıklar için standart Markdown kullanılır
- Her dosya **YAML front matter** ile başlamalıdır (aşağıya bakınız)

### Dosya Şablonu (Front Matter)
```yaml
---
baslik: "Dosya Başlığı"
kategori: "01-project-general | 02-hardware | 03-software | 04-tests | 05-research | 06-meetings"
durum: "taslak | gözden-geçiriliyor | onaylandı"
son_guncelleme: "YYYY-MM-DD"
guncelleyen: "İsim"
---
```

### İçerik Tutarlılığı
- Bileşen isimleri her dosyada aynı şekilde yazılmalı
- Ölçüm birimleri standart SI birimleri olmalı (V, A, mAh, °C, % vb.)
- Şarj/deşarj protokolleri `04-tests/test-protocol.md` ile tutarlı olmalı

---

## Ekip Bilgileri

_(Ekip üyeleri bu tabloyu dolduracak)_

| İsim | Rol | Sorumluluk Alanı | İletişim |
|------|-----|------------------|----------|
| — | Donanım Tasarımcısı | ClusterPilot + Vertex devreleri | — |
| — | Yazılım Geliştirici | Veri toplama + analiz | — |
| — | Araştırma / Yayın | Literatür + makale yazımı | — |
| — | Proje Koordinatörü | Genel yönetim + test protokolleri | — |

---

## Güncel Durum Özeti

> _Bu bölümü her hafta güncelleyin._

**Son Güncelleme:** 2026-09-18

**Aktif Faz: Faz 1 — Temel Test Sistemi (İklimlendirmesiz)**

**Faz 1 Bileşenleri:**

| Bileşen | Durum | Not |
|---------|-------|-----|
| Vertex Donanım Tasarımı | ⚪ Başlamadı | BQ25756 + BQ34Z100 + STM32L476, KiCad |
| Vertex PCB Üretimi | ⚪ Başlamadı | İlk prototip 1–3 Vertex |
| Şarj/Deşarj Firmware | ⚪ Başlamadı | STM32 temel döngü |
| CAN Haberleşme | ⚪ Başlamadı | Vertex ↔ BeagleBone |
| BeagleBone Veri Toplama | ⚪ Başlamadı | mdadm RAID1 + loglama |
| İlk Test Protokolü | ⚪ Başlamadı | 1C şarj/deşarj |
| Literatür Taraması | ⚪ Başlamadı | — |

**Faz 2 Bileşenleri (Faz 1 tamamlandıktan sonra):**

| Bileşen | Durum | Not |
|---------|-------|-----|
| Peltier + Soğutma Tasarımı | ⚪ Başlamadı | Kapalı döngü, kondenzasyon tuzağı |
| PTC Trim Isıtıcı | ⚪ Başlamadı | 50W, 24V |
| Manifold + Boru Sistemi | ⚪ Başlamadı | ASA/PETG 3D baskı, Armaflex |
| İklim Firmware | ⚪ Başlamadı | PID valf kontrolü |
| 50 Vertex Ölçekleme | ⚪ Başlamadı | Alt manifold sistemi |

**Durum Simgeleri:** ⚪ Başlamadı → 🟡 Devam Ediyor → 🔵 İncelemede → ✅ Tamamlandı → ❌ Engellendi

---

## Sürüm Politikası

Proje uzun soluklu olduğu için semantik versiyonlama yerine tarih bazlı etiketleme kullanılır:

```
v{YIL}.{AY}.{HAFTA}  →  v2026.02.1
```

Major değişiklikler (yeni donanım versiyonu, yeni test serisi) için `CHANGELOG.md` güncellenir.
