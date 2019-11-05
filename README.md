# satranc
A little chess game with C

    settings.txt Konfigürasyonu:
      1- settings.txt her zaman exe ile aynı konumda olmalıdır.
      2- Örneğine uygun biçimde değiştirilmelidir.
      3- Yapı her satıra ait iki özellikten oluşmaktadır.
        1.özellik değişken ismini
        2.öellik ise değişken değerini
        temsil etmektedir.
        Kullanılabilen değişken isimleri ve değerlerinin ne işe yaradığı aşağıda verilmiştir:
          SETTING_TEST: Test modülünü aktif eder. Test modülü hakkında daha detaylı bilgi daha sonra eklenecektir.
          SETTING_EXPORTING: Oyunda yapılacak hamlelerin dışarı aktarılmasını sağlayar.
          SETTING_LOAD_FILE: Oyunun ana tahtasının yükleneceği belge ismini temsil eder.
          SETTING_MAKE_FILE: Oyunun ana tahtası yüklendikten sonra bu tahta üzerinde yapılacak hareketleri temsil eder.
                             Hareketler, henüz oyun başlamadan tahta üzerinde yapılır.
                             Not: SETTING_EXPORTING dosyası ile aynı isimde olamaz!.
                             Hareketler sistemdeki sırayla aynı sırada olmalıdır. (void ChangePlayer())
                             SETTING_EXPORTING dosyası ile (farklı isimde olduğu sürece) doğrudan uyumludur.
          SETTING_PRINT_AVAIBLE_PIECES: Oyuncunun oynayabileceği taşların ekranda gösterilmesini sağlar. 0: açar, 1: kapatır.
          SETTING_PRINT_AVAIBLE_MOVE: Oyuncunun belirlediği taşı oynayabileceği yerlerin ekranda gösterilmesini sağlar. 0: açar, 1: kapatır.
          SETTING_AUTO_PLAY_1_MOMENT: Eğer oyuncunun seçtiği taşın gidebileceği sadece bir yer var ise hamle tipi seçmeden direkt gitmesini sağlar. 0: açar, 1: kapatır.

      SETTING_EXPORTING ve SETTING_MAKE_FILE Dosyası Konfigürasyonu: 4 veriden oluşur:
        1: Taşın bulunduğu koordinatın x değeri
        2: Taşın bulunduğu koordinatın y değeri
        3: Taşın hareket edeceği koordinatın x değeri
        4: Taşın hareket edeceği koordinatın y değeri

      SETTING_LOAD_FILE Dosyası Konfigürasyonu: 4 veriden oluşur:
        1: Taşın bulunduğu koordinatın x değeri
        2: Taşın bulunduğu koordinatın y değeri
        3: Taşın tipi (enum PieceTypes)
        4: Taşın sahibi olan oyuncu (enum PlayerTypes)
