---
baslik: "ADR-0012 — Telemetri zamanını payload oluştururken okuma"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0012 — Telemetri zamanını payload oluştururken okuma

## Bağlam ve karar

Zamanı context’ten kopyalamak yerine payload oluşturulurken doğrudan RTC’den alıyoruz. Dispatcher `measurement_time_ms` alanını doğrudan `TL_RTC_GetMs()` ile doldurur. Context’teki zaman alanı ve güncellemesi kaldırılır.

## Alternatif ve gerekçe

Önceki uygulama zamanı hızlı context ölçümünden sonra saklıyordu. Yeni düzenlemede RTC doğrudan payload oluşturulurken okunuyor. ADR-0011’in zaman edinim seçimi yerine bu karar geçerlidir; sıcaklık ve tel biçimi kararları korunur.

## Sonuçlar

Alan adı `measurement_time_ms` olarak kalır, ancak anlamı payload oluşturma anıdır; sensör edinim anını garanti etmez. uint64_t Unix ms, 17 bayt toplam ve ISO-TP çerçeve sayısı değişmez. İleride tamponlanan bir paket yeniden gönderilirken bu zamanın korunması gerekir; ring buffer henüz uygulanmadı.

## Kaynaklar

2026-09-18 proje notları; firmware `Core/Src/tl_dispatcher.c`, `Core/Src/tl_context.c`, `Core/Inc/tl_context.h`.

[Önceki karar](ADR-0011-telemetry-time-temperature.md) · [Güncel telemetri](../03-software/vertex-telemetry-message.md)
