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


// variables
unsigned long lastBlinkMillis = 0;
float temperature = 0;
float pressure = 0;

void callback(char *topic, byte *payload, unsigned int length)
{
  //store last time data came in from the broker
  lastBlinkMillis = millis();
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


void setupBMP(){
  bmp.reset();
  Serial.println("bmp read data test");
  int failCount = 0;
  while (bmp.begin() != BMP::eStatusOK)
  {
    Serial.println("bmp begin faild");
    failCount++;
    if (failCount > 30) ESP.restart();
    delay(2000);
  }
  Serial.println("bmp begin success");
  delay(100);
}

void readBMP(){
  temperature = bmp.getTemperature();
  pressure = bmp.getPressure();
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Pressure: ");
  Serial.println(pressure);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Temperaturesensor V0, Software Version V1.2");
  Serial.print("Sensor ID: ");
  Serial.println(SENSOR_ID);
  Serial.print("Hostname: ");
  Serial.println(hostname);
  Serial.print("Temperature Topic: ");
  Serial.println(temperature_topic);
  Serial.print("Pressure Topic: ");
  Serial.println(pressure_topic);
  
  pinMode(LED_BUILTIN, OUTPUT);    // Turn the LED off by making the voltage HIGH
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW); // Turn the LED off by making the voltage HIGH

  //connect to sensor and read Values
  setupBMP();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  lastBlinkMillis = millis();

}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  
  readBMP();
  String temperature_message = "{\"sensor_id\":";
  temperature_message += String(SENSOR_ID);
  temperature_message +=",\"parameter_id\":2,\"value\":";
  temperature_message += String(temperature);
  temperature_message += "}";
  
  
  String pressure_message = "{\"sensor_id\":";
  pressure_message += String(SENSOR_ID);
  pressure_message +=",\"parameter_id\":1,\"value\":";
  pressure_message += String(pressure);
  pressure_message += "}";

  Serial.println(temperature_message);
  Serial.println(pressure_message);
  client.publish(temperature_topic, temperature_message.c_str());
  client.publish(pressure_topic, pressure_message.c_str());
  for (int i = 0; i<= MEASURE_INTERVAL/1000; i++){
    Serial.print("Sekunden bis zur nächsten Messung: ");
    Serial.println((MEASURE_INTERVAL/1000)-i);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
  }
  
  /*
  WiFi.disconnect();
  WiFi.forceSleepBegin();
  for (int i = 0; i<= MEASURE_INTERVAL/1000; i++){
    Serial.print("Sekunden bis zum Neustart: ");
    Serial.println((MEASURE_INTERVAL/1000)-i);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
  }
  ESP.restart();
  */
  // if (now - lastBlinkMillis > 5000){
  //   ESP.restart();
  // }
}