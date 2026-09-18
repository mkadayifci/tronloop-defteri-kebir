---
baslik: "ADR-0011 — Telemetride onda bir sıcaklık ve ölçüm zamanı"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0011 — Telemetride onda bir sıcaklık ve ölçüm zamanı

**Sonraki değişiklik:** [ADR-0013](ADR-0013-separate-temperature-message.md) ile sıcaklıklar ayrı mesaja taşındı; telemetri 13 bayta indi. Aşağıdaki 17 bayt bilgisi önceki düzeni anlatıyor. Zamanı RTC’den alma ve sıcaklığı °C × 10 kodlama yöntemi aynı.

Telemetride pil sıcaklığını ve ortam sıcaklığını ayrı tutuyoruz. İkisi de **`int16_t`, °C × 10**: örneğin 253, 25,3 °C demek. Önceki tek bayt/tam derece yaklaşımını bıraktık. Ölçüm yoksa `INT16_MIN` (−32768) gönderiyoruz.

Zaman için **`uint64_t` Unix milisaniye** kullanıyoruz. Böylece paket toplam **17 bayt** oluyor. Tek CAN çerçevesine sığdırmaya çalışmıyoruz; ISO-TP veriyi 3 çerçeveye bölüyor. Flow Control bunun dışında.

İki veri çerçevesinde kalmak için 13 bayta inmemiz gerekiyordu. Zamanı küçültmeyi ve test başlangıcına göre 32 bit fark göndermeyi değerlendirdik, ama 8 baytlık mutlak zamanı koruduk.

## Kodda durum

Zaman ve sıcaklık alanları firmware’e eklendi. İlk uygulama zamanı context ölçümünden sonra saklıyordu; [ADR-0012](ADR-0012-payload-time.md) ile payload oluştururken doğrudan RTC okumaya geçtik. RTC adımı yaklaşık 3,9 ms; 1 ms doğruluk iddiası yok.

Sıcaklık sensörleri henüz okunmuyor. Sıra numarası, ring buffer, Linux’tan periyodik eşitleme göndericisi ve alıcı uyarlaması bu düzenlemeye dahil değildi. Debug derlemesi, bilgisayardaki ISO-TP paket kontrolleri ve RTC yardımcı testleri geçti. Kart testi yapılmadı.

**Kayıt:** 2026-09-18 · Kabul edildi.

[Telemetri ayrıntıları](../03-software/vertex-telemetry-message.md) · [Zaman ve sıra numarası](ADR-0007-measurement-time-sequence.md)
