*🔋 TRONLOOP — Batarya Kapasite Kayıp İzleme Sistemi*

---
# Proje Tanımı

**Kurum:** Biruni Üniversitesi — Elektrik Elektronik Mühendisliği

Farklı iklim koşullarında sürekli şarj/deşarj döngülerine maruz kalan bataryaların uzun vadeli kapasite kayıplarını izleyen, akademik yayın üretmeyi hedefleyen donanım araştırma sistemi.

---

## Geliştirme Yaklaşımı

Proje, karmaşıklığını yönetmek için **iki aşamalı** olarak geliştirilmektedir.

### Faz 1 — Temel Test Sistemi

İklimlendirme olmadan, oda sıcaklığında çalışan temel şarj/deşarj ve kapasite ölçüm sistemi. Bu fazın amacı donanım, firmware ve veri toplama altyapısını iklim değişkenini devreye almadan doğrulamaktır.

**Kapsam:**
- Node donanımı (BQ25756 + BQ34Z100 + STM32L476)
- Şarj/deşarj döngüsü ve kapasite ölçümü
- CAN haberleşme ve BeagleBone veri toplama
- Temel test protokolü

### Faz 2 — İklim Kontrol Entegrasyonu

Faz 1 üzerine eklenen kapalı döngü iklimlendirme sistemi. Peltier soğutma, PTC ısıtma, nem kontrolü ve 50 node'a ölçekleme bu fazda gerçekleştirilir.

**Kapsam:**
- Peltier soğutma + kondenzasyon tuzağı + hot side ısı eşanjörü
- 50W trim PTC ısıtıcı
- ASA/PETG manifold + Armaflex yalıtım
- Sıcaklık kontrollü test senaryoları (-20°C – +40°C)

---

## Hedef Çıktılar

- Uzun vadeli batarya degradasyon veri seti
- Akademik yayınlar (iklim × kapasite kaybı ilişkisi)

---

**İlgili Dosyalar:** [Ekip](team.md) · [Yol Haritası](roadmap.md)
