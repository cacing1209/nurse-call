
---

# ![Arduino Logo](https://upload.wikimedia.org/wikipedia/commons/4/4e/Arduino_Logo.png) Nurse Call System by CCNG

Sistem Nurse Call ini menggunakan **Arduino Mega 2560** untuk memantau tombol panggil pasien hingga 32 tempat tidur. Status panggilan ditampilkan di LCD dan RGB LED, dengan buzzer sebagai indikator suara opsional.

---

## Features

* Memantau **32 patient call buttons**
* Status ditampilkan via **20x4 I2C LCD**
* **RGB LED** sebagai indikator status
* **Buzzer** untuk peringatan suara (opsional)

---

## Arduino Mega 2560 Pinout Details

### Patient Call Buttons (Inputbutton)

Setiap tempat tidur memiliki tombol khusus pada pin digital berikut:

```cpp
const uint8_t Inputbutton[SizeButton] = {
  22, 23, 24, 25, 26, 27, 28, 29, 30,
  31, 32, 33, 34, 35, 36, 37,
  38, 39, 40, 41, 42, 43, 44, 45,
  46, 47, 48, 49, 50, 51, 52, 53
};
```

**Catatan:**

* Menggunakan pin digital 22–53.
* Pin ini tidak mendukung interrupt eksternal, cocok untuk polling dengan `digitalRead()`.

### Buzzer

* Pin: A3
* Digunakan untuk peringatan suara. Bisa dinyalakan/mati atau dikontrol dengan `tone()`.

### LCD 20x4 (I2C)

* SDA: Pin 20
* SCL: Pin 21
* Komunikasi via I2C, cukup dua pin. Library yang direkomendasikan: `LiquidCrystal_I2C`.

### RGB LED

* Red: Pin 12 (PWM)
* Green: Pin 11 (PWM)
* Blue: Pin 13 (PWM)
* Digunakan untuk menampilkan status dengan kombinasi warna. Intensitas bisa dikontrol dengan `analogWrite()`.

---

### Components Used

* Arduino Mega 2560
* 20x4 I2C LCD
* 32x Push Buttons
* RGB LED (3-pin)
* Buzzer
* Resistor, jumper wires, breadboard atau PCB

---

### How It Works

1. Pasien menekan tombol panggil.
2. Sistem mendeteksi sinyal dari pin terkait.
3. RGB LED menyala sebagai indikator panggilan.
4. Buzzer berbunyi sebagai peringatan (opsional).

---
