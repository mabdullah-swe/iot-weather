# IoT Weather Station (Arduino UNO R4 WiFi)

A modern, responsive, and real-time weather monitoring system powered by the Arduino UNO R4 WiFi.

Unlike traditional Arduino web servers that constantly reload the page, this project utilizes AJAX to fetch data instantly in the background, providing a smooth, app-like experience. The dashboard features a clean interface that works well on both desktop and mobile devices.

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
| **Rain Sensor** | YL-83 Module (Analog). |
| **Breadboard & Wires** | For connections. |

## Pin Configuration

| Sensor | Sensor Pin | Arduino Pin |
| :--- | :--- | :--- |
| **DHT11** | DATA | D2 |
| **BMP180** | SDA | SDA (Near USB) |
| **BMP180** | SCL | SCL (Near USB) |
| **MQ135** | A0 | A0 |
| **Rain Sensor** | A0 | A1 |

**Note:** Ensure all sensors are powered correctly (5V/GND) and your breadboard power rails are not split.

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
