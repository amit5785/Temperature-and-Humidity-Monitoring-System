#include <DHT.h> //  DHT.h library
#include <ESP8266WiFi.h> // ESP8266WiFi.h library
#define DHTPIN 5        
#define DHTTYPE  DHT11  

const char* ssid     = "#Amit Singh";// replace subscribe with your WiFi SSID(Name)
const char* password = "Papa5785";//replace with Your Wifi Password name
const char* host = "api.thingspeak.com";
String writeAPIKey = "OMBQG2WDQQL11IG0"; //copy yout ThingSpeak channel API Key.


int wifi_led=14;


DHT dht(DHTPIN, DHTTYPE);
int soil=A0;
int str;
int cont;

WiFiClient client;


void setup() {
  // Initialize sensor

  Serial.begin(9600);
  delay(1000);
  Serial.println("Amit");
  delay(1000);
  pinMode(A0,INPUT);
  pinMode(wifi_led,OUTPUT);
  pinMode(DHTPIN,INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    digitalWrite(wifi_led,HIGH);
    delay(400);
    digitalWrite(wifi_led,LOW);
    delay(100);
  }
    
  Serial.println("");
  delay(1000);
  Serial.println("WiFi connected.....");
  digitalWrite(wifi_led,HIGH);

 dht.begin();
 delay(1000);
 
}

void loop() 
{
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 str=analogRead(A0);

if (isnan(h) || isnan(t)) 
{
  Serial.println("Failed to read from DHT Sensor");
  return;
}

  if (client.connect(host,80)) 
  {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = writeAPIKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr += "\r\n\r\n";
           postStr +="&field2=";
           postStr += String(h);
           postStr += "\r\n\r\n";
           postStr +="&field3=";
           postStr += String(str);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
 
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("\n");
  Serial.print("Soil MOisture : ");
  Serial.print(str);
  Serial.print("\n");
  Serial.print("Humidity : ");
  Serial.println(h);
  
  Serial.println("Send to ThingSpeak.\n");
  }
  client.stop();
   

   Serial.println("Waiting...");    
  // thingspeak needs minimum 15 sec delay between updates
  // Here I set upload every 20 seconds
  for(unsigned int i = 0; i < 20; i++)
  {
    delay(1000);                         
  }                       

}
