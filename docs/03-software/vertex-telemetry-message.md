---
baslik: "VertexTelemetryPayload — Telemetri Mesajı"
kategori: "03-software"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# VertexTelemetryPayload — Telemetri Mesajı

**Son Güncelleme:** 2026-09-18

## Pakette ne var?

Sık gelen ölçümleri bu paketle gönderiyoruz. Pil ve ortam sıcaklığı ayrı; ikisi de **int16_t, °C × 10**. Zaman için **uint64_t Unix milisaniye** kullanıyoruz. Toplam **17 bayt**, tür kodu **0x01**. Hedef aralık **100 ms**; çok baytlı alanlar little-endian gidiyor.

| Bayt | Alan | Tür / boyut | Anlam |
|---|---|---|---|
| 0 | `payload_type` | `uint8_t` / 1 | `0x01` |
| 1–2 | `battery_voltage_mv` | `uint16_t` / 2 | Pil gerilimi, mV |
| 3–4 | `battery_current_ma` | `int16_t` / 2 | Pil akımı, mA; pozitif şarj, negatif deşarj |
| 5–6 | `battery_temperature_dc` | `int16_t` / 2 | Pil sıcaklığı × 10 |
| 7–8 | `ambient_temperature_dc` | `int16_t` / 2 | Ortam sıcaklığı × 10 |
| 9–16 | `measurement_time_ms` | `uint64_t` / 8 | Payload oluşturulurken RTC’den okunan Unix milisaniye zamanı |

Boyutu ve alanların yerini `_Static_assert` ile derlemede kontrol ediyoruz. `state` alanını çıkardık. Sıra numarası ekleme kararımız var ama henüz pakete girmedi.

## Sıcaklık kodlaması

- `253` → **25,3 °C**, `-125` → **−12,5 °C**. Gönderici kesir atmaz; context'teki onda bir derece değerini doğrudan taşır.
- `INT16_MIN` (**−32768**, telde `00 80`) ölçüm yok/geçersiz işaretidir. Önceki `int8_t` biçimindeki −128 işareti artık geçerli değildir; −128 yeni biçimde gerçek **−12,8 °C** değeridir.
- Geçerli kodlama aralığı −32767…32767, yani −3276,7…3276,7 °C'dir. Bu yalnızca sayısal aralıktır, sensör çalışma aralığı değildir.
- Pil kaynağı `dut.temperature_dC`, ortam kaynağı `ambient_temperature_dC`. Sensör okumaları henüz uygulanmadı; başlangıçta iki alan da `TL_TEMPERATURE_UNAVAILABLE_DC` olur.

## Ölçüm zamanı

Zamanı payload oluştururken doğrudan RTC’den alıyoruz: `.measurement_time_ms = TL_RTC_GetMs()`. Context’te ayrıca zaman tutmuyoruz. Paket ISO-TP ile bölünse de içindeki zaman aynı kalıyor. [ADR-0012](../07-decisions/ADR-0012-payload-time.md).

Alanı `measurement_time_ms` diye bıraktık, ama şu an gösterdiği şey paketin oluşturulma anı. Sensörlerin tam o anda birlikte okunduğunu varsaymıyoruz. Zaten sıcaklık okumaları henüz yok, gerilim güncellemesi de yorum satırında.

`TL_RTC_GetMs()` RTC tarih/saat ve subsecond alanlarını aynı okuma akışında kullanır; `HAL_RTC_GetTime` ardından kilidi açmak için her durumda `HAL_RTC_GetDate` çağrılır:

```text
unix_ms = unix_seconds × 1000
        + floor((SecondFraction - SubSeconds) × 1000 / (SecondFraction + 1))
```

`SynchPrediv=255` ayarında RTC yaklaşık **3,90625 ms** adımlarla ilerliyor. Değeri milisaniye olarak göndermemiz, saatin 1 ms hassasiyetinde olduğu anlamına gelmiyor. Okuma hatasında veya geçersiz saniye altı değerinde fonksiyon 0 dönüyor.

Açılışta RTC’ye hâlâ sabit `1710255720` yazılıyor. Doğru tarihi görmek için Linux’tan eşitleme gelmesi gerekiyor. Mevcut ayarlama mesajı saniye cinsinden; periyodik gönderim, eşitleme doğruluğu ve ilk eşitleme öncesindeki geçerlilik takibi henüz tamamlanmadı. Takvim dönüşümü 2000–2099 aralığını kullanıyor.

## ISO-TP gönderimi

17 bayt veri üç veri çerçevesine ayrılır: **6 + 7 + 4 bayt**. Alıcı bir Flow Control ile devam izni verdiğinde (blocksize 0 veya en az 2), toplam trafik **3 veri + 1 Flow Control** çerçevesidir. Son çerçeve mevcut ayarla doldurulur. Veri uzunluğu türü seçmez; alıcı `0x01` için yeni 17 baytlık şemayı doğrulamalıdır.

ISO-TP meşgulse o tur telemetri atlanıyor. Gönderim başlatılamazsa hata loglanıyor. Ring buffer henüz yok; şu an kaçırılan ölçümleri saklayıp sonra gönderemiyoruz. Başlamış aktarımın içindeki zaman ise değişmiyor.

## Neyi kontrol ettik?

- Debug firmware derlemesi başarılı; senaryo kaynağındaki mevcut printf bildirim/biçim uyarıları sürüyor.
- Önceki uygulamada gerçek dispatcher ve ISO-TP koduyla bilgisayarda 17 bayt yerleşim, sıcaklıklar, 64 bit zaman ve üç veri çerçevesi doğrulandı. Bu testler context zamanını kullanan sürüme aittir; RTC’nin payload oluştururken okunması değişikliğinde Debug derlemesi kullanıldı.
- Gerçek RTC yardımcı kodu, taklit HAL ile saniye altı hesap, saniye/gün geçişi, okuma sırası, hata dönüşleri ve 64 bit sonuç için doğrulandı.
- Kart/sensör testi yapılmadı. ClusterPilot alıcı kodu değiştirilmedi.

## Kaynaklar

`tronloop-vertex-firmware/` altında: `Core/Inc/tl_dispatcher.h`, `Core/Src/tl_dispatcher.c`, `Core/Inc/tl_context.h`, `Core/Src/tl_context.c`, `Core/Inc/tl_rtc.h`, `Core/Src/tl_rtc.c`, `Core/Src/main.c`; STM32 subsecond formülü `Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c` açıklamasından doğrulandı.

[Mesaj envanteri](vertex-message-inventory.md) · [Mesajlaşma protokolü](communication-notes.md) · [Zaman/sıcaklık kararı](../07-decisions/ADR-0011-telemetry-time-temperature.md)
