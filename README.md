## How the Feeders Work (Panther, Tiny & Chubba)
🎥 **Full build tutorial on YouTube:**  
[Watch the video here](https://youtu.be/dFGdMnJU7kA?si=JuvmQTRAC33L4R3a)

This project is designed for a **multi-cat household** with different dietary needs:

* **Panther** (16 years old) and **Tiny** (14 years old) **share one feeder**.
  Both wear their own RFID tags, so the lid opens for either of them.
* **Chubba** has his **own separate feeder**, because he follows a different diet.
  His feeder only opens for his unique RFID tag.

This setup ensures each cat gets the right food while keeping things fair and stress-free.

If this project helps you or you’d like to support my work, you can:

- ☕ [Buy Me a Coffee](https://www.buymeacoffee.com/honeysada)  
- 💜 [Sponsor on GitHub]( https://github.com/sponsors/Honeysad007)

---

### Pin Map (both feeders use the same wiring)

* **RFID (WL-134, 125kHz)** → SoftwareSerial RX = D2, TX = D3
* **IR sensor** → A1 (analog input)
* **Servo lid** → D9 (PWM control)

---

### Panther & Tiny’s Shared Feeder

* **Authorized tags:** Panther’s tag, Tiny’s tag, plus one extra testing tag.
* **Logic:**

  1. Feeder is closed until a valid RFID tag is scanned.
  2. If Panther or Tiny arrives, the feeder opens smoothly.
  3. The IR sensor keeps the feeder open as long as a cat is detected.
  4. When the cat leaves, the system waits **4 seconds** before closing.
  5. Servo closes gently in small steps for quiet operation.
* **Duplicate scans** are ignored for 3 seconds to prevent rapid reopening.
* **RFID reader** is disabled during servo motion, then re-enabled after movement, to avoid interference.

---

### Chubba’s Special Feeder

* **Authorized tags:** Only Chubba’s RFID tag.
* **Logic:**

  1. Feeder stays closed for all tags except Chubba’s.
  2. When Chubba arrives, the feeder opens and stays open while the IR sensor detects him eating.
  3. Once he leaves, the feeder waits a short delay, then closes quietly.
* This feeder ensures Chubba’s special diet is kept separate and protected from the other cats.

---

### What Makes This System Special

* 🐾 **Multi-cat management**: Supports multiple feeders, each assigned to specific cats.
* 🥣 **Special diet protection**: Chubba’s feeder prevents him from eating Panther & Tiny’s food (and vice versa).
* 🤫 **Cat-friendly motion**: Servo moves in small steps, making the lid quiet and non-threatening.
* ⏱️ **Smart timing**: IR sensor ensures feeders only close after cats are done eating.
* 🛡️ **Noise filtering**: RFID reads are paused during servo moves for stable operation.

Key Parts Used for the project:

Motion Sensor – $9
 https://amzn.to/3Zmx9KU

Servo Motor – $15 (per 1)
 https://amzn.to/44Bu9O6

RFID Tag Reader – $30
 https://amzn.to/43aZNQ1

Small Acrylic Hinges – $7 (per 12 pack)
 https://amzn.to/3YJXrXc

Servo Mount Brackets – $10 (per 2)
 https://amzn.to/3SvQnK3

Servo Arm Horns – $13 (per 4)
 https://amzn.to/3GSV7qQ

Arduino Nano Board (Presoldered, by ELEGOO) – $15 (set of 3)
 https://amzn.to/3GSVcLa

Tag holders for collars
 https://amzn.to/4jQGeTK

RFID Tags (Yellow Round Livestock Style) – $20 (set of 10)
 https://ebay.us/eq4hpd


