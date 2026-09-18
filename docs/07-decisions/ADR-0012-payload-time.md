---
baslik: "ADR-0012 — Telemetri zamanını payload oluştururken okuma"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0012 — Telemetri zamanını payload oluştururken okuma

**Sonraki değişiklik:** [ADR-0013](ADR-0013-separate-temperature-message.md) ile sıcaklıklar ayrı mesaja taşındı; telemetri 13 bayta indi. Aşağıdaki 17 bayt bilgisi önceki düzeni anlatıyor. Zamanı RTC’den alma ve sıcaklığı °C × 10 kodlama yöntemi aynı.

Zamanı payload oluştururken doğrudan RTC’den alıyoruz:

```c
.measurement_time_ms = TL_RTC_GetMs()
```

Önce hızlı context güncellemesinde zamanı saklayıp pakete kopyalıyorduk. Artık o ara alana gerek yok; context’teki zaman alanını ve güncellemesini kaldırdık.

`measurement_time_ms` adını koruyoruz, ama bu değer **paketin oluşturulduğu an**. Sensörün okunduğu anla aynı olmak zorunda değil. Paket yine `uint64_t` Unix ms kullanıyor ve toplam 17 bayt; çerçeve sayısı değişmedi.

İleride ring buffer eklediğimizde bekleyen paketi yeniden gönderirken bu zamanı korumamız gerekiyor. Ring buffer henüz yazılmadı.

**Kayıt:** 2026-09-18 · Kabul edildi. [ADR-0011](ADR-0011-telemetry-time-temperature.md) içindeki zaman okuma yöntemi yerine geçiyor; sıcaklıklar ve paket biçimi aynı.

Kod: `tronloop-vertex-firmware/Core/Src/tl_dispatcher.c`, `Core/Src/tl_context.c`, `Core/Inc/tl_context.h`. Debug derlemesi geçti; kart testi yapılmadı.

[Güncel telemetri](../03-software/vertex-telemetry-message.md)
