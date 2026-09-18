---
baslik: "ADR-0014 — Genel duruma pil ve ortam sıcaklığını ekleme"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0014 — Genel duruma pil ve ortam sıcaklığını ekleme

**Sonraki değişiklik:** Ayrı sıcaklık mesajı [ADR-0015](ADR-0015-remove-temperature-message.md) ile kaldırıldı. Aşağıdaki ayrı mesaja ilişkin notlar önceki düzeni anlatıyor; sıcaklıklar yalnız genel durumda kaldı.

Genel durumdan pil ve ortam sıcaklığını da görmek istiyoruz. `VertexStatusPayload` sonuna iki ayrı `int16_t`, °C × 10 alanı ekledik. −32768 yine ölçüm yok demek. Mevcut alanların yerleri aynı; pil sıcaklığı bayt 7–8, ortam sıcaklığı bayt 9–10.

Paket **7 bayttan 11 bayta** çıktı. Tür kodu `0x03`, aralık 3 saniye. Test durmuşken de gönderiliyor. ISO-TP artık **2 veri çerçevesi + 1 Flow Control** kullanıyor; tek çerçeveye sığmıyor.

Ayrı `VertexTemperaturePayload` duruyor: yalnız RUNNING sırasında 10 saniyede bir, zaman alanıyla gönderiliyor. Hızlı telemetriye sıcaklıkları geri eklemiyoruz. Genel durumun kendi zaman alanı yok.

Bu değişiklik [ADR-0010](ADR-0010-message-type-and-operation-mode.md) içindeki 7 bayt/tek çerçeve bölümünün yerine geçiyor. Diğer alanlar ve türle ayrıştırma kuralı aynı. [ADR-0013](ADR-0013-separate-temperature-message.md) içindeki ayrı sıcaklık mesajı korunuyor.

Firmware derlemesi ve bilgisayarda gerçek dispatcher/ISO-TP ile paket kontrolleri geçti. Alıcı `0x03` türünü 11 bayt olarak okumalı; alıcı kodu henüz güncellenmedi. Sensör okumaları ve kart testi henüz tamamlanmadı.

**Kayıt:** 2026-09-18 · Kabul edildi.

[Genel durumun alanları](../03-software/general-status-message.md)
