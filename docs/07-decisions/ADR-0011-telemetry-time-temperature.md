---
baslik: "ADR-0011 — Telemetride onda bir sıcaklık ve ölçüm zamanı"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0011 — Telemetride onda bir sıcaklık ve ölçüm zamanı

**Son Güncelleme:** 2026-09-18

- **Durum:** Kabul edildi

## Karar

`VertexTelemetryPayload` içindeki pil ve ortam sıcaklıkları ayrı `int16_t` alanlarıdır; birim 0,1 °C, örneğin 253 = 25,3 °C. Mesajda ölçüm zamanı bulunur. Önceki 1 bayt/tam derece sıcaklık tasarımı bu düzenlemeyle değiştirildi. Tek CAN çerçevesi telemetri için zorunlu değildir; ISO-TP çok çerçeveli gönderim kullanılır.

## Uygulama seçimleri ve sonuç

Önceki milisaniye zaman kararı doğrultusunda `uint64_t` Unix milisaniye seçildi; toplam paket **17 bayt**. İlk uygulamada zaman hızlı context ölçümünden sonra kaydediliyordu; bu seçim [ADR-0012](ADR-0012-payload-time.md) ile değiştirilmiştir: zaman payload oluşturulurken RTC’den alınır. Sıcaklık için `INT16_MIN` geçersiz işaretidir; sensör okumaları henüz yoktur. Mevcut RTC prescaleriyle zaman adımı yaklaşık 3,9 ms'dir; 1 ms doğruluk iddiası yoktur.

Zaman ve sıcaklık alanları firmware'e uygulandı. Sıra numarası, dairesel tampon, Linux periyodik saat eşitleme göndericisi ve alıcı uyarlaması bu değişiklikte uygulanmadı. Debug derlemesi, bilgisayarda ISO-TP paket testleri ve RTC yardımcı testleri geçti; kart testi yapılmadı.

[Telemetri ayrıntıları](../03-software/vertex-telemetry-message.md) · [Ölçüm zamanı kararı](ADR-0007-measurement-time-sequence.md)

## Zaman alanının korunması

İki ISO-TP veri çerçevesine inmek için 17 bayttan 13 bayta düşme ve zaman alanını küçültme seçenekleri görüşüldü. Mevcut biçim korunuyor: **uint64_t Unix milisaniye, 8 bayt zaman alanı ve toplam 17 bayt telemetri**. Test başlangıcına göre 32 bit zaman farkı seçeneği benimsenmedi. Mesaj 3 veri çerçevesiyle taşınır; alıcının Flow Control trafiği ayrıca vardır. Kod değişikliği gerekmedi.
