// Code to measure water level in a water tank
// Board: ESP8266 NodeMCU
// 15 May 2021
// Pablo Cruz Lemini
// portfedh@gmail.com

// Mills Function 
const unsigned long event_interval = 300000; // 5 minute interval, 300 second
//const unsigned long event_interval = 2000; // 2 second interval. For testing
unsigned long previous_time = 0; 

// MQTT Library
#include "EspMQTTClient.h" 

#define WIFI_SSID        "<Wifi_Username_here>"                          // WiFi Username
#define WIFI_PASS        "<Wifi_Password_here>"                          // Wifi Password

#define BROKER_IP        "<MQTT_ip_here>"                                // IP adress of MQTT broker
#define BROKER_USERNAME  "<broker_username_here>"                        // Broker username
#define BROKER_PASSWORD  "<broker_password_here>"                        // Broker password
#define CLIENT_NAME      "<device_name_here>"                            // MQTT client name to identify the device
#define BROKER_PORT      <mqtt_port_here>                                // MQTT Port. No "" needed
#define lastwill_topic   "<lastwill_topic_here>"                         // MQTT topic to report lastwill and testament.
#define lastwill_text    "<lastwill_message_here>"                       // MQTT memssage to report lastwill and testament.

String  client_name       = CLIENT_NAME;                                 // MQTT Topic to report initial value
String  startup_topic     = "<startup_topic_here>";                      // MQTT Topic to report startup
String  water_level_topic = "<reporting_values_topic_here>";             // MQTT topic to report values

// Function to connect to MQTT 
EspMQTTClient client(
                  WIFI_SSID,
                  WIFI_PASS,
                  BROKER_IP,
                  BROKER_USERNAME,
                  BROKER_PASSWORD,
                  CLIENT_NAME,
                  BROKER_PORT
                  );

// Water Sensor pins
#define TRIG 14 //GPIO Number 14, D5
#define ECHO 12 //GPIO Number 12, D6

void setup() { 
  
  Serial.begin(115200); // Serial monitoring 
  
  // Enable  debugging messages sent to serial output
  client.enableDebuggingMessages(); 

  // Enable the web updater.
  client.enableHTTPWebUpdater();     
  
  // MQTT Last Will & Testament
  client.enableLastWillMessage( lastwill_topic , lastwill_text);
  
  // Water level sensor Pin Setup
  pinMode(TRIG, OUTPUT);       // Initializing Trigger Output
  pinMode(ECHO, INPUT_PULLUP); // Initializing Echo Input
  } 

// MQTT Innitial Connection
void onConnectionEstablished() {
  client.publish(startup_topic, String(client_name + " is now online."));
  }
  
  void loop() { 

    // MQTT Loop: Must be called once per loop.
    client.loop(); 

    // Mills Loop
    unsigned long current_time = millis();

    // Mills if Statement
    if(current_time - previous_time >= event_interval) {
      // Set the trigger pin to low for 2uS 
      digitalWrite(TRIG, LOW); 
      delayMicroseconds(2); 

      // Send a 20uS high to trigger ranging
      digitalWrite(TRIG, HIGH);  
      delayMicroseconds(20); 

      // Send pin low again
      digitalWrite(TRIG, LOW);  

      // Read pulse times
      int distance = pulseIn(ECHO, HIGH,26000);  

      //Convert the pulse duration to distance
      distance= distance/58; 

      //Print Result in serial monitor
      Serial.print("Distance ");
      Serial.print(distance);
      Serial.println("cm");

      // MQTT Client Publisher
      client.publish(water_level_topic, String(distance));

      // Mills Update timing for next time
      previous_time = current_time;
    }
    
}
