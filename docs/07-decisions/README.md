---
baslik: "Karar Kayıtları"
kategori: "07-decisions"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Karar Kayıtları

**Son Güncelleme:** 2026-09-18

Her karar ayrı bir `ADR-NNNN-kisa-baslik.md` dosyasında tutulur. Kimlikler tekrar kullanılmaz.

| Kimlik | Tarih | Karar | Durum |
|---|---|---|---|
| [ADR-0001](ADR-0001-documentation.md) | 2026-09-18 | Kararların ve mimari notların kalıcı tutulması | Kabul edildi |
| [ADR-0002](ADR-0002-documentation-scope.md) | 2026-09-18 | Defteri Kebir ana merkez; `__` ile biten klasörler geçersiz | Kabul edildi |
| [ADR-0003](ADR-0003-system-overview.md) | 2026-09-18 | Cluster, Vertex, ClusterPilot ve MQTT/SQLite akışları | Kabul edildi |
| [ADR-0004](ADR-0004-autonomous-vertex.md) | 2026-09-18 | Vertex üzerinde ClusterPilot’tan bağımsız test yürütme | Kabul edildi |
| [ADR-0005](ADR-0005-vertex-data-buffer.md) | 2026-09-18 | Vertex’te test verilerini biriktirme ve sonradan aktarma | Yerine ADR-0006 geçti |
| [ADR-0006](ADR-0006-vertex-ring-buffer.md) | 2026-09-18 | Kısa kesintiler için üzerine yazılan Vertex dairesel tamponu | Kabul edildi |
| [ADR-0007](ADR-0007-measurement-time-sequence.md) | 2026-09-18 | Ölçüm zamanı, sıra numarası ve Linux’tan periyodik RTC eşitleme | Kabul edildi |
| [ADR-0008](ADR-0008-unique-message-length.md) | 2026-09-18 | Farklı CAN mesaj türleri için benzersiz veri uzunluğu | Yerine ADR-0010 geçti |
| [ADR-0009](ADR-0009-tsphere-name.md) | 2026-09-18 | Bulut sunucusunun TSphere olarak adlandırılması | Kabul edildi |
| [ADR-0010](ADR-0010-message-type-and-operation-mode.md) | 2026-09-18 | Tür alanıyla ayrıştırma ve idle/şarj/deşarj çalışma modu | Kabul edildi |
| [ADR-0011](ADR-0011-telemetry-time-temperature.md) | 2026-09-18 | Telemetride ayrı 2 bayt sıcaklıklar ve ölçüm zamanı | Kabul edildi |
| [ADR-0012](ADR-0012-payload-time.md) | 2026-09-18 | Telemetri zamanını payload oluştururken doğrudan RTC’den alma | Kabul edildi |

## Yeni karar şablonu

```markdown
# ADR-NNNN — Karar başlığı

- Tarih:
- Durum: Öneri / Kabul edildi / Reddedildi / Yerine yeni karar geldi
- Kaynak: Görüşme tarihi veya belge/kod bağlantısı

## Bağlam ve problem
## Karar
## Gerekçe
## Değerlendirilen alternatifler
## Etkiler ve ödünleşimler
## Uygulama durumu ve doğrulama
## Açık konular
## İlişkili belgeler ve kararlar
```

Dosyanın başına diğer belgelerde kullanılan YAML ön bilgisini ve son güncelleme tarihini ekle. Kararın kabul edilmesi, uygulamanın tamamlandığı anlamına gelmez.

