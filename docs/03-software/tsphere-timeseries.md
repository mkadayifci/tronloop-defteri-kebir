---
baslik: "TSphere — Zaman serileri ve InfluxDB"
kategori: "03-software"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# TSphere — Zaman serileri ve InfluxDB

Zaman serilerini TSphere’de InfluxDB’ye yazacağız. Bu seçim [ADR-0016](../07-decisions/ADR-0016-tsphere-influxdb.md) ile kesinleşti. Aşağıdaki servis, şema ve saklama düzeni ise henüz öneri; kurulmuş sistemin tarifi değil.

## Veriyi nasıl yazalım?

Önerilen yol: **ClusterPilot → TSphere MQTT → kayıt servisi → InfluxDB**. Kayıt servisi mesaj türünü ve uzunluğunu kontrol eder, hangi Cluster ve Vertex’ten geldiğini belirler, alanları veritabanına yazar. Mevcut C#/.NET yapısıyla küçük bir worker bu iş için uygun görünüyor.

Bugünkü `TelemetryPublisher.cs`, struct belleğini ham ikili MQTT payload’ı olarak gönderiyor. Varsayılan topic de tek bir ClusterPilot/Vertex adı içeriyor. 16 Vertex’e geçmeden kaynak kimliğini doğru taşımalı, alıcıdaki eski `FastTelemetryPayload` ayrıştırmasını güncellemeliyiz. MQTT için sürüm, kaynak kimlikleri, ilk alınma zamanı ve test ilişkisini taşıyan bir zarf öneriyoruz; JSON veya ikili biçim seçimi henüz açık. CAN paketlerini bunun için değiştirmek gerekmiyor.

Telegraf, MQTT’den veri alıp InfluxDB’ye yazabilen hazır alternatif. Basit, kararlı bir JSON şeması seçersek işimizi azaltabilir. Bizim paket doğrulama, zaman kaynağı ve tekrar gönderim kurallarımız için ayrı servis daha esnek görünüyor. [Telegraf MQTT örneği](https://docs.influxdata.com/telegraf/v1/examples/collect-mqtt/).

## İlk şema önerisi

Her Vertex için ayrı tablo açmak yerine kaynak kimliklerini etiketlerde tutalım. `cluster_id` ve `vertex_id` etiket; sayısal ölçümler alan olsun. `test_run_id` ile deney ilişkisini de taşıyalım, ancak bunun etiket mi alan mı olacağını seçilen sürüm ve sorgulara göre belirleyelim. Her kayıtta değişen sıra numarasını doğrudan etiket yapmayalım. [InfluxDB şema rehberi](https://docs.influxdata.com/influxdb3/core/write-data/best-practices/schema-design/).

| Seri | Alanlar | Kayıt zamanı |
|---|---|---|
| `vertex_telemetry` | `battery_voltage_mv`, `battery_current_ma` | Vertex’in `measurement_time_ms` değeri |
| `vertex_status` | `battery_voltage_mv`, `battery_current_ma`, `scenario_player_state`, `charger_mode`, `battery_temperature_c`, `ambient_temperature_c`, iki sıcaklık için geçerlilik alanları | ClusterPilot’un paketi ilk aldığı UTC zamanı |

Gerilim ve akımı tam sayı mV/mA olarak koruyalım. Sıcaklıkları °C × 10’dan °C’ye çevirip sürekli aynı sayısal türle yazalım. −32768, ölçüm olmadığı anlamına geliyor: sıcaklık alanını o noktada yazmayıp ilgili geçerlilik alanını false yapalım. Bu değeri −3276,8 °C diye grafiğe taşımayalım. Ayrı sıcaklık mesajı yok; sıcaklıkların kaynağı genel durum.

Telemetri zamanı sensörün tam ölçüm anı değil, firmware’in payload oluşturduğu an. Yazma API’sine Unix milisaniye hassasiyetini açıkça bildirelim. Genel durumda zaman alanı yok; bu yüzden ClusterPilot zamanı ayrıca taşınmalı ve SQLite’tan tekrar gönderirken korunmalı. Buluta varış zamanını ayrıca tutabiliriz, fakat bunu eski kayıtların asıl zamanı yerine koymayalım. RTC eşitlenmemişse veya geçersizse kaydı sessizce bugüne taşımak yerine saat sorunu olarak ayıralım; bu kontrol henüz uygulanmadı.

Tekrar gönderimde aynı kaydın zamanını, etiketlerini ve ölçüm değerlerini koruyalım. InfluxDB 3’te zaman ve etiket kümesi kayıt anahtarını oluşturuyor; aynı anahtar farklı iki ölçümü birbirinden ayırmaz. Sıra numarasını sıradan bir alan olarak eklemek de bu çakışmayı çözmez. Saat geri alınması ve Vertex yeniden başlaması için kayıt kimliğini ayrıca netleştirmeliyiz. Fiziksel ClusterPilot kimliğini ölçüm anahtarına koymak, yedek karttan gelen aynı kaydı farklı bir kayıt haline getirebilir.

## Kesinti ve tekrar gönderim

Başlangıç için en fazla yaklaşık 1 saniye veya 200 kayıt biriktiğinde toplu yazma deneyebiliriz. Gerçek gecikme ve yükü ölçüp ayarları değiştirelim. RAM’de bekleyen bir grup kayıt kalıcı kuyruk sayılmaz.

MQTT QoS 1, kalıcı abonelik ve broker’ın disk kalıcılığı birlikte yapılandırılmalı. Tüketici mesajı ancak InfluxDB yazısı başarıyla sonuçlanınca veya kendi kalıcı kuyruğuna kaydedince onaylamalı. MQTT kütüphanesi otomatik onay veriyorsa bu davranış özellikle kontrol edilmeli. Telegraf’ın MQTT girdisi de çıkışa teslimi izleyerek onay verebiliyor. [Teslim takibi](https://docs.influxdata.com/telegraf/v1/examples/collect-mqtt/).

Başarısız yazıları aynı kayıt kimliğiyle yeniden deneyelim. Toplu yazıda kısmi başarıyı kontrol edelim; bozuk bir paketi ayrı hata kaydına alıp bütün kuyruğu durdurmasını önleyelim. Bekleyen kayıt sayısı, en eski kaydın yaşı ve son başarılı yazma zamanı izlenmeli.

ClusterPilot’un MQTT yayın onayı, InfluxDB’ye yazıldığının onayı değil. SQLite’tan silme koşulu ve uçtan uca uygulama onayı hâlâ açık. Buradaki düzen, Vertex ring buffer’ının üzerine yazdığı veriyi geri getirmez; bütün sistem için kayıpsızlık sözü vermiyoruz.

## Sürüm ve veri büyüklüğü

16 Vertex sürekli RUNNING durumunda, 100 ms aralıkla **160 telemetri kaydı/saniye**, yani **13.824.000 kayıt/gün** üretir. Genel durum yaklaşık **460.800 kayıt/gün** daha ekler. Bunlar CAN çerçevesi değil, veritabanındaki nokta sayıları; sıcaklıklar genel durumun alanları olduğu için ayrı nokta sayılmıyor. Yeniden aktarımlar ve olası heartbeat arşivi bu hesaba dahil değil.

Canlı ekranlarda seyreltilmiş grafikler kullanabiliriz. Araştırma verisini yalnız ortalamalara indirgemeden önce ham kayıtların ne kadar süre InfluxDB’de kalacağını ve uzun vadeli arşivini belirleyelim. Disk ihtiyacını temsili veriyle ölçelim; yedekleme ve geri yüklemeyi de deneyelim.

InfluxDB sürümünü uzun test sorgularına göre seçmeliyiz. InfluxDB 3 Core’un varsayılan `query-file-limit=432` ve 10 dakikalık dosya aralığı yaklaşık 72 saatlik sorgu aralığına karşılık geliyor; geç gelen veriler bunu daraltabilir. Sınır artırılabiliyor ama bellek ve sorgu maliyeti büyüyor. Bu bir veriyi 72 saatte silme kuralı değil. Haftalar/aylar süren testleri sorgulama ihtiyacını Core’a karar vermeden deneyelim; sürüm ve edisyon henüz seçilmedi. [Core yapılandırması](https://docs.influxdata.com/influxdb3/core/reference/config-options/#query-file-limit).

**Kaynak ve kontrol:** 2026-09-18 proje görüşmesi; `tronloop-clusterpilot-engine/TelemetryPublisher.cs`, `CanIsoTpListener.cs`; güncel [telemetri](vertex-telemetry-message.md) ve [genel durum](general-status-message.md) şemaları. InfluxData belgeleri aynı tarihte incelendi. Sunucu kurulumu ve uygulama kodu değiştirilmedi.
