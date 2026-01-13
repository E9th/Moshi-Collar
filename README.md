# Moshi Collar 🐱🌡️ (Cat Heatstroke Monitor)

**Moshi Collar** คือโปรเจกต์ปลอกคออัจฉริยะสำหรับติดตามสุขภาพและความปลอดภัยของน้องแมว โดยเน้นไปที่การป้องกันโรคลมแดด (Heatstroke) ระบบจะทำการวัดอุณหภูมิ ความชื้น และระยะห่าง แล้วส่งข้อมูลแบบ Real-time ไปยัง Firebase เพื่อแสดงผลบนหน้าเว็บไซต์ พร้อมระบบแจ้งเตือนด้วยเสียงที่ตัวปลอกคอเมื่อสภาพอากาศอยู่ในเกณฑ์อันตราย

## 📋 คุณสมบัติหลัก (Features)

* **Real-time Monitoring:** แสดงค่าอุณหภูมิ ความชื้น และความเสี่ยง Heatstroke บนหน้าเว็บไซต์
* **Heatstroke Risk Calculation:** คำนวณความเสี่ยงการเป็นลมแดดจากค่าอุณหภูมิและความชื้นสัมพันธ์
* **Alert System:**
* **สถานะปกติ (Comfort/Normal):** ไฟ LED ดับ, Buzzer เงียบ
* **สถานะร้อน (Hot):** แจ้งเตือนสถานะบนเว็บเป็นสีส้ม
* **สถานะอันตราย (Danger):** แจ้งเตือนสถานะบนเว็บเป็นสีแดง และ **ส่งเสียงเตือน (Buzzer)** ที่ปลอกคอทันที


* **Distance Tracking:** วัดระยะห่างด้วยเซนเซอร์อัลตราโซนิก (Ultrasonic)

## 🛠️ อุปกรณ์ที่ต้องใช้ (Hardware Requirements)

อ้างอิงจากไฟล์ `Cat_Collar.ino`:

1. บอร์ด ESP8266 (NodeMCU)
2. เซนเซอร์วัดอุณหภูมิและความชื้น DHT11
3. เซนเซอร์วัดระยะทาง HC-SR04
4. Active Buzzer
5. สายจั๊มเปอร์และแหล่งจ่ายไฟ

## 🔌 การต่อวงจร (Wiring Diagram)

| อุปกรณ์ (Component) | ขาบนอุปกรณ์ (Pin) | ขาบน ESP8266 (Pin) |
| --- | --- | --- |
| **DHT11** | Signal | D2 |
| **HC-SR04** | Trig | D5 |
| **HC-SR04** | Echo | D6 |
| **Buzzer** | Positive (+) | D7 |

## ⚙️ การติดตั้งและใช้งาน (Installation)

### ส่วนที่ 1: Hardware (Arduino IDE)

1. ติดตั้ง Library ที่จำเป็นใน Arduino IDE:
* `FirebaseESP8266`
* `DHT sensor library`


2. เปิดไฟล์ `Cat_Collar.ino`
3. แก้ไขค่า Config WiFi และ Firebase ในส่วนต้นของไฟล์ให้ถูกต้อง:
```cpp
#define FIREBASE_HOST "YOUR_FIREBASE_URL"
#define FIREBASE_AUTH "YOUR_FIREBASE_SECRET"
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

```
4. Upload โค้ดลงบอร์ด ESP8266

### ส่วนที่ 2: Web Interface

1. ไฟล์เว็บประกอบด้วย `index.html`, `style.css`, และ `script.js`
2. ในไฟล์ `script.js` ได้มีการตั้งค่า Firebase Config ไว้แล้ว (API Key, Database URL)
* *หมายเหตุ: หากต้องการเปลี่ยน Database ต้องแก้ไขตัวแปร `firebaseConfig` ในไฟล์ `script.js` ให้ตรงกับโปรเจกต์ของคุณ*

3. เปิดไฟล์ `index.html` ผ่าน Web Browser เพื่อดูข้อมูลได้ทันที

## 🌡️ เกณฑ์การแจ้งเตือน (Thresholds)

ระบบใช้เกณฑ์ดังนี้ในการตัดสินใจส่งเสียงเตือนและเปลี่ยนสถานะ:

| สถานะ (Status) | เงื่อนไขอุณหภูมิ (°C) | เงื่อนไขความชื้น (%) | การทำงาน |
| --- | --- | --- | --- |
| **Comfort** (สบาย) | 24.0 - 28.0 | 40.0 - 55.0 | ปกติ |
| **Hot** (ร้อน) | ≥ 29.0 | ≥ 55.0 | แจ้งเตือนบนเว็บ (สีส้ม) |
| **Danger** (อันตราย) | ≥ 33.0 | ≥ 65.0 | **Buzzer ดัง**, แจ้งเตือนสีแดง |

## 📂 โครงสร้างไฟล์ (File Structure)

* `Cat_Collar.ino` - โค้ดควบคุมหลักสำหรับบอร์ด ESP8266
* `index.html` - หน้าแสดงผลหลัก
* `script.js` - สคริปต์เชื่อมต่อ Firebase และอัปเดตหน้าเว็บ
* `style.css` - ไฟล์ตกแต่งหน้าเว็บ
