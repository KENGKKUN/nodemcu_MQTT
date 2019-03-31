#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial com_serial(D2,D3); // RX, TX
int enablePin = D4;
int num;


// Update these with values suitable for your network.
const char* ssid = "your-ssid";  // WIFI NAME
const char* password = "your-password"; // WIFI PASSWORD


// Config MQTT Server
#define mqtt_server "m14.cloudmqtt.com"
#define mqtt_port 10670
#define mqtt_user "TEST"
#define mqtt_password "1234"

//#define LED_PIN D5

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
//  pinMode(LED_PIN, OUTPUT);
  pinMode(enablePin, OUTPUT);
  Serial.begin(9600);
  com_serial.begin(9600);
  digitalWrite(enablePin, LOW); // เมื่อส่งข้อมูลเสร็จแล้วต้องส่ง “0″ มาที่ขา RE และ DE เพื่อรอรับข้อมูล 
  delay(200);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, mqtt_port); // (server, port)
  client.setCallback(callback);
}

void loop() {
  int n;
  char message[256];
  while(com_serial.available()) {
    message[n] = com_serial.read(); // read data
    n++;
  }
   num = atoi(message); // convert string to int
   //client.publish("/ESP/LED", "Hello");
   client.publish("/ESP/LED", message); //ส่งค่าข้อมูลไปยัง MQTT Cloud
   Serial.print(message);

  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("/ESP/LED");
   
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  int i=0;

  /*while (i<length) msg += (char)payload[i++];
  if (msg == "GET") {
    client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDON" : "LEDOFF"));
    client.publish("/ESP/LED", "TEST SENT DATA");
    Serial.println("Send !");
    return;
  }
  digitalWrite(LED_PIN, (msg == "LEDON" ? HIGH : LOW));*/
  Serial.println(msg);
}
