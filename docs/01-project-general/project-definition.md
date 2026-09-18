---
baslik: "Proje Tanımı"
kategori: "01-project-general"
durum: "taslak"
son_guncelleme: "2026-09-18"
guncelleyen: "Codex"
---

# Proje Tanımı

**Son Güncelleme:** 2026-09-18

**Kurum:** Biruni Üniversitesi — Elektrik Elektronik Mühendisliği

Farklı kimyadaki pilleri farklı koşullarda test edip kapasite kayıplarını izlemek istiyoruz. Bunun için senaryoları tekrar tekrar çalıştırabileceğimiz, ölçümleri düzenli toplayabileceğimiz bir pil test sistemi geliştiriyoruz. Uzun vadede bu verilerden bilimsel yayınlar ve pil ömrünü tahmin etmekte kullanılabilecek veri setleri çıkarmayı hedefliyoruz.

---

## Bileşenler

Pili test eden birime Vertex diyoruz. Vertex’leri Cluster’lar halinde topluyoruz. ClusterPilot da bu birimlerle bulut arasında veri ve komut taşıyan Linux sunucusu. [Adlandırma rehberi](terminology.md).

## Geliştirme Yaklaşımı

Her şeyi aynı anda çözmeye çalışmıyoruz. Önce temel test sistemi, ardından iklim kontrolü: **iki aşamada** ilerliyoruz.

### Faz 1 — Temel Test Sistemi

İlk aşamada oda sıcaklığında çalışacağız. Önce şarj/deşarj, kapasite ölçümü, firmware ve veri toplama birlikte düzgün çalışsın istiyoruz. İklim kontrolü daha sonra gelecek.

**Kapsam:**

- Vertex donanımı (BQ25756 + BQ34Z100 + STM32L476)
- Şarj/deşarj döngüsü ve kapasite ölçümü
- CAN haberleşme ve BeagleBone veri toplama
- Temel test protokolü

### Faz 2 — İklim Kontrol Entegrasyonu

Temel sistem oturduktan sonra kapalı döngü iklimlendirmeyi ekleyeceğiz. Bu aşamanın planında Peltier soğutma, PTC ısıtma, nem kontrolü ve 50 Vertex’e çıkma var.

**Kapsam:**

- Peltier soğutma + kondenzasyon tuzağı + hot side ısı eşanjörü
- 50W trim PTC ısıtıcı
- ASA/PETG manifold + Armaflex yalıtım
- Sıcaklık kontrollü test senaryoları (-20°C – +40°C)

---

## Hedef Çıktılar

- Uzun vadeli batarya degradasyon veri seti
- EV sürüş profilleriyle eğitim verisi üretme fikri; kullanımını henüz netleştirmedik.
- Akademik yayınlar (iklim × kapasite kaybı ilişkisi)
- Pillerin kalan ömrünü tahmin edecek modeller için veri seti.

---

**İlgili Dosyalar:** [Ekip](team.md) · [Yol Haritası](roadmap.md)
