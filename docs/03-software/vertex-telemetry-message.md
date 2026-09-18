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

Gerilim ve akımı bu paketle gönderiyoruz. Sıcaklıkları yalnız [genel durumda](general-status-message.md) gönderiyoruz; ayrı sıcaklık mesajını kaldırdık. Telemetri artık **13 bayt**, tür kodu **0x01**. Test oynatıcısı RUNNING durumundayken hedef aralık **100 ms**. Çok baytlı alanlar little-endian gidiyor.

| Bayt | Alan | Tür / boyut | Anlam |
|---|---|---|---|
| 0 | `payload_type` | `uint8_t` / 1 | `0x01` |
| 1–2 | `battery_voltage_mv` | `uint16_t` / 2 | Pil gerilimi, mV |
| 3–4 | `battery_current_ma` | `int16_t` / 2 | Pil akımı, mA; pozitif şarj, negatif deşarj |
| 5–12 | `measurement_time_ms` | `uint64_t` / 8 | Payload oluşturulurken RTC’den okunan Unix milisaniye zamanı |

Boyutu ve alanların yerini `_Static_assert` ile derlemede kontrol ediyoruz. `state` alanını çıkardık. Sıra numarası ekleme kararımız var ama henüz pakete girmedi.

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

13 bayt veri iki çerçeveye ayrılıyor: **6 + 7 bayt**. Bir Flow Control ile toplam **2 veri + 1 Flow Control** çerçevesi var. Alıcı önce türü, sonra boyutu kontrol etmeli: `0x01` artık 13 bayt. Mesajları uzunlukla değil tür alanıyla ayırıyoruz.

Önceki 17 baytlık biçim [ADR-0013](../07-decisions/ADR-0013-separate-temperature-message.md) ile değişti. Tür kodu aynı kaldı; alıcı da yeni alan yerleşimine geçirilmeden bu sürüme geçilmemeli.

ISO-TP meşgulse o tur telemetri atlanıyor. Gönderim başlatılamazsa hata loglanıyor. Ring buffer henüz yok; şu an kaçırılan ölçümleri saklayıp sonra gönderemiyoruz. Başlamış aktarımın içindeki zaman ise değişmiyor.

## Neyi kontrol ettik?

Debug derlemesi geçti. Gerçek dispatcher ve ISO-TP koduyla bilgisayarda 13 baytlık yerleşimi, RTC zamanının paket boyunca korunmasını ve iki veri çerçevesini kontrol ettik. RUNNING dışındaki telemetri kısıtını da denedik. Ayrı sıcaklık gönderiminin kaldırıldığı da kontrol edildi. Kart testi ve ClusterPilot alıcı güncellemesi yapılmadı.

## Kaynaklar

`tronloop-vertex-firmware/` altında: `Core/Inc/tl_dispatcher.h`, `Core/Src/tl_dispatcher.c`, `Core/Inc/tl_context.h`, `Core/Src/tl_context.c`, `Core/Inc/tl_rtc.h`, `Core/Src/tl_rtc.c`, `Core/Src/main.c`; STM32 subsecond formülü `Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c` açıklamasından doğrulandı.

[Mesaj envanteri](vertex-message-inventory.md) · [Mesajlaşma protokolü](communication-notes.md) · [Zaman/sıcaklık kararı](../07-decisions/ADR-0011-telemetry-time-temperature.md)
