---
baslik: "Ana Ünite Tasarımı"
kategori: "02-hardware"
durum: "taslak"
son_guncelleme: "2026-02-26"
guncelleyen: "—"
---

# Ana Ünite Tasarımı

Tüm node'ları koordine eden, verileri toplayan ve depolayan merkezi sistem.

## İşlemci / Platform

**BeagleBone Black Rev C — TI AM3358BZCZ**

| Parametre | Değer |
|-----------|-------|
| İşlemci | AM3358BZCZ ARM Cortex-A8 @ 1GHz |
| RAM | 512MB DDR3 |
| Depolama | 4GB eMMC + microSD |
| İşletim Sistemi | Linux (Debian) |
| Real-time | 2x PRU @200MHz (deterministik zamanlama) |
| Yazılım dili | Python |

## İletişim

Node'larla haberleşme **CAN bus** üzerinden yapılır. AM3358 dahili **2x DCAN kontrolcüsü** içerir — harici CAN IC gerekmez.

| Parametre | Değer |
|-----------|-------|
| Protokol | CAN bus (CAN 2.0A / 2.0B — belirlenecek) |
| Baud rate | — kbps |
| Topoloji | Multi-drop bus, her node paralel |
| Sonlandırma | Her iki uçta 120 Ω |
| Node adresleme | Her node'a benzersiz CAN ID |
| Linux CAN stack | SocketCAN + can-utils |

---

**İlgili Dosyalar:** [Node Tasarımı](node-design.md) · [Malzeme Listesi](bill-of-materials.md) · [Yazılım Mimarisi](../03-software/architecture.md)

## Yedeklilik (Redundancy) Mimarisi

Sistem senelerce kesintisiz çalışacak şekilde tasarlanmıştır. Hiçbir tek nokta arızası (single point of failure) sistemi durdurmamalıdır.

### BeagleBone — 2x Aktif/Standby

| Rol | Durum | Görev |
|-----|-------|-------|
| Primary BBB | Aktif | CAN bus yönetimi, veri toplama, depolama |
| Secondary BBB | Hot standby | Primary'yi izler, heartbeat kesilirse otomatik devralır |

Her iki BBB da donanımsal watchdog timer ile korunur.

**Tek CAN Bus:** Tüm node'lar tek bir CAN hattı üzerinden her iki BBB'a da bağlıdır. Primary çökerse secondary aynı hat üzerinden devralır — node'lar geçişi fark etmez. AM3358'in ikinci DCAN kontrolcüsü ileride genişleme veya debug için boşta bekler.

```
NODE'lar ──── CAN Bus (DCAN0) ──┬── Primary BBB (aktif)
                                └── Secondary BBB (standby)
```

### Rezerv Pil Paketi — 3x (N+1)

2 paket aktif, 1 paket bakımda veya yedekte. Bir paket devre dışı kalırsa sistem kalan 2 paketle çalışmaya devam eder. Her paketin bağımsız BMS'i ve 48V bus'a bağlı bidirectional DC-DC regülatörü vardır.

### Node Arızası

Node arızası kabul edilebilir — arızalı node CAN heartbeat kaybıyla tespit edilir, kaydedilir, ilgili test senaryosu ileride tekrarlanır. Diğer node'lar etkilenmez.

### Güç

48V omurga bus'a çift bağımsız güç yolu. BBB'lar ve CAN elektroniği için UPS/tampon kondansatör — güç kesintisinde güvenli kapanma süresi sağlar.
