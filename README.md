# ML-Based Temperature Monitoring System with IR Control

An IoT-based smart temperature monitoring and control system using ESP32, SX1278 LoRa modules, and a neural network hosted on Raspberry Pi 4B. The system predicts optimal control actions for IR-based devices using real-time sensor data.

## 📆 Project Structure

```
ML-Based-temperature-monitoring-system/
├── esp32s/
│   ├── lora_sender.ino
│   └── lora_receiver.ino
├── raspi_4b_server/
│   ├── raspi_app.py
│   ├── train_model.py
│   └── nn_model.h5
├── LICENSE
├── README.md
└── .gitignore
```
## system_architecture

![image](https://github.com/user-attachments/assets/c092c42b-8b3f-462f-bc4f-74f35808cb32)


## ✅ Requirements

### 🔧 Hardware Components

* 2 × ESP32 Dev Boards
* 2 × SX1278 LoRa Modules (433 MHz)
* 1 × DHT22 Temperature & Humidity Sensor
* 1 × Raspberry Pi 4B
* 1 × IR Transmitter Module (optional)
* Breadboard, jumper wires, USB cables

### 💻 Software Dependencies

**On Raspberry Pi:**

```bash
pip install flask tensorflow numpy
```

**On Arduino IDE (for ESP32):**

* LoRa by Sandeep Mistry
* DHT sensor library
* Adafruit Unified Sensor
* ArduinoJson
* WiFi (built-in for ESP32)

## 📁 Wireless Communication (SX1278 LoRa)

This project uses the SX1278 LoRa transceiver module for long-range, low-power wireless communication between ESP32 devices. It operates at 433 MHz and communicates via SPI.

* **Node 1**: Sends DHT22 sensor data over LoRa.
* **Node 2**: Receives data and forwards it to Raspberry Pi via Wi-Fi.

## 🔌 Pin Connections

### 🄽 Node 1 (ESP32 + DHT22 + SX1278)

| SX1278 Pin | ESP32 Pin |
| ---------- | --------- |
| VCC        | 3.3V      |
| GND        | GND       |
| MISO       | GPIO12    |
| MOSI       | GPIO13    |
| SCK        | GPIO14    |
| NSS (CS)   | GPIO15    |
| RESET      | GPIO16    |
| DIO0       | GPIO5     |

| DHT22 Pin | ESP32 Pin |
| --------- | --------- |
| VCC       | 3.3V      |
| GND       | GND       |
| DATA      | GPIO2     |

### 🄽 Node 2 (ESP32 + SX1278)

| SX1278 Pin | ESP32 Pin |
| ---------- | --------- |
| MISO       | GPIO19    |
| MOSI       | GPIO23    |
| SCK        | GPIO18    |
| NSS        | GPIO5     |
| RESET      | GPIO14    |
| DIO0       | GPIO26    |

## ⚙️ How the System Works

1. **Node 1 (Sender):** Reads temperature and humidity via DHT22 and sends it via SX1278 LoRa.
2. **Node 2 (Receiver):** Receives LoRa packet and forwards it via HTTP POST to Raspberry Pi.
3. **Raspberry Pi:** Flask API receives data, loads a neural network model, predicts optimal control temperature.
4. **Node 2:** Displays prediction or controls an IR device (like an AC) using an IR transmitter.

## 🧠 ML Model (Neural Network)

* Trained using `train_model.py` on Raspberry Pi
* **Input:** Temperature & Humidity
* **Output:** Predicted AC/IR device temperature setting
* **Saved as:** `nn_model.h5`

## ⚠️ Important Notes

* SX1278 uses 3.3V only – do **NOT** connect to 5V.
* Both ESP32s and Raspberry Pi must be on the **same Wi-Fi network**.
* Update Wi-Fi credentials and Flask server IP in `lora_receiver.ino`
* Upload `lora_sender.ino` to Node 1 and `lora_receiver.ino` to Node 2
* Run `raspi_app.py` on Raspberry Pi **after** training and saving `nn_model.h5`

## 🛠️ Future Improvements

* Add OLED display to Node 2 for offline viewing
* Extend ML model with additional environmental parameters
* Add IR feedback loop using receiver module

## 📄 License

This project is licensed under the **MIT License**.

## 👨‍💻 Author

Built and maintained by **Gnanu** — exploring the edge of embedded intelligence, IoT, and automation.
Feel free to connect via [GitHub](https://github.com/Gnanu97)
📞 **Phone**: +91-6385456690
📧 **Email**: [ggnaneshwar126@gmail.com](mailto:ggnaneshwar126@gmail.com)
