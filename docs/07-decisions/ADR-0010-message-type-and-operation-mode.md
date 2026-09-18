---
baslik: "ADR-0010 — Tür alanıyla ayrıştırma ve birleşik çalışma modu"
kategori: "07-decisions"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0010 — Tür alanıyla ayrıştırma ve birleşik çalışma modu

**Sonraki değişiklik:** [ADR-0014](ADR-0014-status-temperatures.md) ile genel duruma iki sıcaklık eklendi; paket 11 bayt oldu. Aşağıdaki 7 bayt/tek çerçeve anlatımı önceki düzen.

Mesaj tipini uzunluğundan tahmin etmiyoruz; **tür alanından okuyoruz**. İki farklı mesaj aynı uzunlukta olabilir. `dataLength` bize yalnızca o tür için beklediğimiz kadar veri gelip gelmediğini söylüyor. Bu karar [ADR-0008](ADR-0008-unique-message-length.md) yerine geçti.

Alıcı önce türü okuyacak kadar veri var mı diye bakacak. Ardından tür kodunu ve o türün uzunluğunu kontrol edip alanları okuyacak. Tür bilinmiyorsa veya boyut yanlışsa başka bir mesajmış gibi yorumlamayacak. Alan boyutlarını, kodları ve sürümleri açıkça yazacağız; C enum/struct boyutlarına güvenerek ilerlemeyeceğiz. Alıcı kodu henüz bu düzene geçirilmedi.

## Genel durumu nasıl taşıyoruz?

Şarj etkinliği ve ters mod için iki ayrı bayrak yerine tek bir çalışma modu var:

| Kod | Mod |
|---|---|
| 0 | Idle |
| 1 | Şarj |
| 2 | Deşarj |

Oynatıcı durumu ayrı. Örneğin senaryo çalışıyor olabilir ama o an bekleme adımındadır; charger modu idle görünür. İki bilgiyi **ayrı birer baytta** tutuyoruz.

Toplam boyut: tür (1) + gerilim (2) + oynatıcı durumu (1) + charger modu (1) + akım (2) = **7 bayt**. Bu pakette zaman ve sıra numarası yok.

Akım mA cinsinden `int16_t`: −32768…+32767 mA. Pozitif şarj, negatif deşarj. Context içindeki değişkenin `int32_t` kalması sorun değil; pakete yazmadan önce aralığı kontrol ediyoruz. Sığmıyorsa o tur genel durumu göndermiyor, hata logluyoruz.

Modu context bayraklarından çıkarıyoruz: önce reverse, sonra enabled kontrolü. Reverse açıksa deşarj, yalnız enabled açıksa şarj, ikisi de kapalıysa idle. Bu yazılımın kontrol durumu; donanımdan geri okunmuş fiziksel durum değil.

## Neden 7 bayt?

ISO-TP’nin tek çerçevesinde bir bayt başlıktan sonra 7 bayt uygulama verisi kalıyor. Genel durum tam buraya sığıyor; ham CAN’a geçmeye gerek yok. CAN veri alanı toplam 8 bayt oluyor.

Önce oynatıcı ve charger durumunu aynı baytta bit alanlarıyla taşımayı düşünmüştük. Akımı 2 bayta indirdiğimizde o ara tasarım 6 bayttı. Sonra durumları ayrı baytlara aldık; son biçim 7 bayt.

Firmware bu biçimi gönderiyor. Debug derlemesi ve bilgisayarda gerçek dispatcher/ISO-TP koduyla paket kontrolleri geçti. Alıcıyı güncellemek ve kart üzerinde test etmek hâlâ gerekiyor.

**Kayıt:** 2026-09-18 · Kabul edildi.

[Genel durumun alanları](../03-software/general-status-message.md) · [Haberleşme notları](../03-software/communication-notes.md)
