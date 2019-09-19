#include <Ethernet.h>
#include <SPI.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include "RestClient.h"
#include "Servo.h"

#define IP "192.168.0.6"
#define PORT 3000

RestClient client = RestClient(IP, PORT);

void setup()
{
  Serial.begin(9600);
  
  Serial.println("connect to network");
  client.dhcp();

  Serial.print("IP de Placa Ethernet Shield ");
  Serial.println(Ethernet.localIP());
  
  Serial.println("Listo :D!");
}

void loop()
{
  Higrometro();
}

  int Higrometro()
  {
    #define DHTPIN 8
    #define DHTTYPE DHT11
    
    int t = 0;
    int h = 0;
    
    DHT dht(DHTPIN, DHTTYPE);
    dht.begin();

    h = (int) dht.readHumidity();
    t = (int) dht.readTemperature();
    
    Serial.println("Temperatura: ");
    Serial.print(t);
    Serial.println(" ° C");
    Serial.println("Humedad: ");
    Serial.print(h);
    Serial.println(" %");

    postHigrometro(t, h);
  }
  void postHigrometro(int t, int h)
  {
    String responseHigrometro = "";
    
    client.setHeader("Content-Type: application/json");
  
    StaticJsonBuffer<200> jsonBuffer;
    char json[256];  
    JsonObject& root = jsonBuffer.createObject();
    root["temperatura"] = t;
    root["humedad"] = h;
    root.printTo(json, sizeof(json));
    Serial.println(json);
    
    int statusCode = client.post("http://localhost:3000/api/higrometro/", json, &responseHigrometro);
    
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    Serial.print("ResponseHigrometro body from server: ");
    Serial.println(responseHigrometro);
    
    delay(3000);
  }
