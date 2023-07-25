#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#define M_PI 3.14159265358979323846264338327950288
#define rxGPS 4
#define txGPS 5
const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>

  <style type="text/css">
    body {
        background-image: url('https://images.unsplash.com/photo-1488866022504-f2584929ca5f?auto=format&fit=crop&w=1486&q=80&ixid=dW5zcGxhc2guY29tOzs7Ozs%3D');
        background-size: cover;
        margin: 0;
        background-repeat: no-repeat;
        background-position: 0 0;
        transition: 2s cubic-bezier(0.645, 0.045, 0.355, 1);
        color :white;
    }

    hr {
        visibility: visible;
        border: 0;
        height: 1px;
        background-image: linear-gradient(to right, rgba(255, 255, 255, 0), rgba(255, 255, 255, 0.75), rgba(255, 255, 255, 0));
    }

    .button {
      background-color: rgba(55, 72, 87, 0.8); 
      border: none;
      color: white;
      padding: 12px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      border-radius: 12px;
      font-size: 16px;
    }
  </style>
  <script async defer 
	src="https://maps.googleapis.com/maps/api/js?key=AIzaSyD4oyfwEbnJ9K8Zb8UmdaG_J0lfYVK1Yrc&callback=initMap"></script>
	</head>
	

  <body style="background-color: #ffffff ; font-family:verdana">
    
    <center>
      <p>Device Status <span id = "Device_status" style="color : rgba(0, 255, 106, 0.857);">offline</span></p>
      <hr style = "width: 70%;">
      <h1>Dashboard</h1>
      <hr style = "width: 70%;">
      <div>
        <button class="button" onclick="send(1)">Fence 1</button>
        <button class="button" onclick="send(2)">Fence 2</button>
        <p>Selected Fence: <span id="fence_id">0</span><br></p>
      </div>
      <hr style = "width: 40%;">
      <div style="background-color: rgba(89, 154, 211, 0.3);; width: 30%; border-radius: 12px;">
          <table style="text-align: center; " >
              <tr>
                  <th style="padding: 0 20px 0 20px;">Date</th>
                  <th style="padding: 0 20px 0 20px;">No.of Sats</th>
              </tr>
              <tr>
                  <td><span id="date">0</span><br></td>
                  <td><span id="sats">0</span><br></td>
              </tr>
          </table>
      </div>
      <hr style = "width: 40%;">
      <div>
        <div style="background-color: rgba(89, 154, 211, 0.3); display: inline-block; width: 10%; border-radius: 12px;">
            <table style="text-align: center; " >
                <tr>
                    <th>Latitude</th>
                </tr>
                <tr>
                    <td><span id="lat_val">0</span><br></td>
                </tr>
                <tr>
                    <th>Longitude</th>
                </tr>
                <tr>
                    <td><span id="lon_val">0</span><br></td>
                </tr>
            </table>
          </div>
          <div style="background-color: rgba(89, 154, 211, 0.3); display: inline-block; width: 15%; border-radius: 12px;">
            <table style="text-align: center; " >
                <tr>
                    <th>Target Status</th>
                </tr>
                <tr>
                    <td><span id="target_status">0</span><br></td>
                </tr>
                <tr>
                    <th>Cumulative Angle</th>
                </tr>
                <tr>
                    <td><span id="angle">0</span><br></td>
                </tr>
            </table>
          </div>
      </div>
      <hr style = "width: 70%;">
	  <div id="googleMap" style="width:100%;height:400px;"></div>
    </center>
	<script>
	    let map;
	    let lat = 0;
	    let lng = 0;
        function send(fence_val) {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("fence_id").innerHTML = this.responseText;
            }
          };
          xhttp.open("GET", "status?fenceValue="+fence_val, true);
          xhttp.send();
        }

        setInterval(function() {getData();initMap();}, 2000); 

        function getData() {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              const data = this.responseText.split("#");
              document.getElementById("date").innerHTML = data[1];
              document.getElementById("sats").innerHTML = data[0];
              document.getElementById("lat_val").innerHTML = data[2];
			  lat = data[2];
              document.getElementById("lon_val").innerHTML = data[3];
			  lgn = data[3];
              document.getElementById("target_status").innerHTML = data[4];
              document.getElementById("angle").innerHTML = data[5];
              document.getElementById("Device_status").innerHTML = data[6];             
            }
          };
          xhttp.open("GET", "values", true);
          xhttp.send();
        }
		
		

		function initMap() {
			const mapOptions = {
				zoom: 8,
				center: { lat, lng },
			};

			map = new google.maps.Map(document.getElementById("googleMap"), mapOptions);
			const marker = new google.maps.Marker({
				
				position: { lat, lng },
				map: map,
			});
		   
			const infowindow = new google.maps.InfoWindow({
				content: "<p>Marker Location:" + marker.getPosition() + "</p>",
			});

			google.maps.event.addListener(marker, "click", () => {
				infowindow.open(map, marker);
			});
		}

		window.initMap = initMap;
    </script>
  </body>
</html>
)=====";

const double fences[1][10][2] = {{{17.529188, 78.361845},
                                  {17.529840, 78.361919},
                                  {17.529934, 78.362197},
                                  {17.530624, 78.362507},
                                  {17.530832, 78.363043},
                                  {17.530999, 78.363451},
                                  {17.530924, 78.363976},
                                  {17.529248, 78.363288},
                                  {17.529101, 78.362858},
                                  {17.529040, 78.362489},}
};

/*Variables to store AP credentials*/
String ssid = "";
String password = "";
int WiFiConnectMode = 1; /* 0: smart config; 1: hard code*/

double latitude, longitude;
int sat;
String date;
char lati[12];
char longi[12];
int targetStatus;
int fence;
char cumulativeAngle[12];
int deviceStatus = 0;

SoftwareSerial gpsSerial(rxGPS, txGPS);
TinyGPSPlus gps;
ESP8266WebServer gpsServer(80);

void connectWifi();
void updateLatLon();
void pip();
void handleRoot();
void fenceSelect();
void gps_data();

void setup(){
    Serial.begin(9600);
    gpsSerial.begin(9600);
    connectWifi();

    gpsServer.on("/", handleRoot);
    gpsServer.on("/status", fenceSelect);
    gpsServer.on("/values", gps_data);
    gpsServer.begin();
}
 
void loop(){
    while (gpsSerial.available()){
        deviceStatus = 1;
        updateLatLon();
        pip();        
        gpsServer.handleClient();
    }
    gpsServer.handleClient();
}

void connectWifi(){
    if(WiFiConnectMode == 0){
        // Operate the ESP12E in wifi station mode for smart config
        WiFi.mode(WIFI_STA);

        // Begin the smart configuration to get the Access Point credentials
        WiFi.beginSmartConfig();
        Serial.println("------------------------------------------------");
        Serial.print("Waiting for SmartConfig ");
        while (!WiFi.smartConfigDone()) {
            delay(250);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("SmartConfig done.");

        // Print the AP credentials to the serial monitor
        ssid = WiFi.SSID();
        password = WiFi.psk();
        Serial.println("------------------------------------------------");
        Serial.print("Acesspoint SSID : ");
        Serial.println(ssid);
        Serial.print("Acesspoint password : ");
        Serial.println(password);
        Serial.println("------------------------------------------------");

        // Connect the ESP12E to the AP
        Serial.print("Connecting to Access Point ");
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Connected.");
        Serial.println("------------------------------------------------");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.println("------------------------------------------------");
    }
    else{
        String ssid = "Galaxy"; //Access point ssid
        String password = "1234567890"; //Access point password
        WiFi.begin(ssid,password);
        Serial.println("------------------------------------------------");
        Serial.print("Connecting to Access Point ");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("------------------------------------------------");
    }
}

void updateLatLon(){
    if (gps.encode(gpsSerial.read()))
    {  
      sat = gps.satellites.value();
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      dtostrf(latitude,9,7,lati);
      dtostrf(longitude,9,7,longi);
      Serial.print("SATS: ");
      Serial.println(sat);
      Serial.print("LAT: ");
      Serial.println(latitude,6);
      Serial.print("LONG: ");
      Serial.println(longitude,6);
      Serial.print("ALT: ");
      Serial.println(gps.altitude.meters());
      Serial.print("SPEED: ");
      Serial.println(gps.speed.mps());
 
      Serial.print("Date: ");
      date = String(gps.date.day())+"/"+gps.date.month()+"/"+gps.date.year();
      Serial.println(date);
 
      Serial.print("Hour: ");
      Serial.print(gps.time.hour()); Serial.print(":");
      Serial.print(gps.time.minute()); Serial.print(":");
      Serial.println(gps.time.second());
      Serial.println("---------------------------");
      delay(1000);
    }
}

void pip(){
    int fenceSize = sizeof(fences[fence - 1])/sizeof(fences[fence - 1][0]);
    double vectors[fenceSize][2];
    for(int i = 0; i < fenceSize; i++){
        vectors[i][0] = fences[fence - 1][i][0] - latitude;
        vectors[i][1] = fences[fence - 1][i][1] - longitude;
    }
    double angle = 0;
    double num, den;
    for(int i = 0; i < fenceSize; i++){
        num = (vectors[i%fenceSize][0])*(vectors[(i+1)%fenceSize][0])+ (vectors[i%fenceSize][1])*(vectors[(i+1)%fenceSize][1]);
        den = (sqrt(pow(vectors[i%fenceSize][0],2) + pow(vectors[i%fenceSize][1],2)))*(sqrt(pow(vectors[(i+1)%fenceSize][0],2) + pow(vectors[(i+1)%fenceSize][1],2)));
        angle = angle + (180*acos(num/den)/M_PI);
    }
    dtostrf(angle,9,7,cumulativeAngle);
    if(angle > 355 && angle < 365)
        targetStatus = 1;
    else
        targetStatus = 0;
}

void handleRoot(){
    String s = webpage;
    gpsServer.send(200, "text/html", s);
}

void fenceSelect(){
    fence = gpsServer.arg("fenceValue").toInt();
    gpsServer.send(200, "text/plane", String(fence));
}

void gps_data(){
    String payload = String(sat) + "#" + date + "#" + lati + "#" + longi;
    if(targetStatus == 0)
        payload = payload + "#outside";
    else
        payload = payload + "#inside";
    payload = payload + "#" + cumulativeAngle;
    if(deviceStatus == 0)
        payload = payload + "#offline";
    else
        payload = payload + "#online";
    gpsServer.send(200, "text/plane", payload);
}