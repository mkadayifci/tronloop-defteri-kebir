---
baslik: "ADR-0008 — Mesaj türleri için benzersiz veri uzunluğu"
kategori: "07-decisions"
durum: "yerine-yeni-karar-geldi"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0008 — Mesaj türleri için benzersiz veri uzunluğu

**Son Güncelleme:** 2026-09-18

- **Tarih:** 2026-09-18
- **Durum:** Yürürlükten kalktı — [ADR-0010](ADR-0010-message-type-and-operation-mode.md)
- **Kararı veren:** Kullanıcı
- **Kaynak:** Kullanıcının CAN üzerinden mesaj alan diğer yazılımın mesaj türünü `dataLength` üzerinden belirlediğini açıklaması ve farklı türlerin aynı uzunlukta olmamasını istemesi.

## Yürürlük durumu

Kullanıcı tür alanıyla ayrıştırmaya geçilmesini istedi. **Aynı uzunlukta farklı mesaj türleri artık mümkündür.** Aşağıdaki eski karar tarihçe olarak korunur; geçerli kural ADR-0010’dadır.

## Bağlam ve karar

Kullanıcının bildirdiği alıcı davranışı: CAN üzerinden alınan mesajın türü veri uzunluğundan (`dataLength`) bulunuyor. Bu nedenle **farklı mesaj türleri aynı veri uzunluğuna sahip olamaz**.

CAN/ISO-TP mesajı tasarlanırken her türün alıcı tarafından görülen toplam bayt uzunluğu açıkça belgelenmeli ve diğer mesaj türlerinin uzunluklarıyla çakışmadığı kontrol edilmelidir. Tür alanı veya komut kodu bulunması, aynı uzunluğu farklı türler için kullanmaya izin vermez.

## Tasarım kuralları

- Yeni mesaj eklenirken veya mevcut mesajın alanları değiştirilirken tür–uzunluk eşlemesi güncellenir; uzunluk çakışması kabul edilmez.
- Uzunluk hesabı yorumlardan veya alan adlarından değil, tel üzerinde gerçekten gönderilen baytlardan yapılır. Başlık, dolgu ve alan boyutları hesaba katılır; C enum/struct boyutları varsayılmaz.
- Bir tür değişken uzunlukta olacaksa kullanabileceği uzunluklar diğer türlerle kesişemez. Bu koşul sağlanmadan değişken uzunluklu veya toplu kayıt biçimi kesinleştirilmez.
- Birlikte desteklenecek sürümlerde de alıcının gördüğü tür–uzunluk eşlemesi belirsiz olmamalıdır. Uzunluk değişikliği alıcı uyumluluğu açısından değerlendirilir.
- Bu karar CAN tarafındaki ayrıştırma içindir; MQTT mesaj türlerinin de uzunlukla ayrıştırılacağı anlamına gelmez.

## Doğrulama sınırı ve açık noktalar

Alıcının `dataLength` davranışı kullanıcı bilgisi olarak kaydedildi; alıcı kodu bu karar sırasında incelenmedi. ISO-TP ile yeniden birleştirilen uygulama mesajının uzunluğu ile tek CAN çerçevesinin DLC değeri birbirine karıştırılmamalıdır. Alıcıda ölçülen kesin katman ve başlıkların uzunluğa dahil oluşu kaynak koddan doğrulanarak uzunluk tablosuna işlenecek.

Henüz yeni türlere sayısal uzunluk atanmadı. Her yeni tür için ad, yön, alan yerleşimi, toplam uzunluk, desteklenen sürüm ve alıcı eşlemesi birlikte belgelenecek.

## Uygulama durumu

Karar ve dokümantasyon kuralları güncellendi. Firmware veya alıcı yazılım değiştirilmedi; mevcut bütün mesajların bu kurala uyduğu doğrulanmış değildir.

## İlişkili belgeler

[Mesajlaşma protokolü](../03-software/communication-notes.md) · [Mevcut Vertex mesaj envanteri](../03-software/vertex-message-inventory.md)
