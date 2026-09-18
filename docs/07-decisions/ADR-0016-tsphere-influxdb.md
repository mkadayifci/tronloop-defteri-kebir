---
baslik: "ADR-0016 — TSphere zaman serilerini InfluxDB’de tutma"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0016 — TSphere zaman serilerini InfluxDB’de tutma

TSphere’de zaman serileri için InfluxDB kullanacağız. MQTT’den gelen ölçümleri buraya yazıp test boyunca gerilim, akım ve sıcaklığın nasıl değiştiğini izleyebileceğiz. ClusterPilot’un yerel SQLite kuyruğu da bağlantı kesintileri için yerinde kalıyor.

Şimdilik kesinleşen seçim InfluxDB. Sürümünü, MQTT tüketicisini, veri şemasını ve saklama süresini daha belirleyeceğiz. Kullanıcı, test senaryosu ve cihaz envanteri gibi diğer kayıtların veritabanını bu kararla seçmiyoruz. Önceki mimari taslağındaki PostgreSQL ve eşitleme düzeni de kendiliğinden bu karara dahil değil.

MQTT ile InfluxDB arasına küçük bir kayıt servisi koymak ilk öneri. Telegraf da değerlendirilebilir; mevcut MQTT içeriği ham ikili paket olduğu için hangi yolu seçersek seçelim mesajları güncel şemayla ayrıştırmamız gerekiyor. [Kayıt akışı ve şema önerisi](../03-software/tsphere-timeseries.md).

Kurulum veya tüketici kodu bu çalışmada yapılmadı. ClusterPilot alıcısı hâlâ eski paket biçimini kullanıyor; önce 13 bayt telemetri ve 11 bayt genel durumla uyumlu hale gelmeli.

**Kayıt:** 2026-09-18 · Proje görüşmesi, kabul edildi. Kod kontrolü: `tronloop-clusterpilot-engine/TelemetryPublisher.cs` ve `CanIsoTpListener.cs`.

[Mimari](../03-software/architecture.md) · [Karar defteri](../01-project-general/decision-hub.md)
