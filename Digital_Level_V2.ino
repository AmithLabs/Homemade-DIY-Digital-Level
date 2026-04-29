/*
  ---------------------------------------------------------
  Project         : WiFi Smart Digital Level V2
  Youtube Channel : @AmithLabs
  Author          : Amith Wijekoon
  ---------------------------------------------------------

  FINAL WORKING VERSION

  Features:
  - OLED live angle display
  - WiFi Web Dashboard
  - AJAX real-time update (no page refresh)
  - Live angle + level status
  - Professional mobile UI

  WiFi:
  SSID     : Your Wifi SSID (You can use mobile Hotspot)
  Password : Your Wifi Password

  Open in browser:
  http://192.168.1.10
  ---------------------------------------------------------
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include <math.h>

/*
  ---------------------------------------------------------
  WiFi Settings
  ---------------------------------------------------------
*/

const char* ssid = "Your Wifi SSID";
const char* password = "Your Wifi Password";

IPAddress local_IP(192, 168, 1, 10);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

/*
  ---------------------------------------------------------
  OLED Settings
  ---------------------------------------------------------
*/

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

/*
  ---------------------------------------------------------
  Objects
  ---------------------------------------------------------
*/

MPU6050 mpu;
ESP8266WebServer server(80);

/*
  ---------------------------------------------------------
  Global Variables
  ---------------------------------------------------------
*/

float angleX = 0.0;
String levelStatus = "PERFECT LEVEL";
String statusColor = "#00ff66";

/*
  ---------------------------------------------------------
  Create Web Page
  ---------------------------------------------------------
*/

String createWebPage() {

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Smart Digital Level</title>

<style>
body{
  margin:0;
  padding:20px;
  background:#050b12;
  font-family:Arial;
  color:white;
  text-align:center;
}

.container{
  max-width:420px;
  margin:auto;
}

.title1{
  font-size:22px;
  font-weight:bold;
  color:white;
}

.title2{
  font-size:42px;
  font-weight:bold;
  color:#00ff66;
  margin-bottom:10px;
}

.subtitle{
  color:#8fdfff;
  margin-bottom:20px;
}

.card{
  background:#0c1420;
  border:1px solid #0d5f3d;
  border-radius:20px;
  padding:20px;
  margin-bottom:20px;
  box-shadow:0 0 20px rgba(0,255,100,0.08);
}

.small-title{
  font-size:18px;
  color:#00ff66;
  margin-bottom:10px;
}

.angle{
  font-size:78px;
  font-weight:bold;
  color:#00ff66;
}

.axis{
  color:#bbbbbb;
  margin-top:8px;
  font-size:16px;
}

.status{
  font-size:32px;
  font-weight:bold;
  margin-top:10px;
}

.footer{
  margin-top:10px;
  color:#bbbbbb;
  font-size:15px;
}
</style>

<script>
function updateData() {
  fetch("/data")
    .then(response => response.json())
    .then(data => {

      document.getElementById("angle").innerHTML =
        data.angle + "&deg;";

      let statusElement =
        document.getElementById("status");

      statusElement.innerHTML =
        data.status;

      statusElement.style.color =
        data.color;
    });
}

setInterval(updateData, 200);
window.onload = updateData;
</script>

</head>
<body>

<div class="container">

<div class="title1">SMART</div>
<div class="title2">DIGITAL LEVEL</div>
<div class="subtitle">WiFi Inclinometer</div>

<div class="card">
<div class="small-title">LIVE ANGLE</div>

<div class="angle" id="angle">
0.0&deg;
</div>

<div class="axis">
X-AXIS TILT
</div>
</div>

<div class="card">
<div class="small-title">LEVEL STATUS</div>

<div class="status" id="status">
PERFECT LEVEL
</div>
</div>

<div class="footer">
WiFi Connected • IP : 192.168.1.10
</div>

</div>
</body>
</html>
)rawliteral";

  return html;
}

/*
  ---------------------------------------------------------
  Root Page Handler
  ---------------------------------------------------------
*/

void handleRoot() {
  server.send(200, "text/html", createWebPage());
}

/*
  ---------------------------------------------------------
  JSON Data Handler
  ---------------------------------------------------------
*/

void handleData() {

  String json = "{";
  json += "\"angle\":\"" + String(angleX, 1) + "\",";
  json += "\"status\":\"" + levelStatus + "\",";
  json += "\"color\":\"" + statusColor + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

/*
  ---------------------------------------------------------
  Setup
  ---------------------------------------------------------
*/

void setup() {

  Serial.begin(115200);

  /*
    Start I2C
    SDA = D2
    SCL = D1
  */

  Wire.begin(D2, D1);

  /*
    OLED Start
  */

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C)) {

    Serial.println("OLED not found");

    while (true);
  }

  /*
    MPU6050 Start
  */

  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 Connected Successfully!");
  } else {
    Serial.println("MPU6050 Connection Failed!");

    while (true);
  }

  /*
    Static IP Setup
  */

  if (!WiFi.config(
        local_IP,
        gateway,
        subnet,
        primaryDNS,
        secondaryDNS)) {

    Serial.println("Static IP Failed");
  }

  /*
    Connect WiFi
  */

  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /*
    Web Server Routes
  */

  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();

  Serial.println("Web Server Started");

  /*
    Clear OLED
  */

  display.clearDisplay();
  display.display();
}

/*
  ---------------------------------------------------------
  Main Loop
  ---------------------------------------------------------
*/

void loop() {

  int16_t ax, ay, az;

  /*
    Read MPU6050
  */

  mpu.getAcceleration(
    &ax,
    &ay,
    &az
  );

  /*
    Calculate X-axis Angle

    angleX = atan2(ay, az)
  */

  angleX =
    atan2(ay, az) * 180 / PI;

  /*
    Level Status Logic
  */

  if (angleX > 0.5) {

    levelStatus = "TILT RIGHT";
    statusColor = "#ff4444";

  }
  else if (angleX < -0.5) {

    levelStatus = "TILT LEFT";
    statusColor = "#ff4444";

  }
  else {

    levelStatus = "PERFECT LEVEL";
    statusColor = "#00ff66";
  }

  /*
    OLED Display
  */

  display.clearDisplay();
  display.setTextColor(WHITE);

  // Title
  display.setTextSize(2);
  display.setCursor(35, 0);
  display.print("LEVEL");

  // Angle value
  display.setTextSize(3);
  display.setCursor(10, 25);
  display.print(angleX, 1);

  // Degree symbol
  display.setTextSize(2);
  display.setCursor(110, 25);
  display.print((char)247);

  display.display();

  /*
    Handle Web Clients
  */

  server.handleClient();

  delay(50);
}