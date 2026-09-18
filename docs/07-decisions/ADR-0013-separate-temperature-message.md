---
baslik: "ADR-0013 — Sıcaklıkları ayrı mesajla gönderme"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0013 — Sıcaklıkları ayrı mesajla gönderme

**Sonraki değişiklik:** Ayrı sıcaklık mesajı [ADR-0015](ADR-0015-remove-temperature-message.md) ile kaldırıldı. Aşağıdaki ayrı mesaja ilişkin notlar önceki düzeni anlatıyor; sıcaklıklar yalnız genel durumda kaldı.

Sıcaklıkları `VertexTelemetryPayload` içinden çıkarıyoruz. Pil ve ortam sıcaklığını ayrı bir mesajla **10 saniyede bir** göndereceğiz. Gerilim ve akım hızlı telemetride kalıyor.

Böylece yavaş gönderdiğimiz sıcaklıkları her 100 ms’de yeniden taşımıyoruz. İki sıcaklık da `int16_t`, °C × 10 olarak kalıyor; −32768 ölçüm yok demek.

Uygulamada yeni mesaja `VertexTemperaturePayload`, tür koduna `0x04` adını verdik. Ayrı sıcaklık kaydında zamanı korumak için `uint64_t` Unix ms alanını da tuttuk. İki mesajın boyutu **13 bayt**; birbirinden tür alanıyla ayrılıyorlar. Telemetrideki zaman bayt 9 yerine artık bayt 5’ten başlıyor. Her mesaj 2 ISO-TP veri çerçevesi ve 1 Flow Control gerektiriyor.

Telemetrideki mevcut RUNNING koşulu korunuyor. Sıcaklık mesajı da yalnızca RUNNING durumunda gidiyor; bağlantı meşgulse boşalmasını bekliyor. Bunlar bu düzenlemenin kod davranışı. Sensör okuma kodu henüz hazır değil.

Bu karar, [ADR-0011](ADR-0011-telemetry-time-temperature.md) içindeki sıcaklıkları telemetride taşıma ve 17 bayt paket düzeninin yerine geçiyor. Sıcaklığın birimi ve zamanın 64 bit olması aynı. [ADR-0012](ADR-0012-payload-time.md) içindeki payload oluştururken RTC okuma yöntemi iki mesajda da geçerli.

Firmware derlemesi ve bilgisayardaki paket/zamanlama kontrolleri geçti. Alıcı yazılım değiştirilmedi; `0x01` için 13 bayt, `0x04` için ayrı 13 bayt şemaya geçirilmesi gerekiyor. Aynı tür koduyla boyut değiştiğinden eski 17 bayt alıcıyla uyumlu değil. Kart testi yapılmadı.

**Kayıt:** 2026-09-18 · Kabul edildi.

[Telemetri](../03-software/vertex-telemetry-message.md) · [Sıcaklık mesajı](../03-software/vertex-temperature-message.md)
