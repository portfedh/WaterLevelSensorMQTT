# MQTT Water Level Sensor

![Water Level Sensor](https://pablocruz.io/wp-content/uploads/2022/11/water_level_featured-2048x970.png)

I recently made a blog post describing this project in detail. 

You find the post [here.](https://pablocruz.io/water-level-monitor/)

The idea of this sensor is to monitor the water level of a water tank and report its values every 5 minutes. 

This way the water level can be checked remotely, and an alarm set to go off if the value falls below a certain range.

The same sensor and code may be used to monitor the level of other liquids in a container with a height up to 4 meters.

## What it does

The Arduino code uses a JSN-SR04T ultrasonic sensor, connected to an ESP8266 and an electrical outlet to read and send the data through Wi-Fi and MQTT.

The microcontroller will take measurements every 5 minutes and transform the ultrasonic value time into the distance from the sensor in cm.  

Measurements will be sent through WIFI to an MQTT broker. Although the code for the MQTT in not provided, here is a brief description of what can be achieved:

With an MQTT broker, Node-Red and Grafana, you can receive the values, store them in a database and then display them in a graphical manner.

Both Grafana and Node-Red can be integrated with Telegram. This allows message alarms to be sent in case water level falls below a threshold value.

The code in WaterLevelSensorMQTT.ino will output the distance to the water level in cm, and the calculation to determine the existing volume of water (in Liters) is made in Node-Red.

An alternative would be calculating the water volume directly in the ESP8266 and report as its output to MQTT.

I chose the former method in case the formula needed any changes. You can update Node-Red remotely, but you must be physically close to flash the ESP8266.

The formula to determine the water volume will vary depending on the container shape, but the height of the shape will be calculated using:  
***height = total_height - sensor_distance***

## How to Install

To use this script, you must have the following:

Hardware:

- ESP8266
- JSN-SR04T ultrasonic sensor
- An Electrical connection to power microcontroller and sensor
- A watertight electrical box
- Optional: Electrical PVC conduit
- Optional: [3D Printed Sensor Fitting](https://www.prusaprinters.org/prints/67422-jsn-sr04t-water-level-sensor-case)

Software:

- ESP8266 Boards installed from the Boards Manager
- Arduino Library: [EspMQTTClient.h](https://www.arduino.cc/reference/en/libraries/espmqttclient/)

The sensor requires 3V input to work and so can be powered directly from the microcontroller.

Connection Diagram:
-- Connection Diagram ---

Some Photos of the setup:

![Water Tank 1](https://bite-size.mx/WaterTank1.png)
![Water Tank 3](https://bite-size.mx/WaterTank2.png)
![Water Tank 3](https://bite-size.mx/WaterTank3.png)

## How to Use

1. Open the WaterLevelSensorMQTT.ino file:
    - Add your Wi-Fi username & password
    - Define your MQTT port (1883 is conventionally used)
    - Add your MQTT broker Username & Password
    - Define a name to identify your sensor
    - Define the MQTT topic the sensor will publish the data into

2. Verify and upload your code

3. Check with the serial monitor that the values are being reported

4. Check with your MQTT broker that values are being reported correctly through Wi-Fi.

Output should be something similar to the following:

---(Serial Monitor GIF here)---

----(Over MQTT Broker GIF here)--

Here is an image of how the information looks when written into influxdb and then displayed in Grafana. The graph shows a 15 day period. 

![Grafana Output](http://bite-size.mx/GrafanaWaterLevelSensor.png)

## Use cases

I designed this to monitor the water level in my main water tank, located in my roof. I plan on installing a second one in a water cistern.

Other uses could be:

- Monitor the level of liquid in any industrial container.
- Monitor the water level of a municipal water reservoir.
- Monitor the amount of product or Inventory in a specific rack through its heigh.
- Monitor the presence of a car in a parking space

## Contributing

The explanation of how to use this sensor could be broadened by:  

- Explaining how to configure the MQTT Broker
- Explaining how to configure Node-Red
- Explaining how to configure a database (I use Influx DB)
- Explaining how to set up Grafana to visualize the information
- Explaining how to set up alarms with Telegram

It could go even further by:

- Explaining how to set up the database and Grafana on AWS/Azure or a server you can access from anywhere.
- How to create an API so anyone can download the information.
