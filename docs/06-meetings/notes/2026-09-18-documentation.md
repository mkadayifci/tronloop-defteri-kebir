---
baslik: "2026-09-18 — Dokümantasyon Düzeni"
kategori: "06-meetings"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# 2026-09-18 — Mimari ve haberleşme notları

Kararları ve mimari notları Defteri Kebir’de topluyoruz. Diğer klasörler kod ve tasarım kaynakları; adı `__` ile biten klasörler güncel çalışmaların dışında. [Dokümantasyon düzeni](../../07-decisions/ADR-0001-documentation.md) ve [kaynak kapsamı](../../07-decisions/ADR-0002-documentation-scope.md) bu ayrımı tanımlıyor.

## Sistemin genel yapısı

Her Cluster içinde Vertex test birimleri var. Test senaryosunu Vertex kendi başına yürütüyor; ClusterPilot’un sürekli komut göndermesine ihtiyaç duymuyor. Linux üzerinde çalışan ClusterPilot, CAN/ISO-TP üzerinden verileri topluyor ve TSphere üzerindeki MQTT’ye gönderiyor. Buluta gönderilemeyen veriler SQLite’ta bekliyor. Panel komutları da aynı yolun tersinden ilgili Vertex’e ulaşıyor; yanıtlar ClusterPilot üzerinden geri dönüyor.

İlgili kararlar: [genel mimari](../../07-decisions/ADR-0003-system-overview.md), [bağımsız test yürütme](../../07-decisions/ADR-0004-autonomous-vertex.md), [TSphere adı](../../07-decisions/ADR-0009-tsphere-name.md).

## Kısa kesintiler ve zaman bilgisi

Normalde Vertex–ClusterPilot bağlantısının açık kalmasını bekliyoruz. Vertex’teki dairesel tampon kısa kesintileri karşılayacak. Dolunca en eski kayıtların üzerine yazılacak; test ve kayıt durmayacak. Bağlantı düzelince tamponda kalan kayıtlar aktarılacak. Kayıpsız saklama veya kalıcı teslim onayına kadar koruma şartı yok. İlk biriktirme kararı [ADR-0005](../../07-decisions/ADR-0005-vertex-data-buffer.md), bu davranışı netleştiren [ADR-0006](../../07-decisions/ADR-0006-vertex-ring-buffer.md) ile değişti.

Kayıtlar için milisaniye cinsinden zaman ve artan bir sıra numarası seçildi. Sıra numarası yalnızca ayırt edici; her testin başında sıfırlanması gerekmiyor. Sayaç genişliği, taşma ve yeniden başlama davranışları açık. STM32 saati Linux zamanıyla periyodik mesajlar üzerinden eşitlenecek. Ham CAN üzerinden saniye tabanlı RTC ayarlama kodu var; Linux’taki periyodik gönderici henüz doğrulanmadı. [ADR-0007](../../07-decisions/ADR-0007-measurement-time-sequence.md).

## Mesaj tasarımının gelişimi

Aşağıdaki ara tasarımlar tarihçedir. Güncel biçimler genel durumda **7 bayt**, telemetride **17 bayt**.

| Konu | İlk yaklaşım | Son durum |
|---|---|---|
| Mesaj türü | Türü `dataLength` ile bulma ve her türe farklı uzunluk ayırma | Tür alanıyla ayrıştırma; uzunluk yalnızca şema kontrolü için kullanılıyor. [ADR-0010](../../07-decisions/ADR-0010-message-type-and-operation-mode.md), [ADR-0008](../../07-decisions/ADR-0008-unique-message-length.md) yerine geçti. |
| Şarj ve ters mod | İki ayrı bayrak | Tek idle/şarj/deşarj alanı |
| Oynatıcı ve charger durumu | Önce aynı baytta bit alanları; 2 bayt akımla ara hedef 6 bayt | Ayrı birer bayt; genel durum toplam 7 bayt |
| Akım | Genel durumda 4 bayt | mA cinsinden işaretli 2 bayt |
| Yapı adları | `GeneralStatusPayload`, `FastTelemetryPayload` | `VertexStatusPayload`, `VertexTelemetryPayload` |
| Telemetri durumu | Sabit `state` alanı | Kaldırıldı |
| Sıcaklık | Tek alan, ardından iki ayrı `int8_t` tam °C alanı | Pil ve ortam için ayrı `int16_t`, °C × 10 |
| Geçersiz sıcaklık | `int8_t` tasarımında −128 | Güncel `int16_t` tasarımında −32768 |
| Zaman | Önce hızlı context güncellemesinde saklanan zaman | Payload oluşturulurken doğrudan `TL_RTC_GetMs()` çağrısı |

Zamanı küçültüp iki veri çerçevesine inme seçeneği değerlendirildi; **uint64_t Unix milisaniye** korundu. Güncel telemetri 3 ISO-TP veri çerçevesiyle taşınıyor. Alıcının Flow Control çerçevesi de trafik hesabına dahil. [Sıcaklık ve zaman biçimi](../../07-decisions/ADR-0011-telemetry-time-temperature.md), [RTC okuma anı](../../07-decisions/ADR-0012-payload-time.md).

## Firmware’deki karşılığı

Genel durumun 7 baytlık biçimi ve telemetrinin 17 baytlık biçimi uygulandı. Genel durumda mod kodları 0 idle, 1 şarj, 2 deşarj; reverse kontrolü öncelikli. Akım `int16_t` aralığı dışındaysa genel durum gönderilmiyor ve hata loglanıyor.

Telemetride pil ve ortam sıcaklıklarının alanları hazır, sensör okumaları henüz yok. Gerilim güncellemesi de yorum satırında. Zaman alanının adı `measurement_time_ms`, fakat son düzenlemede sensör edinim anını değil payload oluşturma anını gösteriyor. RTC’nin nominal adımı yaklaşık 3,9 ms; milisaniye birimi 1 ms doğruluk garantisi vermiyor.

Debug derlemesi geçti. Önceki paket düzenlemelerinde gerçek dispatcher/ISO-TP koduyla bilgisayarda boyut, alan yerleşimi, sıcaklık ve akım sınırları, çok çerçeveli aktarım; RTC yardımcı kodunda saniye altı hesaplar ve hata dönüşleri kontrol edildi. Son RTC çağrı yeri değişikliği derlemeyle doğrulandı. Kart ve sensör testi yapılmadı. ClusterPilot alıcısı eski biçimde; sıra numarası ve dairesel tampon firmware’de henüz yok.

Alanların güncel karşılığı: [genel durum](../../03-software/general-status-message.md), [telemetri](../../03-software/vertex-telemetry-message.md), [mesaj envanteri](../../03-software/vertex-message-inventory.md).

## Hat kapasitesi

16 Vertex ve 100 ms aralıkla saniyede 160 telemetri mesajı oluşuyor. 500 kbit/s CAN hattında ISO-TP, Flow Control, heartbeat ve durum mesajları dahil hesaplanan yük yaklaşık **%15–18,3**. Komutlar, tekrarlar ve tampon boşaltma trafiği bu hesaba dahil değil. [Hesabın ayrıntıları](../../03-software/communication-notes.md).

## Dokümantasyon düzeni

Eski terimler Cluster, ClusterPilot, Vertex ve TSphere adlarıyla eşleştirildi; teknik semboller ve dosya yolları korundu. Mesaj akışları diyagramlarla belgelendi. Defteri Kebir’de konuşma dökümü yerine doğrudan kararları, gerekçeleri ve uygulama durumunu anlatıyoruz. Dil sade ve doğal; geçmiş tasarımlar güncel durumdan ayrı tutuluyor.
