# IoT Weather Station (Arduino UNO R4 WiFi)

A modern, responsive, and real-time weather monitoring system powered by the Arduino UNO R4 WiFi.

Unlike traditional Arduino web servers that reload the page constantly, this project uses AJAX to fetch data instantly in the background, providing a smooth, app-like experience. The dashboard features a clean interface that works well on both desktop and mobile devices.

<img width="963" height="685" alt="Image" src="https://github.com/user-attachments/assets/3ea6da91-3814-4317-9aee-1b843448415d" />

## Features

* **Real-Time Monitoring:** Live updates for Temperature, Humidity, Pressure, Air Quality, and Rain status.
* **Modern UI:** Custom HTML/CSS dashboard with a translucent design.
* **Zero Page Reloads:** Uses AJAX (Asynchronous JavaScript) to update sensor values every 3 seconds without refreshing the browser.
* **Mobile Responsive:** Optimized layout for smartphones and tablets.
* **Smart Alerts:** Dynamic color changes based on weather conditions (e.g., the Rain card turns Blue or Red when it rains).
* **JSON API:** The Arduino serves raw JSON data at the /data endpoint for developers.

## Hardware Required

| Component | Description |
| :--- | :--- |
| **Arduino UNO R4 WiFi** | The microcontroller used for the project. |
| **DHT11** | Temperature & Humidity Sensor. |
| **BMP180** | Barometric Pressure Sensor (I2C). |
| **MQ135** | Air Quality / Gas Sensor. |
| **Rain Sensor** | YL-83 Module. |
| **Breadboard & Wires** | For connections. |

## Pin Configuration

### MQ135 Sensor (Air Quality)
| MQ135 Pin | Arduino UNO R4 Pin | Description |
| :--- | :--- | :--- |
| VCC | 5V | Power |
| GND | GND | Ground |
| AOUT | Analog Pin (A0) | Reads continuous air quality value |
| DOUT | Analog Pin (A3) | Reads digital threshold (or secondary analog) |

### Rain Sensor (YL-83)
| Rain Sensor Pin | Arduino UNO R4 Pin | Description |
| :--- | :--- | :--- |
| VCC | 5V | Power |
| GND | GND | Ground |
| AOUT | Analog Pin (A1) | Reads rain intensity (Light/Heavy) |
| DOUT | Digital Pin (D3) | Digital trigger (Wet/Dry) |

### DHT11 Sensor (Temperature/Humidity)
| DHT11 Pin | Arduino UNO R4 Pin |
| :--- | :--- |
| VCC | 5V |
| GND | GND |
| DATA | Digital Pin (D2) |

### BMP180 Sensor (Pressure)
| BMP180 Pin | Arduino UNO R4 Pin |
| :--- | :--- |
| VCC | 5V |
| GND | GND |
| SDA | SDA (Near USB) |
| SCL | SCL (Near USB) |

**Note on Connections:** * **BMP180:** For the Arduino UNO R4 WiFi, it is often best to use the dedicated SDA/SCL pins located near the USB connector.
* **Rain Sensor:** This configuration uses the Digital Output (D3). Ensure your code is set to read digital values for rain detection.

## Installation & Setup

1.  **Install Libraries**
    Open the Arduino IDE Library Manager and install the following:
    * DHT sensor library (by Adafruit)
    * Adafruit BMP085 Library (by Adafruit)
    * WiFiS3 (Built-in for R4, check for updates)

2.  **Configure Network**
    Open the .ino file and update your WiFi credentials:
    ```cpp
    const char ssid[] = "YOUR_WIFI_NAME";
    const char pass[] = "YOUR_WIFI_PASSWORD";
    ```

3.  **Upload**
    Select **Arduino UNO R4 WiFi** as your board and upload the code.

4.  **Run**
    Open the Serial Monitor (9600 baud). Copy the IP address shown (e.g., 192.168.1.15) and open it in your web browser.

## Project Structure

* **WeatherStation.ino**: The main firmware containing C++ logic, HTML, CSS, and JavaScript.
* **circuit_diagram.png**: Visual wiring guide.
* **docs/**: Additional documentation and datasheets.

## How It Works

1.  **The Server:** The Arduino hosts a lightweight web server on Port 80.
2.  **The Frontend:** When you visit the IP, it sends a single HTML file containing the CSS and JavaScript.
3.  **The Data Loop:**
    * Every 3 seconds, the browser's JavaScript sends a background request to /data.
    * The Arduino replies with a JSON packet containing the sensor values.
    * The browser updates the text on the screen instantly.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
