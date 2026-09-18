---
baslik: "Tronloop Bileşen Adlandırması"
kategori: "01-project-general"
durum: "onaylandı"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Tronloop Bileşen Adlandırması

**Son Güncelleme:** 2026-09-18

Kaynak: Kullanıcının Cluster, ClusterPilot ve Vertex tanımları ve eski dokümantasyonun bu adlarla güncellenmesi talebi.

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

Mevcut dosya yolları, kod sembolleri ve şema alanları bu adlandırma çalışmasıyla yeniden adlandırılmaz. Örneğin `node-design.md`, `node_id` ve `nodes` eski teknik adlar olarak kalabilir; bunların açıklamalarında Vertex kullanılır. Bu adların yeni protokole aktarılacağı anlamı çıkmaz.

Eski belgelerde adların güncellenmesi, bu belgelerdeki donanım, veritabanı, yedeklilik veya mesaj biçimi tercihlerinin yeniden onaylandığı anlamına gelmez. Güncel kararlar için [Karar ve Mimari Defteri](decision-hub.md) esas alınır.

Sunucu adı için [ADR-0009](../07-decisions/ADR-0009-tsphere-name.md) geçerlidir; MQTT hizmetin/protokolün adıdır.
