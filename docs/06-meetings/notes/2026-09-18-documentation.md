---
baslik: "2026-09-18 — Dokümantasyon Düzeni"
kategori: "06-meetings"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# 2026-09-18 — Dokümantasyon Düzeni

**Son Güncelleme:** 2026-09-18

## Görüşme özeti

Kullanıcı, bu alanın Tronloop kararları ve dokümantasyonu için kullanılmasını; özellikle haberleşmeler ile mimari notların iyi biçimlendirilmiş ve düzenli tutulmasını istedi.

## Kesinleşen karar

[ADR-0001 — Kararların ve mimari notların kalıcı tutulması](../../07-decisions/ADR-0001-documentation.md).

## Yapılan düzenleme

Mevcut dokümantasyon deposuna karar defteri, karar kayıtları, haberleşme ve mimari çalışma notları eklendi. Sonraki görüşmelerde aynı düzenin izlenmesi için çalışma alanı talimatı oluşturuldu.

## Açık konular

Güncel mimarinin doğrulanması ve mesaj sözleşmelerinin ayrıntılandırılması. Bu görüşmede yeni bir teknik protokol veya bileşen sorumluluğu kararı verilmedi.


## Takip açıklaması — Ana merkez ve geçersiz klasörler

Kullanıcı Defteri Kebir'i ana üs olarak kesinleştirdi; diğer klasörlerin kod kaynakları olduğunu ve `__` ile bitenlerin geçersiz olduğunu belirtti. [ADR-0002](../../07-decisions/ADR-0002-documentation-scope.md) ile kaydedildi; mimari kaynak kapsamı ve kalıcı çalışma talimatları güncellendi.

## Genel mimari ve protokol görüşmesinin başlangıcı

Kullanıcı Cluster, Vertex ve Linux üzerinde çalışan ClusterPilot yapısını; buluttaki MQTT üzerinden veri, komut ve yanıt akışını; gönderilemeyen verilerin SQLite'ta tutulmasını açıkladı. Bu anlatım [ADR-0003](../../07-decisions/ADR-0003-system-overview.md) ile kaydedildi. Haberleşme ayrıntıları için konu listesi çıkarıldı; ilk açık soru Vertex–ClusterPilot yerel protokolüdür.

## CAN/ISO-TP ve mesaj tasarımının kapsamı

Kullanıcı yerel haberleşmenin CAN/ISO-TP olduğunu belirtti ve mevcut mesajların Vertex firmware'inden okunmasını istedi. Taşıma temelinden ziyade mesaj tipleri birlikte yeniden değerlendirilecek. Geçerli firmware kaynakları incelenerek [mevcut mesaj envanteri](../../03-software/vertex-message-inventory.md) oluşturuldu; gözlenen kod ile öneriler ayrıldı.

## Bağımsız test yürütme kararı

Kullanıcı testin Vertex firmware'i tarafından ClusterPilot'a muhtaç olmadan yürütüleceğini kesinleştirdi. [ADR-0004](../../07-decisions/ADR-0004-autonomous-vertex.md) oluşturuldu; mimari ve haberleşme notları güncellendi. Vertex'te kesinti sırasında veri saklama ve güç kesintisi sonrası devam etme davranışları henüz kararlaştırılmadı.

## Vertex'te veri biriktirme kararı

Kullanıcı Vertex'in test verilerini biriktirip bağlantı problemi çözülünce aktaracağını kesinleştirdi. [ADR-0005](../../07-decisions/ADR-0005-vertex-data-buffer.md) oluşturuldu. Depolama ortamı ve kapasitesi, doluluk davranışı, güç kesintisinde koruma ve teslim/silme sözleşmesi açık bırakıldı.

## Veri biriktirme kapsamının düzeltilmesi

Kullanıcı kesintisiz bağlantının normal varsayım olduğunu ve Vertex'in kayıpsız saklama için değil kısa kesintiler için dairesel tampon kullanacağını açıkladı. Dolunca en eski kayıtların üzerine yazılacak; test ve kayıt devam edecek. [ADR-0006](../../07-decisions/ADR-0006-vertex-ring-buffer.md), önceki ADR-0005'in yerine geçti. Asistanın kalıcı teslim onayına kadar koruma önerisi kabul edilmedi.

## Ölçüm zamanı ve saat eşitleme

Kullanıcı ölçüm zamanı ve sıra numarası kullanımını kabul etti; STM32 saatinin Linux zamanı ile periyodik mesajlar üzerinden güncelleneceğini belirtti. [ADR-0007](../../07-decisions/ADR-0007-measurement-time-sequence.md) oluşturuldu. Vertex RTC ve CAN kodunda Unix saniyeleriyle saat ayarlama doğrulandı; Linux gönderim periyodu ve yeni ölçüm zamanının çözünürlüğü açık bırakıldı.

## Ölçüm zamanının çözünürlüğü

Kullanıcı ölçüm zamanı için milisaniye çözünürlüğünü kabul etti. ADR-0007 ve ilgili belgeler güncellendi. Mevcut saniye tabanlı RTC uygulamasının uyarlanması gerekecek; zaman alanının kodlanması ve saniye altı zamanın üretim yöntemi henüz belirlenmedi.

## Sıra numarasının amacı

Kullanıcı sıra numarasının artmasını, yalnızca ayırt edici olacağını belirtti. Test başında sıfırlama önerisi benimsenmedi; ADR-0007 ve ilgili notlar güncellendi. Sayaç genişliği, yeniden başlama ve taşma davranışları henüz seçilmedi.

## Eski dokümantasyonun adlandırılması

Kullanıcının talebiyle eski belgeler Cluster, ClusterPilot ve Vertex adlandırmasına uyarlandı. Sunucu ve ortak fiziksel altyapı bağlama göre ayrıldı. Dosya yolları, şema alanları ve donanım model adları korundu. [Adlandırma rehberi](../../01-project-general/terminology.md) kalıcı referans olarak eklendi.

## Mesajlaşma taslağının derlenmesi

Kullanıcının talebiyle konuşulan ayrıntılar [Mesajlaşma Protokolü — Çalışma Taslağı](../../03-software/communication-notes.md) içinde toplandı. Veri akışı, ölçüm kaydı, dairesel tampon, saat eşitleme ve komut/yanıt akışları diyagramlarla belgelendi. Kesinleşen davranışlar, mevcut kod gözlemleri ve henüz seçilmemiş paket/MQTT ayrıntıları ayrı gösterildi.
