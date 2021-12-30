# TubesEmbedded
Sistem Keamanan Sepeda Motor
Sistem ini akan memantau posisi motor serta melacak terjadinya tindak pencurian sepeda motor.
Sistem terkoneksi ke ponsel pengguna via aplikasi blynk dan jaringan Wi-Fi.
Sistem akan mengenali pemilik sepeda motor berdasarkan NFC tag yang dimiliki oleh penggunanya.
Anda dapat melacak posisi, mengganti NFC tag dengan yang baru dan menyalakan alarm melalui smartphone anda.


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

*Note: This project is an undone project
In the next update we'll make the system connect to blynk via GSM module
