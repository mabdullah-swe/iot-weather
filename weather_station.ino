/*
  Ultimate IoT Weather Station for Arduino UNO R4 WiFi
  Features: 
  - Modern Glass-morphism UI
  - AJAX Data Fetching (Instant updates, NO page reload)
  - Mobile Responsive
*/

#include <WiFiS3.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

// ------------------- WIFI SETTINGS -------------------
const char ssid[] = "YOUR_WIFI_NAME";      // CHANGE THIS
const char pass[] = "YOUR_WIFI_PASSWORD";  // CHANGE THIS

int status = WL_IDLE_STATUS;
WiFiServer server(80);

// ------------------- SENSOR PINS -------------------
#define DHTPIN 2          
#define DHTTYPE DHT11     
#define MQ135_PIN A0      
#define RAIN_PIN A1       

// ------------------- OBJECTS -------------------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  dht.begin();
  if (!bmp.begin()) {
    Serial.println("BMP180 init failed");
  }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.print("Connected! Open: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    String request = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        
        if (c == '\n') {
          if (currentLine.length() == 0) {
            
            // --- READ SENSORS ---
            float t = dht.readTemperature();
            float h = dht.readHumidity();
            float p = bmp.readPressure() / 100.0F; // hPa
            int aqRaw = analogRead(MQ135_PIN);
            int rainRaw = analogRead(RAIN_PIN);
            
            // Logic: Rain
            String rainStatus = "Dry";
            String rainColor = "#2ecc71"; // Green
            if(rainRaw < 800) { rainStatus = "Light Rain"; rainColor = "#3498db"; } // Blue
            if(rainRaw < 400) { rainStatus = "Heavy Rain"; rainColor = "#e74c3c"; } // Red

            // Logic: Air Quality
            String aqStatus = "Good";
            if(aqRaw > 200) aqStatus = "Moderate";
            if(aqRaw > 400) aqStatus = "Poor";

            // --- API MODE (JSON Data) ---
            if (request.indexOf("GET /data") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: application/json");
              client.println("Connection: close");
              client.println();
              
              // Send data as JSON
              client.print("{");
              client.print("\"temp\":" + String((int)t) + ",");
              client.print("\"hum\":" + String((int)h) + ",");
              client.print("\"press\":" + String((int)p) + ",");
              client.print("\"aq\":\"" + aqStatus + "\",");
              client.print("\"aqRaw\":" + String(aqRaw) + ",");
              client.print("\"rain\":\"" + rainStatus + "\",");
              client.print("\"rainColor\":\"" + rainColor + "\"");
              client.print("}");
              
            } 
            // --- UI MODE (HTML Page) ---
            else {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html; charset=utf-8");
              client.println("Connection: close");
              client.println();
              
              client.println("<!DOCTYPE html><html><head>");
              client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
              
              client.println("<style>");
              client.println("body { font-family: 'Segoe UI', sans-serif; background: linear-gradient(135deg, #1e3c72, #2a5298); color: white; margin: 0; padding: 20px; text-align: center; }");
              client.println(".container { max-width: 600px; margin: auto; }");
              client.println(".header h1 { font-weight: 300; letter-spacing: 2px; margin: 0; }");
              client.println(".time { font-size: 0.9rem; opacity: 0.8; margin-bottom: 20px; }");

              // Glassmorphism Card Style
              client.println(".card { background: rgba(255, 255, 255, 0.15); backdrop-filter: blur(10px); border-radius: 15px; padding: 20px; margin-bottom: 15px; box-shadow: 0 8px 32px 0 rgba(0, 0, 0, 0.2); transition: background 0.5s; }");
              
              // Temp Specifics
              client.println(".temp-display { font-size: 4rem; font-weight: bold; }");
              client.println(".unit { font-size: 1.5rem; vertical-align: super; }");
              
              // Grid for smaller cards
              client.println(".grid { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; }");
              client.println(".info-card { text-align: left; display: flex; align-items: center; }");
              client.println(".icon { font-size: 2rem; margin-right: 15px; }");
              client.println(".label { font-size: 0.8rem; opacity: 0.8; display: block; }");
              client.println(".value { font-size: 1.2rem; font-weight: bold; }");
              
              client.println("</style>");
              client.println("</head><body>");
              
              client.println("<div class='container'>");
              
              client.println("<div class='header'>");
              client.println("<h1>WEATHER STATION</h1>");
              client.println("<p class='time'>Live Updates</p>");
              client.println("</div>");

              // Main Temp Card
              client.println("<div class='card'>");
              client.println("<div>Current Temperature</div>");
              client.println("<div class='temp-display'><span id='tVal'>--</span><span class='unit'>&deg;C</span></div>");
              client.println("<div>Humidity: <span id='hVal'>--</span>%</div>");
              client.println("</div>");

              // Grid Area
              client.println("<div class='grid'>");
              
              // Pressure
              client.println("<div class='card info-card'>");
              client.println("<div class='icon'>⏱</div>");
              client.println("<div><span class='label'>Pressure</span><span class='value'><span id='pVal'>--</span> hPa</span></div>");
              client.println("</div>");

              // Air Quality
              client.println("<div class='card info-card'>");
              client.println("<div class='icon'>🍃</div>");
              client.println("<div><span class='label'>Air Quality</span><span class='value' id='aqVal'>--</span><br><span style='font-size:0.7rem' id='aqRawVal'></span></div>");
              client.println("</div>");
              
              // Rain (Span 2 columns)
              client.println("<div class='card info-card' id='rainCard' style='grid-column: span 2; justify-content: center;'>");
              client.println("<div class='icon'>☔</div>");
              client.println("<div><span class='label'>Precipitation</span><span class='value' id='rVal'>Checking...</span></div>");
              client.println("</div>");

              client.println("</div></div>"); // End Grid & Container

              // --- JAVASCRIPT FOR AJAX ---
              client.println("<script>");
              client.println("function updateData() {");
              client.println("  fetch('/data').then(response => response.json()).then(data => {");
              client.println("    document.getElementById('tVal').innerText = data.temp;");
              client.println("    document.getElementById('hVal').innerText = data.hum;");
              client.println("    document.getElementById('pVal').innerText = data.press;");
              client.println("    document.getElementById('aqVal').innerText = data.aq;");
              client.println("    document.getElementById('aqRawVal').innerText = '(' + data.aqRaw + ')';");
              client.println("    document.getElementById('rVal').innerText = data.rain;");
              client.println("    document.getElementById('rainCard').style.background = data.rainColor;");
              client.println("  });");
              client.println("}");
              client.println("setInterval(updateData, 3000);"); // Run every 3s
              client.println("</script>");
              
              client.println("</body></html>");
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}