---
baslik: "ADR-0007 — Ölçüm zamanı, sıra numarası ve saat eşitleme"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0007 — Ölçüm zamanı, sıra numarası ve saat eşitleme

Gecikmeli gelen bir kaydın ne zamana ait olduğunu ve arada kayıt atlanıp atlanmadığını anlayabilmek için **zaman ve sıra numarası** kullanıyoruz. Zaman milisaniye cinsinden olacak. Bir kayıt tamponda beklediyse, tekrar gönderirken zamanını değiştirmeyeceğiz.

İlk karar zamanı ölçüm anında almaktı. Sonrasında [ADR-0012](ADR-0012-payload-time.md) ile telemetride RTC’yi payload oluştururken okumaya geçtik. Yani bugünkü `measurement_time_ms`, sensörün okunduğu anı değil paketin oluşturulduğu anı gösteriyor.

Sıra numarası sadece kayıtları ayırt etmek için. Artmaya devam edecek; test değişince sıfırlamak zorunda değiliz. Tek başına bütün cihazlarda ve her yeniden başlatmada benzersiz bir kimlik gibi düşünmüyoruz.

## Saat nereden geliyor?

STM32’de RTC çalışıyor. ClusterPilot, Linux/Unix zamanını belli aralıklarla mesajla gönderecek; Vertex de saatini güncelleyecek. Bağlantı kesilince kendi saatiyle devam edecek.

İlk incelemede `TL_RTC_Set(uint32_t unix_ts)` ve `TL_RTC_Get()` yalnızca Unix saniyesiyle çalışıyordu. [ADR-0011](ADR-0011-telemetry-time-temperature.md) ile `uint64_t` Unix ms alanı ve `TL_RTC_GetMs()` eklendi. Bu fonksiyon RTC’nin saniye altı bilgisini kullanıyor. Mevcut ayarda adım yaklaşık **3,9 ms**; milisaniye yazıyor olmamız 1 ms doğruluk sağladığımız anlamına gelmiyor.

## Kodda şu an ne var?

- `Core/Inc/tl_rtc.h` ve `Core/Src/tl_rtc.c`: saniye ve milisaniye okuma, saniye cinsinden saat ayarlama.
- `Core/Src/tl_can.c`: ham CAN üzerinde `0x01` komutu ve dört bayt little-endian zaman değeriyle RTC ayarlama.
- `Core/Src/main.c`: açılışta sabit `1710255720UL` zamanı atanıyor. Linux’tan ilk eşitleme gelmeden bu tarih güncel değil.
- Telemetride zaman var; ölçüm sıra numarası henüz yok. Linux’un periyodik eşitleme göndericisini henüz doğrulamadık.

Dosyalar `tronloop-vertex-firmware/` altında. İlk inceleme 2026-09-18’de yapıldı; o incelemede kod değişmemişti. Daha sonraki zaman düzenlemeleri ADR-0011 ve ADR-0012’de kayıtlı. Kart üzerinde zaman doğrulaması yapılmadı.

## Kalan sorular

Sayacın kaç bayt olacağını, taşınca ve cihaz yeniden başlayınca ne yapacağını belirleyeceğiz. Saatin ne sıklıkla eşitleneceği, ilk eşitlemeden önce kayıtların nasıl işaretleneceği ve saat ileri/geri alındığında verinin nasıl yorumlanacağı da açık. Eski ham CAN saat komutunu yeni protokolde aynen kullanmaya henüz karar vermedik.

**Kayıt:** 2026-09-18 · Kabul edildi; telemetride zamanın okunduğu an için ADR-0012 geçerli.

[Dairesel tampon](ADR-0006-vertex-ring-buffer.md) · [Güncel telemetri](../03-software/vertex-telemetry-message.md) · [Haberleşme notları](../03-software/communication-notes.md)
