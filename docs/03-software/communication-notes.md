---
baslik: "Bileşenler Arası Haberleşme Notları"
kategori: "03-software"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Bileşenler Arası Haberleşme Notları

**Son Güncelleme:** 2026-09-18

## Kaynak ve durum

2026-09-18 tarihli kullanıcı anlatımı esas alınmıştır. Akışlar kullanıcı tarafından belirtilmiştir; Vertex–ClusterPilot taşıma temeli kullanıcı tarafından CAN/ISO-TP olarak belirtildi ve Vertex kaynaklarında doğrulandı. Mevcut mesaj tipleri yeniden tasarıma açıktır; kesinleşmiş yeni protokol kararları değildir. [Genel mimari](architecture-notes.md) · [ADR-0003](../07-decisions/ADR-0003-system-overview.md).

## Haberleşme envanteri

| Kimlik | Gönderen → Alıcı | Yöntem | Amaç | Açık ayrıntı |
|---|---|---|---|---|
| COM-001 | Vertex → ClusterPilot | CAN/ISO-TP | Test verisi ve cihaz yanıtı | Yerel protokol, adresleme, paket biçimi |
| COM-002 | ClusterPilot → Buluttaki MQTT | MQTT | Test verisi ve komut yanıtı | Konular, şema, teslim ve saklama onayı |
| COM-003 | Kullanıcı paneli → Buluttaki MQTT | MQTT üzerinden mantıksal akış | Komut başlatma | Doğrudan erişim veya ara servis, yetkilendirme |
| COM-004 | Buluttaki MQTT → ClusterPilot | MQTT | Komut alma | Abonelik, hedefleme, komut geçerliliği |
| COM-005 | ClusterPilot → Vertex | CAN/ISO-TP | Komutu doğru cihaza iletme | Adres eşleştirmesi, zaman aşımı, yanıt ilişkisi |
| COM-006 | ClusterPilot ↔ Yerel SQLite | Yerel veritabanı | Gönderilemeyen veriyi biriktirme ve sonra gönderme | Yazma/silme zamanı, kapasite, tekrar ve sıra |

**Kesinleşen davranış:** ClusterPilot buluta gönderemediği verileri SQLite'ta biriktirir ve daha sonra gönderir. Komutların veya yanıtların aynı kuyruğa dahil olduğu henüz söylenmedi.

## Protokol görüşmesi için önerilen sıra

Bu bölüm çalışma önerisidir; teknik karar değildir.

| Sıra | Konu | Netleştirilecek noktalar |
|---|---|---|
| 1 | Vertex ↔ ClusterPilot | Fiziksel hat, taşıma protokolü, adresler, paket sınırları |
| 2 | Kimlik ve yönlendirme | Cluster, Vertex, test çalıştırması ve mesaj kimlikleri; kimliklerin benzersizlik kapsamı |
| 3 | MQTT konu düzeni | Telemetri, komut, yanıt, durum/olay ayrımı; yönler ve erişim sınırları |
| 4 | Mesaj şeması | Sürüm, zaman, veri alanları, birimler, örnek mesajlar |
| 5 | Komut yaşam döngüsü | Alındı/kabul edildi/tamamlandı ayrımı, hata ve yanıt eşleştirme |
| 6 | Kesinti ve toparlanma | SQLite kuyruğu, yeniden gönderim, yinelenen mesajlar, sıralama, saklama onayı |
| 7 | Çevrimdışı çalışma | Testin bağlantı kesilince davranışı, bekleyen/eski komutlar, yeniden başlama |

## Açık sorular

- CAN/ISO-TP üzerindeki yeni mesaj tipleri, başlık ve adresleme düzeni nasıl olacak? Mevcut firmware envanteri: [Vertex mesajları](vertex-message-inventory.md).
- Vertex dairesel tamponunun ortamı, kapasitesi, karşılayacağı kesinti süresi ve kayıt kapsamı ne olacak? Dolduğunda en eski kayıtların üzerine yazılacağı kesinleşti.
- Her cluster için bir ClusterPilot mu var? Vertex kimliği cluster içinde mi, tüm sistemde mi benzersiz?
- Panel komutları MQTT'ye doğrudan mı, bir servis üzerinden mi ulaşıyor? Yanıtlar panele nasıl dönüyor?
- Verinin “gönderildi” sayılması için hangi onay yeterli olacak: mesajın MQTT tarafında alınması mı, bulutta kalıcı saklanması mı?
- SQLite kuyruğu yalnızca ölçümleri mi, olayları ve komut yanıtlarını da mı kapsıyor?
- Komut tekrar gelirse, bağlantı kesintisi sonrasında gecikmeli ulaşırsa veya cihaz yanıt vermezse ne yapılacak?

## Mesaj sözleşmesi şablonu

Her mesaj ailesi için: amaç, gönderen/alıcı, konu veya adres, sürüm, alanlar ve türleri, birimler ve sınırlar, tetikleme/sıklık, kimlik ve yanıt eşleştirmesi, teslim davranışı, zaman aşımı, yeniden deneme, yinelenen mesaj davranışı, yetkilendirme, örnek mesaj, ilgili karar ve doğrulama kaynağı.

Bilinmeyen değerler “Belirlenmedi”, henüz kabul edilmemiş örnekler “Öneri” olarak işaretlenecek.


## Tasarım kapsamının netleştirilmesi

Kullanıcı CAN/ISO-TP temelini değiştirmeyi istemediğini, mesaj tiplerini birlikte yeniden kararlaştıracağımızı belirtti. Kodda bulunan mesajlar mevcut uygulama envanteridir; gelecek protokol için bağlayıcı değildir. İlk tasarım önerisi: ölçüm, durum, olay, komut ve komut sonucu ailelerini değerlendirmek; bu sınıflandırma henüz kabul edilmedi.

## Kesinleşen yürütme sorumluluğu

[ADR-0004](../07-decisions/ADR-0004-autonomous-vertex.md): Test adımlarını Vertex firmware’i bağımsız yürütür. Sürekli ClusterPilot komutu veya adım onayı gerekmez. Senaryo aktarımı, başlatma/durdurma ve durum izleme sözleşmeleri bu sorumluluğa göre tasarlanacak; mesaj tipleri henüz kesinleşmedi. Vertex–ClusterPilot kesintisindeki veri kaybı/saklama davranışı, ClusterPilot–bulut arasındaki mevcut SQLite tamponundan ayrı ele alınmalıdır.

## Kesinleşen Vertex dairesel tampon davranışı

[ADR-0006](../07-decisions/ADR-0006-vertex-ring-buffer.md): Sürekli bağlantı normal çalışma varsayımıdır. Vertex kısa kesintiler için test verilerini sınırlı dairesel tamponda tutar. Gönderim kesilince test ve kayıt sürer; tampon dolduğunda gönderilmemiş olsa bile en eski kayıtların üzerine yazılır. Bağlantı düzelince tamponda kalan bekleyen kayıtlar aktarılır. Kayıpsız teslim garantisi yoktur.

Önceki kalıcı teslim onayına kadar veriyi koruma önerisi kabul edilmedi ve bu tasarımın şartı değildir. Aktarım onayı kullanımı, gönderme konumu ve tekrar davranışı ayrıca netleştirilecek. ClusterPilot'un SQLite biriktirme davranışı bu karardan ayrıdır.

## Ölçüm zamanı, sıra ve saat eşitleme

[ADR-0007](../07-decisions/ADR-0007-measurement-time-sequence.md): Ölçüm kayıtları ölçüm zamanı ve sıra numarası taşıyacak. Tamponda bekleyen kaydın ölçüm zamanı aktarım sırasında korunacak. STM32 RTC aktif çalışacak; ClusterPilot Linux/Unix zamanını periyodik mesajlarla ileterek saati güncelleyecek.

Mevcut firmware RTC API'si `uint32_t` Unix saniyeleri kullanıyor; ham CAN üzerinden saat ayarlama yolu mevcut. Linux tarafındaki periyodik gönderim bu incelemede doğrulanmadı. Yeni mesaj biçimi, eşitleme aralığı, ölçüm zamanı çözünürlüğü ve sıra numarasının kapsamı henüz seçilmedi. Başlangıçta firmware saati sabit bir değere kurduğu için ilk eşitleme öncesi zaman geçerliliği de ele alınmalı.
