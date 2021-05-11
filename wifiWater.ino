//14.3V MAXIMUM at A0!!!!
#include <ESP8266WiFi.h>
void writeHeaders(WiFiClient c, int len) {
  c.print("HTTP/1.1 200 OK\r\n");
  c.print("Access-Control-Allow-Origin: *\r\n");
  c.print("Connection: Close\r\n");
  c.print("Cache-Control: no-store\r\n");
  c.print("Content-Type: text/html\r\n");
  c.print("Content-Length: " +String(len)+ "\r\n\r\n");
}
WiFiServer server(80);
bool valveOn = false;
bool charging = true;
int autoMode = 0;
const int valvePin = 5;
const int solarPin = 4;
 
unsigned long lastBurstBegin = 0; 
unsigned long intervalSeconds = 5;
unsigned int periodSeconds = 4;
const float batteryReadInterval = 60;
unsigned long lastBatteryRead = 0;
float batteryVoltage;
int readV;
const float maxBatVoltage = 13.6;
const float minBatVoltage = 12.0;

 
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  
  pinMode(valvePin, OUTPUT);
  pinMode(solarPin, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(solarPin, HIGH);
  digitalWrite(valvePin, HIGH);
  WiFi.mode(WIFI_STA);
  delay(200);
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  connect();
  server.begin();
  //WiFi.forceSleepBegin();
  
}

void loop() {
  
  WiFiClient cli = server.available();
  if(cli){
    responseHandler(cli);
  }
  if(autoMode == 1 && ((millis() - lastBurstBegin)/1000.0 - periodSeconds) >= intervalSeconds){
    //Time for a new burst  
    Serial.println("New burst Begin");
    digitalWrite(valvePin, LOW);
    valveOn = true;
    lastBurstBegin = millis();
  }
  if(valveOn && (millis() - lastBurstBegin)/1000 > periodSeconds){
    //time to stop burst
    Serial.println("Burst Stop");
    digitalWrite(valvePin, HIGH);
    valveOn = false;
  } 
  if((millis() - lastBatteryRead)/1000.0 > batteryReadInterval){ 
    lastBatteryRead = millis();
    readV = analogRead(A0);
    batteryVoltage = 0.01416658794*readV - 0.165303595;
    Serial.println("Voltage "+String(batteryVoltage) + " " + String(readV));
    if(batteryVoltage <= minBatVoltage){
      //Critically low
      digitalWrite(solarPin, HIGH);
      charging = true;
    }
    else if(batteryVoltage > maxBatVoltage){
      //Too high
      digitalWrite(solarPin, LOW);
      charging = false;
    }
    else {
      //in between
      digitalWrite(solarPin, HIGH);
      charging = true;
    }
  }
  delay(10);
}

void connect(){
  Serial.println("Connecting");
  digitalWrite(2, LOW);
  WiFi.begin("ASUS 2,4GHz", "Doris123");
   while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
   
    delay(100);
   }
   Serial.println("Connected");
   digitalWrite(2, HIGH);
   
}
