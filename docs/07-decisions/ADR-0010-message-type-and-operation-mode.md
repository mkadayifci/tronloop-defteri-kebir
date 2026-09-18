---
baslik: "ADR-0010 — Tür alanıyla ayrıştırma ve birleşik çalışma modu"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0010 — Tür alanıyla ayrıştırma ve birleşik çalışma modu

**Son Güncelleme:** 2026-09-18

- **Durum:** Kabul edildi
- **Tarih:** 2026-09-18
- **Yerine geçtiği karar:** [ADR-0008](ADR-0008-unique-message-length.md).

## Mesaj türünü belirleme

Alıcı mesajı **mesaj türü alanına** göre ayrıştıracak. Farklı türler aynı veri uzunluğuna sahip olabilir. `dataLength` tür seçmek için değil, seçilen türün beklenen paket uzunluğunu doğrulamak için kullanılacak. Tür alanı okunmadan önce yeterli veri bulunduğu kontrol edilir; ardından tür tanımı ve gerekli uzunluk doğrulanarak alanlar okunur. Bilinmeyen tür veya geçersiz uzunluk başka bir tür olarak tahmin edilmez.

Gerçek tel biçimindeki alan boyutları açıkça tanımlanmalı; C enum/struct boyutları varsayılmamalıdır. Tür kodlarının ve desteklenen sürümlerin eşlemesi ayrıca belgelenecek. Bu karar mevcut alıcının zaten böyle çalıştığı iddiası değildir; hedef ayrıştırma davranışıdır.

## Genel durumdaki çalışma modu

`charger_enabled` ve `charger_reverse_mode` ayrı alanları yerine tek bir **çalışma modu** kullanılacak:

| Mod | Anlam |
|---|---|
| Idle | Şarj/deşarj çalışması yok |
| Şarj | Pil şarj ediliyor |
| Deşarj | Pil deşarj ediliyor |

Senaryo oynatıcı durumu ayrı bir bilgidir ve korunur; çalışan bir senaryo bekleme adımında idle olabilir. Çalışma modunun sayısal kodları, bit yerleşimi ve firmware'deki doğrulanmış kaynak durumu henüz seçilmedi. Eski iki bayraktan doğrudan güvenilir fiziksel durum türetildiği varsayılmaz.

## Durum alanları — ayrı birer bayt

**Oynatıcı durumu 1 bayt**, **charger çalışma modu 1 bayt** olarak ayrı taşınacak. Önceki aynı baytta bit alanları kullanma kararı bu düzenlemeyle değiştirildi. Charger çalışma modu idle / şarj / deşarj anlamını korur; eski iki bağımsız şarj bayrağına geri dönülmez.

Hedef genel durum boyutu: tür (1) + gerilim (2) + oynatıcı durumu (1) + charger çalışma modu (1) + işaretli akım (2) = **7 bayt**. Ölçüm zamanı ve sıra numarası bu genel durum yerleşimine dahil değildir.

## Akım alanı — 2 bayt

Pil akımı mA cinsinden **`int16_t` (2 bayt)** olarak taşınır. Değer aralığı −32768…+32767 mA; pozitif şarj, negatif deşarj anlamına gelir. Bu karar context içindeki dahili akım değişkeninin boyutunu değiştirmeyi gerektirmez.

## Taşıma ve açık konular

Mevcut ISO-TP kütüphanesi 7 bayt uygulama verisini bir bayt ISO-TP başlığıyla tek klasik CAN çerçevesinde taşır. Hedef genel durum bu sınıra sığar; ham CAN'a geçmek gerekmez. Uygulama verisi 7 bayt, CAN veri alanı 8 bayttır.

Uygulama sırasında mod kodları 0 idle, 1 şarj, 2 deşarj olarak seçildi. Reverse bayrağına öncelik verilir, ardından enabled kontrol edilir; kaynak donanım geri okuması değil yazılım context durumudur. Akım aralık dışındaysa genel durum paketi o tur atlanır ve hata loglanır; sessiz daraltma yapılmaz.

## Kararın gelişimi

Önce iki durumun aynı baytta bit alanları olarak taşınması seçildi. Akım 2 bayta indirildiğinde bu ara tasarım 6 bayttı. Son durumlar ayrı birer bayta ayrıldı; geçerli hedef **7 bayt** oldu.

## Uygulama durumu

Vertex firmware’i 7 baytlık biçime geçirildi. Debug derlemesi ve bilgisayar üzerinde gerçek dispatcher/ISO-TP koduyla paket kontrolleri başarılı. Alıcı yazılım değiştirilmedi; 7 baytlık biçime uyarlanması gerekir. Kart üzerinde test yapılmadı.

[Genel durum mesajı](../03-software/general-status-message.md) · [Mesajlaşma protokolü](../03-software/communication-notes.md)
