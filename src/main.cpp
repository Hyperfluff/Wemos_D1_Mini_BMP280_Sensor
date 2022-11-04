// include libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Wire.h"
#include "DFRobot_BMP280.h"

// include credentials file
#include "credentials.h"

// relevant stuff for the BMP280 sensor
typedef DFRobot_BMP280_IIC BMP; // ******** use abbreviations instead of full names ********
BMP bmp(&Wire, BMP::eSdoLow);
#define SEA_LEVEL_PRESSURE 1015.0f // sea level pressure

// Wifi / MQTT Stuff
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message = message + (char)payload[i]; // convert *byte to string
  }
  Serial.print(message);
  if (message == "on")
  {
    digitalWrite(LED_BUILTIN, LOW);
  } // LED on
  if (message == "off")
  {
    digitalWrite(LED_BUILTIN, HIGH);
  } // LED off
  Serial.println();
  Serial.println("-----------------------");
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect(hostname, mqtt_username, mqtt_password))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("IoT/test/foo");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);    // Turn the LED off by making the voltage HIGH
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW); // Turn the LED off by making the voltage HIGH

  bmp.reset();
  Serial.println("bmp read data test");
  while (bmp.begin() != BMP::eStatusOK)
  {
    Serial.println("bmp begin faild");
    delay(2000);
  }
  Serial.println("bmp begin success");
  delay(100);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > MEASURE_INTERVAL)
  {
    lastMsg = now;
    float temperature = bmp.getTemperature();
    float pressure = bmp.getPressure();
    client.publish(temperature_topic, String(temperature).c_str());
    client.publish(pressure_topic, String(pressure).c_str());
  }
}