---
baslik: "İklim İzleme Sistemi"
kategori: "02-hardware"
durum: "taslak"
son_guncelleme: "2026-02-26"
guncelleyen: "—"
---
*🔋 TRONLOOP — Batarya Kapasite Kayıp İzleme Sistemi*

---
# İklim İzleme Sistemi

Her node, batarya yaşlanması üzerindeki iklim etkisini analiz edebilmek için ortam koşullarını elektriksel ölçümlerle eş zamanlı olarak kaydeder.

---

## İklim Kontrol Mimarisi

### Genel Yaklaşım

Merkezi Peltier modülü (ana ünitede) iki ayrı hava kanalı besler: biri soğuk taraftan, diğeri sıcak taraftan. Her node, bu iki kanaldan aldığı hava akışını bağımsız valfler aracılığıyla karıştırarak hedef ortam sıcaklığını dinamik olarak oluşturur.

```mermaid
flowchart TD
    subgraph ANA["Ana Ünite"]
        P["Peltier Modülü"]
        EX["Egzoz Çıkışı (Dışarı)"]
    end

    P -->|Soğuk Kanal| SV["Soğuk Valf"]
    P -->|Sıcak Kanal| HV["Sıcak Valf"]

    subgraph NODE["Node"]
        SV
        HV
        SV & HV --> MIX["Karışım Odası"]
        MIX -->|"hava pilden geçer"| BAT["Batarya Bölmesi"]
        BAT --> TMP["TMP117 ölçüm · PID kontrol"]
        TMP --> CV["Çek Valf"]
    end

    CV -->|Egzoz Kanalı| EX
```

### Valf Kontrolü

Her node iki bağımsız oransal valf içerir — biri soğuk kanala, biri sıcak kanala. İki valf birlikte kontrol edilerek hem toplam hava akışı hem de karışım oranı ayarlanabilir.

| Parametre | Açıklama |
|-----------|----------|
| Kontrol türü | PWM kontrollü oransal servo valf (her kanal için ayrı) |
| Geri bildirim | TMP117 ortam sıcaklığı ölçümü |
| Algoritma | PID — MCU (STM32L476) üzerinde çalışır |
| Bağımsızlık | Her node kendi sıcaklık hedefini bağımsız tutar |

**İki valfli yaklaşımın avantajı:** Yalnızca karışım oranı değil, toplam debi de kontrol edilebilir. Hızlı sıcaklık geçişlerinde her iki valf açılarak debi artırılır; kararlı durumda valfler kısılarak enerji tasarrufu sağlanır.

### Kontrol Döngüsü

```mermaid
flowchart LR
    T["TMP117 ölçüm"] --> P["PID hesap"]
    P --> SV["Soğuk valf<br/>PWM güncelle"]
    P --> HV["Sıcak valf<br/>PWM güncelle"]
    SV & HV --> L["Döngü tekrar<br/>~1 sn"]
    L --> T
```

### Egzoz Sistemi

Her node bölmesinin çıkışında bir **çek valf (check valve)** bulunur. Egzoz havası buradan tek yönlü olarak merkezi egzoz kanalına verilir; kanal ana ünite üzerinden dışarıya bağlanır. Çek valf, ana kanaldan geri akış olması durumunda komşu node'ların birbirini kirletmesini önler.

| Bileşen | Görev |
|---------|-------|
| Çek valf (node çıkışı) | Geri akışı engeller, node bölmesini izole eder |
| Merkezi egzoz kanalı | Tüm node egzozlarını ana üniteye taşır |
| Ana ünite egzoz çıkışı | Havayı dışarıya atar |

### Pilden Geçen Hava Akışı

Hava karışım odasından doğrudan pil yüzeyinin üzerinden geçerek egzoza ulaşır. Bu **zorla konveksiyon (forced convection)** düzenlemesi kasıtlı olarak tercih edilmiştir:

- Hedef sıcaklıktaki hava sürekli pil üzerinden aktığı için ısı transferi doğal konveksiyona göre çok daha hızlı ve homojen gerçekleşir
- Pil, hedef sıcaklığa daha kısa sürede ulaşır ve o sıcaklıkta sabit kalır
- Ticari iklim test odalarının çalışma prensibiyle birebir örtüşür
- Hava hızı çok yüksek tutulmamalıdır; aksi hâlde pil yüzeyinde lokal sıcaklık gradyanı oluşabilir

### Tasarım Notları

- Soğuk ve sıcak kanallar ısı kaybını önlemek için yalıtılmalıdır
- Yoğuşmayı önlemek için soğuk kanal iç yüzeyi nem geçirmez malzemeyle kaplanmalıdır
- Peltier boyutlandırması en kötü senaryoya göre yapılmalıdır: tüm node'ların eş zamanlı maksimum soğutma talebi
- Node bölmesi hava sızdırmaz tasarlanmalı; tek giriş (karışım odası) ve tek çıkış (egzoz çek valf) noktası olmalıdır

---

## Boru ve Manifold Sistemi (50 Node)

### Yapı

50 node, 5 adet 10'lu gruba bölünür. Her grup bir alt manifolda bağlanır; alt manifoldlar ana hattan beslenir.

```mermaid
flowchart TD
    P["Peltier\n(Ana Ünite)"] -->|"125mm izolasyonlu HVAC flex"| AM["Ana Hat"]
    AM --> M1["Alt Manifold A\n10 node"]
    AM --> M2["Alt Manifold B\n10 node"]
    AM --> M3["Alt Manifold C\n10 node"]
    AM --> M4["Alt Manifold D\n10 node"]
    AM --> M5["Alt Manifold E\n10 node"]
    M1 & M2 & M3 & M4 & M5 -->|"25mm flex hortum"| N["Node (×50)"]
    N -->|"Egzoz kanalı"| EM["Egzoz Toplama\n(aynı yapı, ters yön)"]
    EM --> OUT["Dışarı"]
```

> Sistem iki kez kurulur: biri soğuk kanal, biri sıcak kanal için. Egzoz tarafı 25mm → 63mm → 125mm olarak toplar.

### Boyutlar

| Bileşen | Çap | Malzeme | Not |
|---------|-----|---------|-----|
| Ana hat | 125mm | İzolasyonlu HVAC flex | Soğuk + sıcak için ayrı |
| Alt manifold giriş | 63mm | 3D baskı PETG | 10 node başına 1 adet |
| Alt manifold çıkış | 10 × 25mm | 3D baskı PETG | 150mm aralıklı |
| Alt manifold boyu | 1500mm | 3D baskı PETG | 10 × 150mm node aralığı |
| Node bağlantısı | 25mm | İzolasyonlu HVAC flex | Maks 1.5m |
| Duvar kalınlığı | 3mm | PETG | Manifold gövdesi |

### Debi ve Basınç

| Parametre | Değer |
|-----------|-------|
| Node başına debi | 10 L/min |
| Alt manifold toplam debi | 100 L/min |
| Ana hat toplam debi | 500 L/min |
| Sistem basınç kaybı | ~80 Pa |
| Fan gereksinimi | Min. 500 L/min @ 80 Pa statik basınç |

### Basınç Tahliye Valfi

Tüm node valflerinin eş zamanlı kapanması durumunda kanal içinde basınç birikmesini önlemek için ana hatta bir **basınç tahliye valfi** bulunmalıdır. Ayarlı basınç aşıldığında otomatik açılarak fazla havayı ana üniteye geri verir.

### Manifold Tasarım İpuçları (Onshape / 3D Baskı)

- İç çap boyunca sabit kesit (plenum tasarımı) → tüm çıkışlara eşit basınç
- Çıkış fittingleri 45° açılı yerleştirilirse akış dağılımı iyileşir
- Soğuk manifold iç yüzeyi pürüzsüz olmalı; yoğuşma birikintisi oluşmaması için en alçak noktaya tahliye deliği eklenebilir
- PETG yeterli; sıcak kanal 60°C'yi geçmeyeceği için PLA kullanılmamalı

---

**İlgili Dosyalar:** [Node Tasarımı](node-design.md) · [Ana Ünite](main-unit.md) · [Veri Toplama](../03-software/data-collection.md)
