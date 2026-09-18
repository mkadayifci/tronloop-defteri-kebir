---
baslik: "Genel Durum Mesajı — Mevcut Firmware Biçimi"
kategori: "03-software"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Genel Durum Mesajı — Mevcut Firmware Biçimi

**Son Güncelleme:** 2026-09-18

## Güncel tasarım kararı — henüz kodda uygulanmadı

[ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md) ile `charger_enabled` ve `charger_reverse_mode` yerine tek **çalışma modu: idle / şarj / deşarj** seçildi. Senaryo oynatıcı durumu ve charger çalışma modu **ayrı birer bayt** olarak taşınacak. Önceki ortak bit alanı yerleşimi kaldırıldı. Charger modunun sayısal kodları ve firmware'deki kaynak durumu henüz seçilmedi.

### Hedef genel durum yerleşimi — 7 bayt

| Bayt | Bilgi | Boyut |
|---|---|---|
| 0 | Mesaj türü | 1 bayt |
| 1–2 | Pil gerilimi (mV) | 2 bayt |
| 3 | Oynatıcı durumu, `uint8_t` | 1 bayt |
| 4 | Charger çalışma modu: idle / şarj / deşarj, `uint8_t` | 1 bayt |
| 5–6 | İşaretli pil akımı (mA), `int16_t` | 2 bayt |

Toplam **7 bayt**. Akım alanı **−32768…+32767 mA** aralığındadır; pozitif şarj, negatif deşarj anlamına gelir. Charger modunun sayısal kodları henüz seçilmedi. Yeni biçim kodda uygulanmadı; aşağıdaki mevcut paket tablosu ve örnek çözümleme eski **10 baytlık** kod biçimini açıklar.

Farklı mesaj türleri aynı uzunlukta olabilir; `dataLength` seçilen türün uzunluk doğrulaması içindir. Aşağıdaki 10 baytlık tablo **mevcut kodun** biçimidir; yeni kararın uygulanmış hali değildir.

## Kapsam

Bu belge, 2026-09-18 tarihinde Vertex firmware kaynaklarından doğrulanan **GeneralStatusPayload** mesajını açıklar. Mevcut uygulamanın kaydıdır; mesaj tipleri birlikte yeniden tasarlanabilir. TSphere'e giden MQTT mesajının aynı ikili biçimi kullandığı doğrulanmadı; TSphere konu ve içerik şeması henüz seçilmedi.

| Özellik | Mevcut değer |
|---|---|
| Gönderen → Alıcı | Vertex → ClusterPilot |
| Taşıma | CAN/ISO-TP |
| Uygulama mesajı uzunluğu | **10 bayt**; tür alanı dahil |
| Tür alanı | `0x03` (`PAYLOAD_TYPE_GENERAL_STATUS`) |
| Çok baytlı alanların sırası | Little-endian; düşük bayt önce |
| Hedef gönderim aralığı | **3000 ms** |
| Yapı | `GeneralStatusPayload`; packed yapı, 10 bayt için derleme zamanı boyut kontrolü |

10 bayt, ISO-TP'ye verilen uygulama verisinin boyutudur; tek CAN çerçevesinin DLC değeri değildir. [ADR-0010](../07-decisions/ADR-0010-message-type-and-operation-mode.md) uyarınca tür alanı esas alınır; farklı türler aynı uzunlukta olabilir. Mevcut alıcının bu yeni davranışa uyumu henüz doğrulanmadı.

## Alanlar ve bayt yerleşimi

Bayt konumları sıfırdan başlar.

| Bayt | Alan | Tür / boyut | Birim veya değer | Anlam / kaynak |
|---|---|---|---|---|
| 0 | `payload_type` | `uint8_t` / 1 bayt | `0x03` | Genel durum mesajı türü |
| 1–2 | `battery_voltage_mv` | `uint16_t` / 2 bayt | mV | `g_tl_context.dut.voltage_mV` üzerinden pil gerilimi |
| 3 | `scenario_player_state` | `uint8_t` / 1 bayt | Aşağıdaki 0–4 kodları | Doğrudan `g_scenario_player.playerState` |
| 4 | `charger_enabled` | `uint8_t` / 1 bayt | 0: kapalı, 1: açık | Context içindeki şarj etkinlik bayrağı |
| 5 | `charger_reverse_mode` | `uint8_t` / 1 bayt | 0: normal yön, 1: ters yön | Context içindeki ters mod bayrağı |
| 6–9 | `battery_current_ma` | `int32_t` / 4 bayt | mA; pozitif şarj, negatif deşarj | `g_tl_context.dut.current_mA` üzerinden ölçülen pil akımı |

Toplam: **1 + 2 + 1 + 1 + 1 + 4 = 10 bayt**. Bu pakette ayrıca dört baytlık komut başlığı bulunmaz.

### Senaryo oynatıcı durumları

| Değer | Kod | Anlam |
|---|---|---|
| 0 | `SCENARIO_PLAYER_STATE_IDLE` | Boşta |
| 1 | `SCENARIO_PLAYER_STATE_RUNNING` | Senaryo çalışıyor |
| 2 | `SCENARIO_PLAYER_STATE_PAUSED` | Senaryo duraklatılmış |
| 3 | `SCENARIO_PLAYER_STATE_STOPPED` | Senaryo durdurulmuş |
| 4 | `SCENARIO_PLAYER_STATE_ERROR` | Senaryo hata durumunda |

Bu alan `ScenarioPlayerState_t` değeridir; `TlpState` içindeki şarj/deşarj durum kodlarıyla karıştırılmamalıdır.

## Verilerin mevcut uygulamadaki anlamı

- **Gerilim:** Context başlangıçta sıfırlanıyor; incelenen `tl_context.c` içinde gerilim ölçüm güncellemeleri yorum satırında. Paket alanının bulunması gerilimin gerçekten ölçüldüğünü göstermez.
- **Akım:** Context, `TL_FuelGauge_ReadCurrent_mA()` dönüşüyle güncelleniyor. Paket, şarj hedef akımını değil context'teki pil akımını taşıyor.
- **Şarj bayrakları:** Yazılımın context kontrol durumunu gösteriyor; donanımdan okunarak doğrulanmış durum bilgisi değildir.
- **Oynatıcı durumu:** Dispatcher, doğrudan senaryo oynatıcı durumunu bir bayta dönüştürüyor. Heartbeat'in kullandığı context durum alanından farklı kaynaktır.
- **Bu pakette bulunmayanlar:** Ölçüm zamanı, sıra numarası, sıcaklık, SOC, SOH, test kimliği, adım numarası ve hata ayrıntısı. Ölçümlere zaman ve sıra numarası ekleme kararı henüz bu mevcut pakete uygulanmış değildir.

## Gönderim davranışı

3000 ms aralık dolduğunda ISO-TP gönderimi meşgulse genel durum bekler. Bağlantı gönderime uygun olduğunda diğer periyodik paketlerden önce denenir. Gönderim başlatma hatası loglanır; zaman sayacı deneme sırasında güncellendiği için sonraki deneme 3 saniye sonradır. Bu aralık teslim garantisi değildir.

## Örnek çözümleme

Aşağıdaki paket yalnızca alan yerleşimini göstermek için oluşturulmuştur; cihazdan alınmış kayıt değildir. Gerilim güncellemesinin uygulanmış olduğunu ima etmez.

```text
03 74 0E 01 01 00 18 FC FF FF
```

| Alan | Çözümlenen değer |
|---|---|
| Tür | `0x03` |
| Pil gerilimi | `0x0E74` = 3700 mV |
| Oynatıcı | 1 = çalışıyor |
| Şarj etkinliği | 1 = açık (yazılım bayrağı) |
| Ters mod | 0 = kapalı (yazılım bayrağı) |
| Pil akımı | `0xFFFFFC18` = -1000 mA |

## Kaynaklar ve ilişkili belgeler

Çalışma alanı köküne göre kaynak yolları:

- `tronloop-vertex-firmware/Core/Inc/tl_dispatcher.h`: alan türleri, yerleşim, tür kodu ve boyut kontrolü.
- `tronloop-vertex-firmware/Core/Src/tl_dispatcher.c`: alan kaynakları ve gönderim davranışı.
- `tronloop-vertex-firmware/Core/Inc/tl_context.h`: oynatıcı durum kodları ve context alanları.
- `tronloop-vertex-firmware/Core/Src/tl_context.c`: gerilim/akım güncellemeleri.

[Mesajlaşma protokolü](communication-notes.md) · [Mevcut mesaj envanteri](vertex-message-inventory.md) · [Bileşen adlandırması](../01-project-general/terminology.md)

## ISO-TP ile tek CAN çerçevesi

Hedef genel durum uygulama verisi **7 bayttır**. Mevcut `Middlewares/isotp-c/isotp.c`, `send_size < 8` olduğunda Single Frame gönderir; 8 bayt ve üzerindeki uygulama verisi çok çerçeveli gönderime girer. Kısa mesaj da ISO-TP başlığı taşır; ham mesaja dönüştürülmez.

```text
CAN veri alanı: [0x07 ISO-TP başlığı] [7 bayt genel durum verisi]
Toplam:         1 + 7 = 8 bayt
```

Mevcut `ISO_TP_FRAME_PADDING` ayarı etkin olsa da bu pakette fazladan dolguya yer kalmaz ve gerekmez. ISO-TP alıcısının uygulamaya verdiği veri **7 bayt**, tel üzerindeki CAN veri alanı **8 bayt** olur. Ham CAN'a geçmeden tek çerçeveli gönderim mümkündür.

Klasik CAN'ın 8 bayt veri sınırı için [Bosch açıklaması](https://www.bosch-semiconductors.com/products/ip-modules/can-protocols/can-fd/). Kütüphane davranışı yerel kaynak koddan doğrulandı. Firmware'deki mevcut 10 baytlık mesaj henüz değiştirilmedi.
