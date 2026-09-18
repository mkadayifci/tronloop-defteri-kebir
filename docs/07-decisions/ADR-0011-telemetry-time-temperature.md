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
- **Kararı veren / kaynak:** Kullanıcının pil ve ortam sıcaklıklarını ayrı 2 bayt, ×10 ölçekli yapma ve zaman alanı ekleme talebi.

## Karar

`VertexTelemetryPayload` içindeki pil ve ortam sıcaklıkları ayrı `int16_t` alanlarıdır; birim 0,1 °C, örneğin 253 = 25,3 °C. Mesajda ölçüm zamanı bulunur. Önceki 1 bayt/tam derece sıcaklık tasarımı bu taleple değiştirilmiştir. Tek CAN çerçevesi telemetri için zorunlu değildir; ISO-TP çok çerçeveli gönderim kullanılır.

## Uygulama seçimleri ve sonuç

Önceki milisaniye zaman kararı doğrultusunda `uint64_t` Unix milisaniye seçildi; toplam paket **17 bayt**. Zaman hızlı context ölçümünden sonra kaydedilir. Sıcaklık için `INT16_MIN` geçersiz işaretidir; sensör okumaları henüz yoktur. Mevcut RTC prescaleriyle zaman adımı yaklaşık 3,9 ms'dir; 1 ms doğruluk iddiası yoktur.

Zaman ve sıcaklık alanları firmware'e uygulandı. Sıra numarası, dairesel tampon, Linux periyodik saat eşitleme göndericisi ve alıcı uyarlaması bu değişiklikte uygulanmadı. Debug derlemesi, bilgisayarda ISO-TP paket testleri ve RTC yardımcı testleri geçti; kart testi yapılmadı.

[Telemetri ayrıntıları](../03-software/vertex-telemetry-message.md) · [Ölçüm zamanı kararı](ADR-0007-measurement-time-sequence.md)

## Zaman alanının korunması — kullanıcı onayı

İki ISO-TP veri çerçevesine inmek için 17 bayttan 13 bayta düşme ve zaman alanını küçültme seçenekleri görüşüldü. Kullanıcı mevcut biçimin korunmasını seçti: **uint64_t Unix milisaniye, 8 bayt zaman alanı ve toplam 17 bayt telemetri**. Test başlangıcına göre 32 bit zaman farkı seçeneği benimsenmedi. Mesaj 3 veri çerçevesiyle taşınır; alıcının Flow Control trafiği ayrıca vardır. Kod değişikliği gerekmedi.
