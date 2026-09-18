---
baslik: "ADR-0002 — Dokümantasyon merkezi ve geçerli kaynak kapsamı"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0002 — Dokümantasyon merkezi ve geçerli kaynak kapsamı

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Kabul edildi
- **Kaynak:** 2026-09-18 tarihli proje notları.

## Bağlam ve karar

`tronloop-defteri-kebir`, konuşulan kararların, haberleşme notlarının ve mimari dokümantasyonun ana merkezidir. Diğer klasörler kod ve tasarım kaynaklarını barındırır. Adı `__` ile biten klasörler geçersizdir; güncel sistemi tanımlamak veya teknik kararları dayandırmak için kullanılmaz.

## Gerekçe ve etkiler

Dokümantasyon tek merkezde yönetilir; geçersiz kodun güncel mimariyle karıştırılması önlenir. Yeni incelemelerde `__` ile biten klasörler kapsam dışında tutulur. Tarihsel karşılaştırma ancak açıkça istendiğinde yapılır. Bu karar klasörlerin silinmesini gerektirmez.

## Alternatifler

Bu görüşmede alternatif tartışılmadı.

## Uygulama durumu

Çalışma alanı talimatları, karar defteri ve mimari kaynak envanteri güncellendi. Kodda değişiklik yapılmadı.

## İlişkili kayıtlar

[ADR-0001](ADR-0001-documentation.md) kararını tamamlar; onu yürürlükten kaldırmaz. Dokümantasyonun ana merkezi bu kayıtla Defteri Kebir olarak belirlendi.

[Mimari çalışma notları](../03-software/architecture-notes.md)
