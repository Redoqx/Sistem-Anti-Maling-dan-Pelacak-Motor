# TubesEmbedded
Pengembangan sistem keamanan sepeda motor
Dikembangkan oleh:
    - Ridho Liwardana 119140038
    - Nurul Aulia Larasati 119140008
    - Nesa OKtavia 119140091
    - Sophia Nouriska 119140027

Cara membangun sistem:
1. Install hal-hal berikut ini:
    - Arduino IDE
    - Library Blynk
    - Library RFID
    - Library tinyGPS
2. Susun perangkat berdasarkan WiringDiagram_bb.png
    * Pastikan Output yang dihasilkan dari Stepdown converter adalah 5V
3. Buat project baru pada Blynk dan atur Project baru seperti pada gambar BlynkSetup.jpg dengan ketentuan:
    - Push Button Alarm terhubung dengan V0
    - Map terhubung dengan V1
    - Push Button Ganti Kartu terhubung dengan GP27
    - LED Alarm Set terhubung dengan V3
    - LED Machine Access State terhubung dengan V4
4. Upload Code ke DOIT ESP32 Devkit
