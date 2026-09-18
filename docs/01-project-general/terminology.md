---
baslik: "Tronloop Bileşen Adlandırması"
kategori: "01-project-general"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Tronloop Bileşen Adlandırması

**Son Güncelleme:** 2026-09-18

İsimler birbirine karışmasın diye aynı parçaya her yerde aynı adı veriyoruz. 2026-09-18 itibarıyla kullandığımız adlar şöyle:

| Ad | Kapsam | Belgelerde kullanım |
|---|---|---|
| **Cluster** | Vertex birimlerini barındıran test grubu ve ortak altyapısı | Grup, ortak güç/iklim altyapısı ve fiziksel bütün |
| **ClusterPilot** | Linux üzerinde çalışan, Vertex verilerini toplayıp buluta ileten ve komutları yönlendiren sunucu | Sunucu, veri toplama ve iletişim sorumlulukları |
| **Vertex** | Test senaryosunu bağımsız yürüten pil test birimi | Test donanımı, firmware ve ölçüm kaynağı |
| **TSphere** | Bulut sunucusu | MQTT hizmetini barındıran sunucu; bulut tarafındaki mesajlaşma hedefi |

## Eski ifadelerin karşılıkları

- “Node” yerine **Vertex** kullanılır.
- “Ana ünite” sunucuyu anlatıyorsa **ClusterPilot**, ortak fiziksel donanımı anlatıyorsa **Cluster altyapısı** kullanılır.
- BeagleBone bir donanım/platform adıdır; ClusterPilot'un eş anlamlısı değildir. Eski donanım önerilerinde model adı korunur.

## Teknik adlar ve belge geçmişi

Metinleri güncellerken dosya yollarını ve kod adlarını topluca değiştirmiyoruz. Bu yüzden `node-design.md`, `node_id` veya `nodes` gibi eski adlara hâlâ rastlayabiliriz; açıklamada Vertex diyoruz. Yeni protokolde hangi alan adlarını kullanacağımız ayrı konu.

Eski belgelerde yalnızca adı düzeltmiş olmamız, oradaki donanım ve veritabanı seçimlerini de kesinleştirmiyor. Bugün nerede olduğumuza [karar defterinden](decision-hub.md) bakıyoruz.

Bulut sunucusu TSphere; MQTT ise haberleşme hizmeti. [İsim kararı](../07-decisions/ADR-0009-tsphere-name.md).
