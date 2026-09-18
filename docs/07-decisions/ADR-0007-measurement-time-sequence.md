---
baslik: "ADR-0007 — Ölçüm zamanı, sıra numarası ve saat eşitleme"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0007 — Ölçüm zamanı, sıra numarası ve saat eşitleme

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Kabul edildi
- **Kaynak:** 2026-09-18 tarihli proje notları.

## Karar

Test ölçüm kayıtları **milisaniye çözünürlüğünde ölçüm zamanını** ve sıra numarasını içerecek. Zaman ölçümün alındığı anı ifade eder; tampon boşaltılırken gönderim zamanı ile değiştirilmez. Bu alanlar gecikmeli kayıtların zamanını ve aradaki kayıt boşluklarını ayırt etmek için kullanılır.

STM32 üzerinde RTC çalışır. ClusterPilot Linux sistem zamanı, periyodik saat güncelleme mesajlarıyla Vertex'e iletilir ve Vertex saati güncellenir. Unix zaman temeli kullanılır. Bağlantı kesilince Vertex kendi saatiyle çalışmayı sürdürür.

## Kaynaktan doğrulanan mevcut uygulama

2026-09-18 tarihinde geçerli Vertex çalışma ağacı incelendi:

- `Core/Inc/tl_rtc.h` ve `Core/Src/tl_rtc.c`: `TL_RTC_Set(uint32_t unix_ts)` ve `TL_RTC_Get()` Unix saniyeleri ile çalışıyor. Mevcut dönüşüm saniye çözünürlüğünde.
- `Core/Src/tl_can.c`: ham CAN alım yolunda RTC ayarlama komutu, dört bayt little-endian zaman değerini `TL_RTC_Set` çağrısına aktarıyor. Komut değeri `Core/Inc/tl_can.h` içinde `0x01`.
- `Core/Src/main.c`: başlangıçta RTC sabit `1710255720UL` değeriyle ayarlanıyor. Bu değer güncel Linux saati değildir; ilk eşitleme öncesi zamanın geçerliliği tasarımda ele alınmalı.
- Periyodik Linux göndericisinin uygulaması bu incelemede doğrulanmadı. Mevcut RTC komut biçimi yeni protokol için bağlayıcı mesaj tipi olarak kabul edilmedi.

Bu yollar `tronloop-vertex-firmware/` deposuna göredir. Kod değiştirilmedi, donanım testi yapılmadı.

## Gerekçe ve etkiler

Dairesel tampondan gecikmeli gelen ölçümlerin zamanı korunur. Sıra numarası kayıt sırasını ve boşlukları değerlendirmeyi sağlar; kayıpsız teslim garantisi oluşturmaz.

## Kararın netleştirilmesi — 2026-09-18

Ölçüm zamanı milisaniye cinsinden tutulacak. Bu, mevcut RTC kodunun milisaniye ürettiği veya saat eşitlemenin 1 ms doğruluk sağladığı anlamına gelmez. Mevcut saniye tabanlı uygulamanın uyarlanması gerekiyor. Kod değişikliği yapılmadı; alanın kodlanma biçimi ve saat güncelleme paketinin çözünürlüğü ayrıca seçilecek.

## Sıra numarasının netleştirilmesi — 2026-09-18

Sıra numarası artmaya devam edecek ve kayıtları ayırt etmek için kullanılacak. Her testte sıfırdan başlatma önerisi benimsenmedi; test değişimi sayacı sıfırlamayı gerektirmez. Bu alan tek başına tüm cihazlar ve yeniden başlamalar boyunca benzersiz bir kimlik garantisi olarak yorumlanmaz.

## Açık konular

- Milisaniye çözünürlüklü ölçüm zamanının alan boyutu ve tel biçimi; STM32 üzerinde saniye altı zamanın nasıl üretileceği.
- Artan ayırt edici sayacın alan genişliği, cihaz yeniden başladığında başlangıç değeri ve taşma davranışı. Test başında sıfırlama şartı yoktur.
- Saat eşitleme aralığı ve ilk eşitlemenin zamanı.
- İlk eşitleme öncesi zaman geçerliliği ve saat ileri/geri düzeltildiğinde kayıtların yorumlanması.
- Saat güncelleme mesajının yeni protokoldeki biçimi ve yanıtı.

## İlişkili belgeler

[Dairesel tampon](ADR-0006-vertex-ring-buffer.md) · [Haberleşme notları](../03-software/communication-notes.md)

## Uygulama güncellemesi — ADR-0011

Telemetriye uint64_t Unix milisaniye alanı eklendi. İlk uygulamada zaman hızlı context ölçümünde kaydediliyordu. [ADR-0012](ADR-0012-payload-time.md) ile bu bölüm değişti: zaman artık payload oluşturulurken RTC’den okunuyor. Ölçüm anı ile paket oluşturma anı aynı kabul edilmemeli. `TL_RTC_GetMs()` RTC subsecond alanını kullanıyor; mevcut prescaler nominal ~3,9 ms adımlı. Önceki yalnız saniye çözünürlüklü uygulama gözlemi `TL_RTC_Get()` için geçerli kalır, yeni API ayrı eklenmiştir. Sıra numarası ve Linux saat eşitleme ayrıntıları henüz uygulanmadı. [Güncel telemetri](../03-software/vertex-telemetry-message.md).
