---
baslik: "ADR-0008 — Mesaj türleri için benzersiz veri uzunluğu"
kategori: "07-decisions"
durum: "yerine-yeni-karar-geldi"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# ADR-0008 — Mesaj türleri için benzersiz veri uzunluğu

**Bu kuralı bıraktık.** Artık türü mesajın kendi tür alanından okuyoruz; farklı türler aynı uzunlukta olabiliyor. Güncel karar [ADR-0010](ADR-0010-message-type-and-operation-mode.md).

İlk yaklaşımda alıcı, mesaj tipini `dataLength` üzerinden buluyordu. Bu nedenle iki ayrı mesajın aynı uzunlukta olmaması gerekiyordu. Tür alanı olsa bile uzunlukların çakışmasına izin vermiyorduk.

Eski tasarımda her mesaj için tür–uzunluk tablosu tutacak, alan değişince bu tabloyu da güncelleyecektik. Değişken uzunluklu mesajların kullandığı boyutlar da başka türlerle kesişmeyecekti. Birden fazla sürüm destekleniyorsa aynı kontrol sürümler arasında da yapılacaktı.

Uzunluğu yorum satırından değil gerçekten gönderilen baytlardan hesaplamak gerekiyordu: başlık, dolgu ve alan boyutları dahil. C enum/struct boyutları varsayılmayacaktı. ISO-TP’nin birleştirdiği mesaj uzunluğuyla tek CAN çerçevesinin DLC değeri de birbirine karıştırılmayacaktı. Bu yaklaşım yalnızca CAN tarafı içindi; MQTT’ye aynı kuralı taşımamıştık.

Bu karar alınırken alıcı kodunu henüz incelememiştik. Yeni türlere uzunluk atanmamış, firmware ve alıcı değiştirilmemişti. Her mesajın adı, yönü, alanları, toplam uzunluğu ve desteklediği sürüm birlikte yazılacaktı. Kaydı burada tutmamızın nedeni eski düşünceyi görebilmek; yeni mesaj tasarlarken bu kurala dönmüyoruz.

**Kayıt:** 2026-09-18 · Yerine ADR-0010 geçti.

[Mesajlaşma protokolü](../03-software/communication-notes.md) · [Mesaj envanteri](../03-software/vertex-message-inventory.md)
