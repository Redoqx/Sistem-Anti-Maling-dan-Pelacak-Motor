# TubesEmbedded
Pengembangan sistem keamanan sepeda motor
Dikembangkan oleh:
- Ridho Liwardana 119140038
- Nurul Aulia Larasati 119140008
- Nesa Oktavia 119140091
- Sophia Nouriska 119140027

Cara membangun sistem:
1. Install hal-hal berikut ini:
    - Arduino IDE
    - ESP32 Board       https://github.com/espressif/arduino-esp32
    - Library Blynk     https://downloads.arduino.cc/libraries/github.com/blynkkk/Blynk-1.0.1.zip
    - Library RFID      https://github.com/pablo-sampaio/easy_mfrc522
    - Library tinyGPS   https://github.com/mikalhart/TinyGPSPlus/releases
2. Susun perangkat berdasarkan WiringDiagram_bb.png
    * Pastikan Output yang dihasilkan dari Stepdown converter adalah 5V
3. Buat project baru pada Blynk dan atur Project baru seperti pada gambar BlynkSetup.jpg dengan ketentuan:
    - Push Button Alarm terhubung dengan V0
    - Map terhubung dengan V1
    - Push Button Ganti Kartu terhubung dengan GP27
    - LED Alarm Set terhubung dengan V3
    - LED Machine Access State terhubung dengan V4
4. Upload Code ke DOIT ESP32 Devkit
    * Pastikan anda telah memilih doit esp 32 pada opsi board
5. You are ready to go
