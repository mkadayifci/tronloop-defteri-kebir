---
baslik: "VertexTelemetryPayload — Telemetri Mesajı"
kategori: "03-software"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# VertexTelemetryPayload — Telemetri Mesajı

**Son Güncelleme:** 2026-09-18

## Güncel biçim

Pil ve ortam sıcaklıkları ayrı **int16_t, 0,1 °C** alanlarına geçirildi ve zaman alanı eklendi. Zamanın tel biçimi **uint64_t Unix milisaniye** olarak korundu. Toplam uygulama mesajı **17 bayt**, tür kodu **0x01**, hedef gönderim aralığı **100 ms**. Çok baytlı alanlar STM32 üzerinde little-endian gönderilir.

| Bayt | Alan | Tür / boyut | Anlam |
|---|---|---|---|
| 0 | `payload_type` | `uint8_t` / 1 | `0x01` |
| 1–2 | `battery_voltage_mv` | `uint16_t` / 2 | Pil gerilimi, mV |
| 3–4 | `battery_current_ma` | `int16_t` / 2 | Pil akımı, mA; pozitif şarj, negatif deşarj |
| 5–6 | `battery_temperature_dc` | `int16_t` / 2 | Pil sıcaklığı × 10 |
| 7–8 | `ambient_temperature_dc` | `int16_t` / 2 | Ortam sıcaklığı × 10 |
| 9–16 | `measurement_time_ms` | `uint64_t` / 8 | Payload oluşturulurken RTC’den okunan Unix milisaniye zamanı |

Boyut ve ofsetler derleme zamanı `_Static_assert` kontrolleriyle sabitlenir. `state` alanı yoktur. Sıra numarası önceki kararlarda istenmiştir ancak bu kod değişikliğinde eklenmedi; henüz bu pakette bulunmaz.

## Sıcaklık kodlaması

- `253` → **25,3 °C**, `-125` → **−12,5 °C**. Gönderici kesir atmaz; context'teki onda bir derece değerini doğrudan taşır.
- `INT16_MIN` (**−32768**, telde `00 80`) ölçüm yok/geçersiz işaretidir. Önceki `int8_t` biçimindeki −128 işareti artık geçerli değildir; −128 yeni biçimde gerçek **−12,8 °C** değeridir.
- Geçerli kodlama aralığı −32767…32767, yani −3276,7…3276,7 °C'dir. Bu yalnızca sayısal aralıktır, sensör çalışma aralığı değildir.
- Pil kaynağı `dut.temperature_dC`, ortam kaynağı `ambient_temperature_dC`. Sensör okumaları henüz uygulanmadı; başlangıçta iki alan da `TL_TEMPERATURE_UNAVAILABLE_DC` olur.

## Ölçüm zamanı

Dispatcher, payload oluştururken `.measurement_time_ms = TL_RTC_GetMs()` ile zamanı doğrudan RTC’den alır. Context içinde zaman alanı tutulmaz. ISO-TP devam çerçeveleri aynı paketin zamanını korur. [ADR-0012](../07-decisions/ADR-0012-payload-time.md).

Alan adı korunmuştur; bu zaman payload oluşturma anını temsil eder, sensör edinim zamanı değildir. Sıcaklık sensörleri henüz çalışmadığından eşzamanlı çoklu sensör ölçümü iddiası değildir. Gerilim güncellemesi de henüz yorum satırındadır.

`TL_RTC_GetMs()` RTC tarih/saat ve subsecond alanlarını aynı okuma akışında kullanır; `HAL_RTC_GetTime` ardından kilidi açmak için her durumda `HAL_RTC_GetDate` çağrılır:

```text
unix_ms = unix_seconds × 1000
        + floor((SecondFraction - SubSeconds) × 1000 / (SecondFraction + 1))
```

Mevcut `SynchPrediv=255` ile nominal saniye altı adım yaklaşık **3,90625 ms**'dir. Alan milisaniye cinsindedir; 1 ms doğruluk/çözünürlük garantisi verilmez. RTC okuma hatasında veya geçersiz subsecond değerinde 0 döner.

Başlangıç kodu RTC'yi hâlâ sabit `1710255720` değerine kurar. Gerçek mutlak zaman için Linux saat eşitlemesi gereklidir. Mevcut RTC ayarlama mesajı saniye tabanlıdır; periyodik Linux gönderimi, eşitleme doğruluğu ve ilk eşitleme geçerlilik takibi bu değişiklik kapsamında uygulanmadı. Mevcut takvim dönüşümleri 2000–2099 RTC yıl yorumuna dayanır.

## ISO-TP gönderimi

17 bayt veri üç veri çerçevesine ayrılır: **6 + 7 + 4 bayt**. Alıcı bir Flow Control ile devam izni verdiğinde (blocksize 0 veya en az 2), toplam trafik **3 veri + 1 Flow Control** çerçevesidir. Son çerçeve mevcut ayarla doldurulur. Veri uzunluğu türü seçmez; alıcı `0x01` için yeni 17 baytlık şemayı doğrulamalıdır.

ISO-TP meşgulse mevcut dispatcher o tur telemetriyi göndermez; gönderim başlatma hatası loglanır. Daha önce kararlaştırılan Vertex dairesel tamponu henüz bu kodda uygulanmamıştır. Dolayısıyla kaydedilen zaman aktarım sırasında korunur, ancak kaçırılan ölçümlerin tamponlanıp sonradan gönderilmesi bu güncellemenin tamamladığı bir özellik değildir.

## Doğrulama

- Debug firmware derlemesi başarılı; senaryo kaynağındaki mevcut printf bildirim/biçim uyarıları sürüyor.
- Önceki uygulamada gerçek dispatcher ve ISO-TP koduyla bilgisayarda 17 bayt yerleşim, sıcaklıklar, 64 bit zaman ve üç veri çerçevesi doğrulandı. Bu testler context zamanını kullanan sürüme aittir; RTC’nin payload oluştururken okunması değişikliğinde Debug derlemesi kullanıldı.
- Gerçek RTC yardımcı kodu, taklit HAL ile saniye altı hesap, saniye/gün geçişi, okuma sırası, hata dönüşleri ve 64 bit sonuç için doğrulandı.
- Kart/sensör testi yapılmadı. ClusterPilot alıcı kodu değiştirilmedi.

## Kaynaklar

`tronloop-vertex-firmware/` altında: `Core/Inc/tl_dispatcher.h`, `Core/Src/tl_dispatcher.c`, `Core/Inc/tl_context.h`, `Core/Src/tl_context.c`, `Core/Inc/tl_rtc.h`, `Core/Src/tl_rtc.c`, `Core/Src/main.c`; STM32 subsecond formülü `Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c` açıklamasından doğrulandı.

[Mesaj envanteri](vertex-message-inventory.md) · [Mesajlaşma protokolü](communication-notes.md) · [Zaman/sıcaklık kararı](../07-decisions/ADR-0011-telemetry-time-temperature.md)
