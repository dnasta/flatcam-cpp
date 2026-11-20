<p align="right"><a href="README.md">🇪🇸 Español</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.de.md">🇩🇪 Deutsch</a> · <a href="README.fr.md">🇫🇷 Français</a> · <a href="README.it.md">🇮🇹 Italiano</a> · <a href="README.pt-BR.md">🇧🇷 Português</a> · <a href="README.ro.md">🇷🇴 Română</a> · <a href="README.ru.md">🇷🇺 Русский</a> · <a href="README.tr.md">🇹🇷 Türkçe</a></p>

<p align="center">
  <img src="src/gui/resources/images/flatcam_icon256.png" width="140" alt="FlatCAM">
</p>

<h1 align="center">FlatCAM-cpp</h1>

<p align="center">
  CNC veya lazerle baskılı devre kartı üretmek için <strong>FlatCAM 2024.4</strong>'ün C++17, Qt 6 ve GEOS'a yerel (native) portu.
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><img alt="Release" src="https://img.shields.io/badge/release-v1.0.0-b4771b"></a>
  <a href="LICENSE"><img alt="lisans" src="https://img.shields.io/badge/lisans-MIT-2f6f4f"></a>
  <a href="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml"><img alt="CI" src="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml/badge.svg"></a>
  <img alt="macOS" src="https://img.shields.io/badge/macOS-15%2B%20arm64-16202b">
  <img alt="Windows" src="https://img.shields.io/badge/Windows-10%20%7C%2011-0078D4">
  <img alt="Linux" src="https://img.shields.io/badge/Linux-deb%20%7C%20rpm%20%7C%20AppImage-FCC624">
  <img alt="C++" src="https://img.shields.io/badge/C%2B%2B-17-00599C">
  <img alt="Qt" src="https://img.shields.io/badge/Qt-6-41CD52">
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><strong>⬇ v1.0.0'ı indir</strong></a> ·
  <a href="docs/NOTAS_1.0.0.md"><strong>📝 Sürüm notları (İspanyolca)</strong></a> ·
  <a href="https://github.com/dnasta/flatcam-cpp/issues"><strong>🐞 Sorun bildir</strong></a>
</p>

---

## İçindekiler

- [FlatCAM-cpp nedir](#flatcam-cpp-nedir)
- [Ekran görüntüleri](#ekran-görüntüleri)
- [Özellikler](#özellikler)
- [Diller](#diller)
- [Desteklenen biçimler](#desteklenen-biçimler)
- [Kurulum](#kurulum)
- [Hızlı başlangıç](#hızlı-başlangıç)
- [Komut satırı ve otomasyon](#komut-satırı-ve-otomasyon)
- [Kaynak koddan derleme](#kaynak-koddan-derleme)
- [Testler ve doğrulama](#testler-ve-doğrulama)
- [Proje yapısı](#proje-yapısı)
- [Lisans, yazarlar ve teşekkürler](#lisans-yazarlar-ve-teşekkürler)

---

## FlatCAM-cpp nedir

FlatCAM, bir baskılı devre kartının tasarım dosyalarını —**Gerber** katmanları ve **Excellon** delik dosyaları— bir CNC freze, router veya lazer için **G-code**'a dönüştürür: yol yalıtımı, bakır temizleme, delme, kart kesimi ve çok daha fazlası.

FlatCAM-cpp, FlatCAM 2024.4'ün (Python + PyQt5) **C++17**'ye **tam** bir portudur: arayüz için **Qt 6**, geometri için **GEOS** kullanır. Arayüz orijinalin aynısıdır —gerçek orijinal arayüzden üretilmiştir—; aynı menüler, paneller, kısayollar ve ayarlarla gelir ve aynı `.FlatPrj` projelerini okuyup yazar.

**Tek cümleyle:** kartınızı açın, aracı seçin, takım yollarını kontrol edin ve G-code'u kaydedin.

### Neden yerel bir port

| Konu | FlatCAM 2024.4 (Python) | FlatCAM-cpp |
|---|---|---|
| Çalışma | Python 3 + PyQt5 + Shapely + numpy + vispy | Yerel çalıştırılabilir dosya |
| Kurulum | Python ve bağımlılıklarını kurmak | macOS, Windows ve Linux için yükleyiciler |
| Geometri | Shapely | GEOS (yeniden girişli C API) |
| Arayüz | Qt 5 | OpenGL çizim alanlı Qt 6 |
| Hız (gerçek bir kartta bakır temizleme) | 567 sn | 56 sn |
| Orijinalin bilinen hataları | Var | 200'den fazlası düzeltildi (`--original-bugs` orijinal davranışa döner) |

---

## Ekran görüntüleri

<p align="center">
  <img src="docs/images/interfaz.png" alt="Ana pencere" width="90%">
  <br><em>Ana pencere: gerçek bir kartın bakırı ve özellikleri (İspanyolca arayüz).</em>
</p>

<p align="center">
  <img src="docs/images/ncc.png" alt="Bakır temizleme" width="90%">
  <br><em>Bakır temizleme (NCC) aracı.</em>
</p>

<p align="center">
  <img src="docs/images/cncjob.png" alt="G-code" width="90%">
  <br><em>Bakır temizleme, delme ve kesimin G-code'u (boş hareketler sarı).</em>
</p>

<p align="center">
  <img src="docs/images/deutsch.png" alt="Almanca arayüz" width="90%">
  <br><em>Aynı arayüz Almanca, delme aracıyla.</em>
</p>

---

## Özellikler

### Dosyalar

- **Gerber RS-274X**: C/R/O/P açıklıkları, `%AM` makroları, `G36/G37` bölgeleri, `G02/G03` yayları, polarite ve X2 öznitelikleri.
- **Excellon**: takımlar, delikler, oluklar (`G85` ve yönlendirme), biçimler ve sıfırlar, başlıksız dosyalar.
- **G-code**: açma, görüntüleme, düzenleme ve dışa aktarma.
- **SVG, DXF, PDF, HPGL2 ve görüntüler** (PNG, JPG, BMP), Geometri veya Gerber olarak.
- Python FlatCAM'in **`.FlatPrj` projeleri** (LZMA ile sıkıştırılmış, yüzlerce MB olsa bile); orijinal FlatCAM, portun kaydettiği projeleri açar.
- Gerber, Excellon, SVG, DXF, PNG ve PDF olarak **dışa aktarma**.

### CAM araçları

- **Yalıtım**: geçişler, bindirme, V uçlu takımlar, birleşik, kalan malzeme işleme, alana veya çokgene göre.
- **Bakır temizleme (NCC)** ve **Boyama (Paint)**: Standart, Tohum, Çizgiler ve Kombine yöntemleri, kalan malzeme işleme, alan ve referans nesnesi; "lazer çizgileri".
- **Delme**: en iyi sıralama, çok derinlikli delme, delik olarak oluklar; delik ve olukların frezelenmesi.
- **Kesim**: serbest veya dikdörtgen, köprüler, "mouse bites" ve elle köprüler.
- **Çift taraflı PCB**: nokta, kutu veya deliğe göre aynalama ve hizalama delikleri.
- **Ayrıca**: Panelleme, Film, Bakır doldurma (thieving), Lehim pastası, QR kod, Referans işaretleri (fiducial), Köşe işaretleri, Delik çıkarma, Gerber delme, Çıkarma, Optimum, Kural denetimi, Kalibrasyon, Aşındırma telafisi, Nesneleri hizalama, Gerber'i ters çevirme, Hesap makineleri, Dönüşümler, Mesafe ve Otomatik seviyeleme (GRBL, MACH3/4 ve LinuxCNC ile yoklama).
- **Takım veritabanı** ve **hariç tutma alanları**.

### G-code

- Orijinalin **20 son işlemcisi** (`default`, `GRBL_laser`, `grbl_11`, `Marlin`, `Repetier`, `Roland_MDX_20`, `hpgl`, `ISEL_CNC`, `Toolchange_*`, `Z_laser`…), satır satır aynı.
- Çok derinlik, ek kesim, başlangıç ve bitiş kodu parçaları, milimetre veya inç.

### Düzenleyiciler

- **Geometri** (sistem yazı tipleriyle yazılmış metinlerle), **Excellon**, **Gerber** ve **G-code**.

### Arayüz

- FlatCAM 2024.4'ün arayüzü: menüler, araç çubukları, Proje / Özellikler / Araç sekmeleri ve aynı kısayollar.
- Yakınlaştırma, kaydırma, ızgara, ızgaraya yapışma, HUD ve çalışma alanı olan OpenGL çizim alanı.
- Ayarlar (485 seçenek), Temel ve Gelişmiş seviyeler, milimetre veya inç, son dosyalar ve yer imleri.

### Otomasyon

- Gerçek Tcl yorumlayıcısı ve orijinalin **65 komutu** ile **Tcl konsolu**; `.tcl` betikleri.

---

## Diller

Arayüz, orijinalin **9 dilinde** ve aynı çevirilerle gelir: Almanca, İngilizce, İspanyolca, Fransızca, İtalyanca, Brezilya Portekizcesi, Rumence, Rusça ve Türkçe.

- **Sistem dilinde** açılır (dokuz dil arasında değilse İngilizce).
- Değiştirmek için: **Ayarlar → Genel →** dili seçin **→ Seçili Dili Uygula** (uygulama yeniden başlar).
- Açılışta bir dili zorlamak için: `FLATCAM_LANGUAGE` ortam değişkeni (örneğin `FLATCAM_LANGUAGE=tr`).

---

## Desteklenen biçimler

| Biçim | Açma / içe aktarma | Dışa aktarma | Notlar |
|---|:---:|:---:|---|
| Gerber RS-274X | ✅ | ✅ | Açıklıklar, makrolar, bölgeler, polarite |
| Excellon | ✅ | ✅ | Delikler ve oluklar |
| G-code | ✅ | ✅ | 20 son işlemci |
| SVG | ✅ | ✅ | Geometri veya Gerber olarak |
| DXF | ✅ | ✅ | ASCII DXF |
| PDF | ✅ | ✅ | Vektörel içe aktarma; yazdırma ve film |
| HPGL2 | ✅ | — | `hpgl` son işlemcisi çiziciler için kod üretir |
| PNG / JPG / BMP | ✅ | ✅ | Eşik ile vektörleştirme; çizim alanı görüntüsü ve film |
| `.FlatPrj` | ✅ | ✅ | Python FlatCAM ile uyumlu |

---

## Kurulum

Yükleyiciler [**Releases**](https://github.com/dnasta/flatcam-cpp/releases/latest) sayfasındadır.

### macOS

- **macOS 15 veya üstü**, **Apple Silicon** (M1–M4).
- `FlatCAM-1.0.0-macOS-arm64.dmg` dosyasını açın ve **FlatCAM**'i *Uygulamalar* klasörüne sürükleyin.
- İlk seferde: sağ tık → **Aç** (ad hoc imza, noter onaylı değil).
- Qt, GEOS, FreeType ve Tcl dahildir: Homebrew gerekmez.

### Windows

- **Windows 10 (1809 veya üstü) ve Windows 11**, 64 bit.
- `FlatCAM-1.0.0-Windows-x64-setup.exe` (yükleyici) veya `FlatCAM-1.0.0-Windows-x64.zip` (taşınabilir, kurulumsuz).
- "Windows bilgisayarınızı korudu" uyarısı çıkarsa: **Ek bilgi → Yine de çalıştır** (sertifikayla imzalı değildir).

### Linux

| Dağıtım | Dosya | Kurulum |
|---|---|---|
| Ubuntu 24.04+, Debian 13, Linux Mint 22 | `flatcam-cpp_1.0.0_amd64.deb` | `sudo apt install ./flatcam-cpp_1.0.0_amd64.deb` |
| Fedora 41+ | `flatcam-cpp-1.0.0-1.x86_64.rpm` | `sudo dnf install ./flatcam-cpp-1.0.0-1.x86_64.rpm` |
| Diğerleri (glibc 2.39+) | `FlatCAM-1.0.0-Linux-x86_64.AppImage` | `chmod +x FlatCAM-*.AppImage && ./FlatCAM-*.AppImage` |

`.deb` veya `.rpm` ile FlatCAM uygulama menüsünde görünür ve `flatcam` komutuyla açılır.

---

## Hızlı başlangıç

```text
1. Dosya → Aç → Gerber'i Aç      kartın bakırı ve dış hattı
   Dosya → Aç → Excellon'u Aç    delikler
2. Proje içinde bakırı seçin → Özellikler → Yalıtım Oluşturma (veya Bakır Temizleme)
   takım çapı, geçişler ve bindirme → Şekil Oluştur
3. Yeni Geometride: Özellikler → kesme Z'si, ilerleme → CNC İş Nesnesi Oluştur
4. CNC Job'da: CNC Kodunu Kaydet
5. Delikler: Delme → CNC İş Nesnesi Oluştur
6. Dış hat: PCB Kesimi → köprüler → Şekil Oluştur
```

> **İşlemeden önce:** G-code'u, iş parçası sıfırını ve Z yüksekliğini kontrol edin. 0,1 mm'lik bir hata kartı bozabilir.

---

## Komut satırı ve otomasyon

```bash
FlatCAM kart-B_Cu.gbr kart-Edge_Cuts.gbr kart.drl   # dosyaları açar (uzantıya göre)
FlatCAM proje.FlatPrj                                # bir proje açar
FlatCAM betik.tcl                                    # bir Tcl betiği açar ve çalıştırır
FlatCAM --original-bugs                              # tam olarak orijinal FlatCAM gibi davranır
```

Çalıştırılabilir dosya `FlatCAM`'dir (macOS'ta `/Applications/FlatCAM.app/Contents/MacOS/FlatCAM`), Windows'ta `FlatCAM.exe`, Linux'ta `flatcam`.

### Tcl konsolu

**Komut Satırı** ile açılır (`S` tuşu). Örnek:

```tcl
open_gerber kart-B_Cu.gbr -outname bakir
isolate bakir -dia 0.2 -passes 2 -overlap 10 -combine True -outname bakir_iso
cncjob bakir_iso -dia 0.2 -z_cut -0.1 -z_move 2 -feedrate 120 -pp default -outname bakir_cnc
write_gcode bakir_cnc bakir.nc

open_excellon kart.drl -outname delikler
drillcncjob delikler -drillz -1.8 -travelz 2 -feedrate_z 100 -pp default -outname delikler_cnc
write_gcode delikler_cnc delikler.nc
```

`help` 65 komutu listeler, `help <komut>` her birini açıklar.

---

## Kaynak koddan derleme

| Bağımlılık | Kullanım |
|---|---|
| C++17 derleyicisi ve CMake 3.20+ | AppleClang, GCC veya MSVC 2022 |
| GEOS 3.11+ | Geometri |
| Qt 6.4+ | Arayüz (Widgets, OpenGLWidgets, Svg, PrintSupport; SerialPort isteğe bağlı) |
| Tcl 8.6 veya 9 | Tcl konsolu |
| liblzma | Sıkıştırılmış `.FlatPrj` projeleri |
| FreeType (isteğe bağlı) | Geometri düzenleyicisinde yazı tipli metinler |
| GoogleTest | Testler (yoksa CMake indirir) |

**macOS (Homebrew)**

```zsh
brew install cmake ninja geos qt tcl-tk freetype xz googletest
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
open build/src/gui/FlatCAM.app
tools/package_macos.sh            # build/dist/FlatCAM-1.0.0-macOS-arm64.dmg
```

**Linux (Ubuntu / Debian)**

```bash
sudo apt install build-essential cmake ninja-build libgeos-dev liblzma-dev libgtest-dev \
  qt6-base-dev qt6-svg-dev qt6-serialport-dev libgl-dev tcl-dev libfreetype-dev
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
tools/package_linux.sh            # .deb ve AppImage (Fedora'da: .rpm)
```

**Windows (Visual Studio 2022 + vcpkg + Qt)**

```bat
vcpkg install geos liblzma freetype gtest tcl --triplet x64-windows
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
cmake --build build
ctest --test-dir build --output-on-failure
pwsh tools/package_windows.ps1    # yükleyici (Inno Setup) ve .zip
```

CMake seçenekleri: `-DFLATCAM_BUILD_TESTS=OFF`, `-DFLATCAM_BUILD_DOCS=OFF`, `-DFLATCAM_BUILD_GUI=OFF` (yalnızca çekirdek).

**Kod belgeleri** (Doxygen, İspanyolca): `cmake --build build --target docs` → `build/doxygen/html/index.html`.

---

## Testler ve doğrulama

- **2.854 test** (GoogleTest): çekirdek ve arayüz. CI'da (GitHub Actions) Linux ve Windows üzerinde ve iki GEOS sürümüyle çalışır.
- Beklenen değerler **elle yazılmadı**: `tests/tools/` içindeki kâhinler FlatCAM'in **orijinal** kodunu (ayrıştırıcılar, araçlar, son işlemciler, düzenleyiciler, Tcl konsolu ve PyQt5 arayüzü) çalıştırır ve sonuçlarını kaydeder.
- Gerçek bir kartla **FlatCAM 2024.4 çalıştırılabilir dosyasına** karşı: kesim **bayt bayt aynı** çıkar, Paint ve NCC aynı takım yollarını, delme aynı delikleri verir.
- Orijinalin hataları uygulamada düzeltilmiştir; testler orijinalle karşılaştırmaya devam etmek için onları yeniden açar ve her düzeltmenin kendi testi vardır.

---

## Proje yapısı

```text
src/core/        Qt'siz çekirdek: şekiller (GEOS), geometri, yaylar, dönüşümler
src/parsers/     Gerber, Excellon, SVG, DXF, PDF, HPGL2, görüntüler ve yazı tipleri
src/tools/       CAM araçları (yalıtım, NCC, Paint, kesim, çift taraf, panelleme, film...)
src/cam/         G-code, son işlemciler, delik sıralaması, seviyeleme
src/editors/     Düzenleyici modelleri (Qt'siz)
src/io/          .FlatPrj projeleri ve dışa aktarma
src/gui/         Qt 6 arayüzü (generated/: orijinal arayüzden üretilmiş)
tests/           GoogleTest, gerçek veriler ve Python kâhinleri
tools/           Arayüz, çeviri ve paket üreticileri
reference/       FlatCAM 2024.4 kaynakları (salt okunur)
```

---

## Lisans, yazarlar ve teşekkürler

FlatCAM-cpp, FlatCAM gibi **MIT lisansı** ile dağıtılır. Bkz. [`LICENSE`](LICENSE) ve [`NOTICE`](NOTICE).

Bir dizi FlatCAM sürümünün portudur:

1. **FlatCAM**, **Juan Pablo Caram** tarafından oluşturuldu (2014–2016). <http://flatcam.org>
2. **FlatCAM Beta** (8.9xx ve 2024.x), **Marius Adrian Stanciu** ve katkıda bulunanlar (2019–2024).
3. **FlatCAM β 2024.4**, bakımı **vika-sonne** tarafından yapıldı (2023–2024).
4. **FlatCAM-cpp**: Qt 6 ve GEOS ile C++17'ye port.

**Teşekkürler:** FlatCAM'in yazarları ve çevirmenleri; [GEOS](https://libgeos.org) (OSGeo); [Qt](https://www.qt.io) (The Qt Company); [Tcl](https://www.tcl-lang.org); [FreeType](https://freetype.org); Lincoln Loop'un [qrcode](https://github.com/lincolnloop/python-qrcode) kütüphanesi (C++'a portlandı).

<p align="center"><sub>C++17 · Qt 6 · GEOS · macOS · Windows · Linux · v1.0.0</sub></p>
