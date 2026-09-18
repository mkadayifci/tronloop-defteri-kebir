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
- **Kararı veren:** Kullanıcı
- **Kaynak:** Kullanıcının ölçüm zamanı ve sıra numarası önerisini kabul etmesi; STM32 saatinin aktif olacağını ve Linux zamanı kullanılarak mesajlarla belirli aralıklarla güncelleneceğini belirtmesi.

## Karar

Test ölçüm kayıtları ölçüm zamanını ve sıra numarasını içerecek. Zaman ölçümün alındığı anı ifade eder; tampon boşaltılırken gönderim zamanı ile değiştirilmez. Bu alanlar gecikmeli kayıtların zamanını ve aradaki kayıt boşluklarını ayırt etmek için kullanılır.

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

## Açık konular

- Ölçüm zamanının saniye veya daha yüksek çözünürlükte olması; alan boyutu ve tel biçimi.
- Sıra numarasının genişliği, test/cihaz/oturum kapsamı, başlangıç ve taşma davranışı.
- Saat eşitleme aralığı ve ilk eşitlemenin zamanı.
- İlk eşitleme öncesi zaman geçerliliği ve saat ileri/geri düzeltildiğinde kayıtların yorumlanması.
- Saat güncelleme mesajının yeni protokoldeki biçimi ve yanıtı.

## İlişkili belgeler

[Dairesel tampon](ADR-0006-vertex-ring-buffer.md) · [Haberleşme notları](../03-software/communication-notes.md)
