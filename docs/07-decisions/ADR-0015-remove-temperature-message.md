---
baslik: "ADR-0015 — Ayrı sıcaklık mesajını kaldırma"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0015 — Ayrı sıcaklık mesajını kaldırma

Sıcaklıklar zaten genel durum mesajında var. Ayrı `VertexTemperaturePayload` mesajını tamamen kaldırıyoruz; test çalışırken de ayrıca sıcaklık paketi göndermiyoruz.

Firmware’den yapıyı, `0x04` tür sabitini, ona ait boyut kontrollerini, geçersizlik sabitini, 10 saniyelik zamanlayıcıyı ve gönderim kodunu çıkardık. Genel durumun kullandığı sıcaklık kaynakları ve −32768 geçersizlik değeri aynı.

Güncel mesajlar: RUNNING sırasında 100 ms’de bir 13 bayt telemetri, 500 ms’de bir heartbeat ve test durumundan bağımsız 3 saniyede bir 11 bayt genel durum. Pil/ortam sıcaklıkları yalnız genel durumda, ayrı int16_t °C × 10 alanlarında. Telemetriye sıcaklık geri eklenmedi.

[ADR-0013](ADR-0013-separate-temperature-message.md) içindeki ayrı sıcaklık mesajı kararı kalktı; telemetrinin sıcaklıksız 13 bayt olması devam ediyor. [ADR-0014](ADR-0014-status-temperatures.md) içindeki genel durum alanları aynı.

Derleme ve gerçek dispatcher/ISO-TP ile bilgisayardaki kontroller geçti. Eski 10 saniyelik eşikte hiçbir oynatıcı durumunda ayrı sıcaklık gönderimi oluşmadığı kontrol edildi. Kart testi yapılmadı; ClusterPilot alıcısının mevcut telemetri ve genel durum biçimlerine uyarlanması hâlâ gerekiyor.

**Kayıt:** 2026-09-18 · Kabul edildi.

[Genel durum](../03-software/general-status-message.md) · [Telemetri](../03-software/vertex-telemetry-message.md)
