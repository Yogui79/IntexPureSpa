# Intex PureSpa modify to WIFI

Modify the Intex PureSpa to Wifi for  **#28458, #28462 & #28442, #28440**. No hardware modification and used with Smart Home (optimized for Jeedom & Home Assistant).

Need Help? Join  [Whaller](https://whaller.com/sphere/aixade)

## Pictures & Videos

Videos and pictures coming soon... 

## Hardware

-   **ESP32 Dev Kit C V4**  (Microcontroller) -  [amazon.de](https://www.amazon.de/AZDelivery-ESP32-NodeMCU-gratis-eBook/dp/B07Z83MF5W/ref=sr_1_4?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=ESP32&qid=1613410149&sr=8-4)
-   **LC12s**  (Wire modul, [Manuel about the Chip](Docs/H2-LCS12.pdf)) -  [amazon.de](https://www.amazon.de/LC12S-Wireless-serielle-transparente-Transmition/dp/B07JDN3QL7/ref=sr_1_1?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=lc12s&qid=1613409977&sr=8-1)  or  [aliexpress](https://de.aliexpress.com/item/4001201940321.html?spm=a2g0o.productlist.0.0.488361e7d3jNj7&algo_pvid=0319d211-c29a-4aef-ba9c-feb4d60fade2&algo_expid=0319d211-c29a-4aef-ba9c-feb4d60fade2-1&btsid=0b0a555616134100516381178e3281&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603)
-   **Arduino Uno**  (Optional) -  [amazon.de](https://www.amazon.de/Arduino-Uno-Rev-3-Mikrocontroller-Board/dp/B008GRTSV6/ref=sr_1_3?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Arduino%20Uno&qid=1613414774&quartzVehicle=35-163&replacementKeywords=arduino&sr=8-3)

## Software

-   [**Arduino IDE**](https://www.arduino.cc/en/software)
-   **Install the “[ESP32 Dev Kit C V4](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)” Board for Arduino IDE**  ->  [Screenshot](Docs/Image/Board-Screenshot-IDE.PNG)
-   **Install the following libraries**  (search in the Arduino IDE in libraries)
    -   EspMQTTClient.h
    -   Timer …link… (new?)
    -   SoftwareSerial
    -   WiFi
    -   ESPmDNS
    -   ArduinoOTA
    -   MySensors (optional)
-   **You need a MQTT broker**  (e.g. Mosquitto Broker)
-   **[Download](Code/Spa/Spa.ino) the PureSpa Code**

## Pinouts

| LC12S | ESP32 | Arduino |
|-------|-------|---------|
| GND   | GND   | GND     |
| CS    | D18   | D5      |
| SET   | D19   | D6      |
| TX    | D16   | D2      |
| RX    | D17   | D4      |
| VCC   | 3.3V  | 3.3V    |

![ESP32 to LC12s](Docs/Image/Pinouts_ESP32-LC12s_v1.jpg)


## Choose your PureSpa modell

**Uncomment one line for your Spa**
```
//#define _28458_28462_
//#define _28442_28440_
```
## MQTT communication protocol

**Write your WIFI settings in this lines**

```
const char* Myssid = "YourSSID";
const char* Mypassword = "YourPassword";
```

----------

**Write your MQTT settings in this lines**

```
"YourMQTT-Broker-IP", 	  // MQTT Broker server ip
"NameMQTTBroker",        // Can be omitted if not needed
"PasswordMQTTBroker",   // Can be omitted if not needed
"IntexSpa",            // Client name that uniquely identif your device. Don't change the name!
 1883                 // The MQTT port, default to 1883. this line can be omitted
```

----------

**MQTT topic & payload**

-   Topic is the path to communtions over MQTT:  
    e.g.:  _**IntexSpa/Cmd Power on off**_
-   Payload is the command 


| Description                  | Topic String                     | Payload | Payload | Only Status           |
|------------------------------|----------------------------------|---------|---------|-----------------|
| **Power**                    | IntexSpa/Cmd Power on off        | ON=1    | OFF=0   | -               |
| **Water Filter**             | IntexSpa/Cmd water filter on off | ON=1    | OFF=0   | -               |
| **Bubble**                   | IntexSpa/Cmd bubble on off       | ON=1    | OFF=0   | -               |
| **Heater**                   | IntexSpa/Cmd heater on off       | ON=1    | OFF=0   | -               |
| **Change Farenheit/Celsius** | IntexSpa/Cmd Farenheit Celsius   | F=1     | C=0     | -               |
| **Decrease the Temp.**       | IntexSpa/Cmd decrease            | UP=1    | -       | -               |
| **Increase the Temp.**       | IntexSpa/Cmd increase            | Down=1  | -       | -  
| **Status Communication with pump**| IntexSpa/Communication with pump | Com/OK=1   | 0=lost connection       | -   |              |
| **ESP Reset**                | IntexSpa/Cmd Reset ESP            | reset | -       | -             |  
| **Send Farenheit Temp.**     | IntexSpa/Farenheit Celsius       | -       | -       | Yes             |
| **Setpoint Temp.**           | IntexSpa/Temperature Setpoint    | -       | -       | Yes             |
| **Send Actual Temp.**        | IntexSpa/Actual Temperature      | -       | -       | Yes             |
| **Send Error Message**       | IntexSpa/Error Number            | -       | -       | Yes             |
| **Status Power on**          | IntexSpa/Power on                | -       | -       | Yes             |  
| **Status Bubble on**         | IntexSpa/Bubble on               | -       | -       | Yes             |  
| **Status Heater on**         | IntexSpa/heater on               | -       | -       | Yes             |  
| **Status Filter on**         | IntexSpa/filter on               | -       | -       | Yes             |  


***Only for Spa #28458 #28462***

| Description | Topic String                   | Payload | Payload | Only Status          |
|-------------|--------------------------------|---------|---------|-----------------|
| **Water Jet**             |  IntexSpa/Cmd water jet on off | ON=1    | OFF=0   | -               |
| **Sanizer**               | IntexSpa/Cmd sanizer on off    | ON=1    | OFF=0   | -               |
| **Status Water Jet**      | IntexSpa/Water jet on          | -       | -       | Yes             |
| **Status Sanizer**        | IntexSpa/Sanizer on            | -       | -       | Yes             |



## Debugging

You can debug on Arduino IDE with serial print on  **baud rate: 115200**

Options to debug  
_Uncomment to debug_  

    //#define DEBUG_RECIEVED_DATA  
    //#define DEBUG_SEND_COMMAND  
    //#define DEBUG_PUMP_DATA  
    //#define DEBUG_CONFIG  
    //#define DEBUG_MQTT  
    //#define DEBUG_SEND_VALUE_TO_HOME_AUTOMATION_SW

## OTA update

You can use OTA update (wireless) via Arduino IDE after the first upload via USB.  [Screenshot](Docs/Image/Screenshot-OTA.PNG)

## Changing ID and automatically search

## Home Assistant
Is a powerful open source home automation software. [www.home-assistant.io](https://www.home-assistant.io)

You can use it as you want, I'll show you an example of a part of the files configuration.yaml and automations.yaml


**Screenshot:**

comming soon...

**configuration.yaml**

Define the switches and sensors with MQTT Topics and Payload.

```csharp
#Switch MQTT
switch:
  - platform: mqtt
    command_topic: IntexSpa/Cmd Power on off
    state_topic: IntexSpa/Power on
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Einschalten"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd water filter on off
    state_topic: IntexSpa/filter on
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Filter"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd bubble on off
    state_topic: IntexSpa/Bubble on
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Bubble"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd heater on off
    state_topic: IntexSpa/heater on
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Heizung"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd decrease
    state_topic: IntexSpa/Cmd decrease
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Runter-"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd increase
    state_topic: IntexSpa/Cmd increase
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Rauf+"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd water jet on off
    state_topic: IntexSpa/Water jet on
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Jet-Duesen"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd sanizer on off
    state_topic: IntexSpa/Sanizer on
    payload_on: "1"
    payload_off: "0"
    name: "Intex_Desinfektion"
    
  - platform: mqtt
    command_topic: IntexSpa/Cmd Reset ESP
    state_topic: IntexSpa/Cmd Reset ESP
    payload_on: "reset"
    payload_off: " "
    name: "Reset ESP-script-switch"

#Sensor
sensor:
  - platform: mqtt
    name: "Status"
    state_topic: "IntexSpa/Error Number"
    unit_of_measurement: 'E'
  - platform: mqtt
    name: "Aktuelle Temp."
    state_topic: "IntexSpa/Actual Temperature"
    unit_of_measurement: '°c'
  - platform: mqtt
    name: "Set Temp."
    state_topic: "IntexSpa/Temperature Setpoint"
    unit_of_measurement: '°c'
  - platform: mqtt
    name: "IntexSpa_Kommunikation mit Pumpe"
    state_topic: "IntexSpa/Communication with pump"
    
#Template Sensor   
  - platform: template
    sensors:
      intexstatustemplate:
        friendly_name: "Intex Status Template"
        value_template: >-
          {% if is_state('sensor.status','0') %}
            OK
          {% else %}
            E 
            {{ states('sensor.status') }}
          {% endif %}
  - platform: template
    sensors:
      intexkommunikationmitpumpetemplate:
        friendly_name: "Intex Kommunikation mit Pumpe - Template"
        value_template: >-
          {% if is_state('sensor.intexspa_kommunikation_mit_pumpe','1') %}
            OK
          {% else %}
            Keine Verbindung
          {% endif %}
```
**automations.yaml** (optional)

All your automation settings such as push-notification on your mobile phone.
```csharp
- service: notify.mobile_app_mi_9_lite
    data:
      title: Whirlpool
      message: Whirlpool ist eingeschalten
  mode: single
- id: 'xxxxx'
  alias: Push-Notify/Whirlpool Heizung Einschalten
  description: ''
  trigger:
  - platform: state
    entity_id: switch.intex_heizung
    from: 'off'
    to: 'on'
  condition: []
  action:
  - service: notify.mobile_app_mi_9_lite
    data:
      title: Whirlpool Heizung
      message: Die Heizung ist eingeschalten!
  mode: single
- id: 'xxxxxxx'
  alias: Push-Notify/Whirlpool Set. Temp >33°C
  description: ''
  trigger:
  - platform: mqtt
    topic: IntexSpa/Temperature Setpoint
    payload: '33'
  condition: []
  action:
  - service: notify.mobile_app_mi_9_lite
    data:
      title: Whirlpool Set. Temp
      message: Die eingestellte Temperatur ist über 33°C
  mode: single
- id: 'xxxxxxx'
  alias: Push-Notify/Whirlpool Aktuelle Temp >34°C
  description: ''
  trigger:
  - platform: mqtt
    topic: IntexSpa/Actual Temperature
    payload: '34'
  condition: []
  action:
  - service: notify.mobile_app_mi_9_lite
    data:
      title: Whirlpool Aktuelle Temperatur
      message: Die Aktuelle Temperatur ist über 34°C
  mode: single
- id: 'xxxxxxxx'
  alias: Push-Notify/Whirlpool Error Meldungen
  description: ''
  trigger:
  - platform: numeric_state
    entity_id: sensor.status
    above: '0'
  condition: []
  action:
  - service: notify.mobile_app_mi_9_lite
    data:
      title: Whirlpool Error
      message: ACHTUNG! Whirlpool Error {{ states('sensor.status') }}
  mode: single
```


## Jeedom

## The official Intex PureSpa instructions

| Articel No. | English  | German   |
|-------------|----------|----------|
| 28462 & 28458      | [Download](Docs/28462-28458_344IO_202_EN_.pdf) | [Download](Docs/28462_28458_344IO_2020_DE.pdf) |
| 28442 & 28440       | [Download](Docs/28442-28440_346IO_2020_EN.pdf) | [Download](Docs/28442_28440_346IO_2020_DE.pdf) |
