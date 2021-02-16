# Intex PureSpa modify to WIFI

Modify the Intex PureSpa to Wifi for  **#28458, #28462 & #28442, #28440**. No hardware modification and used with Smart Home (optimized for Jeedom & Home Assistant).

Need Help? Join  [Whaller](https://whaller.com/sphere/aixade)

## Pictures & Videos

youtube link…  
pictures…link

## Hardware

-   **ESP32 Dev Kit C V4**  (Microcontroller)  [amazon.de](https://www.amazon.de/AZDelivery-ESP32-NodeMCU-gratis-eBook/dp/B07Z83MF5W/ref=sr_1_4?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=ESP32&qid=1613410149&sr=8-4)
-   **LC12s**  (Wire modul)  [amazon.de](https://www.amazon.de/LC12S-Wireless-serielle-transparente-Transmition/dp/B07JDN3QL7/ref=sr_1_1?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=lc12s&qid=1613409977&sr=8-1)  or  [aliexpress](https://de.aliexpress.com/item/4001201940321.html?spm=a2g0o.productlist.0.0.488361e7d3jNj7&algo_pvid=0319d211-c29a-4aef-ba9c-feb4d60fade2&algo_expid=0319d211-c29a-4aef-ba9c-feb4d60fade2-1&btsid=0b0a555616134100516381178e3281&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603)
-   **Arduino Uno**  (Optional)  [amazon.de](https://www.amazon.de/Arduino-Uno-Rev-3-Mikrocontroller-Board/dp/B008GRTSV6/ref=sr_1_3?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Arduino%20Uno&qid=1613414774&quartzVehicle=35-163&replacementKeywords=arduino&sr=8-3)

## Software

-   [**Arduino IDE**](https://www.arduino.cc/en/software)
-   **Install the “[ESP32 Dev Kit C V4](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)” Board for Arduino IDE**  ->  [Screenshot](https://ibb.co/HgSv2k0)
-   **Install the following libraries**  (search in the Arduino IDE in libraries)
    -   EspMQTTClient.h
    -   Timer …link… (new?)
    -   SoftwareSerial
    -   WiFi
    -   ESPmDNS
    -   ArduinoOTA
    -   MySensors (optional)
-   **You need a MQTT broker**  (e.g. Mosquitto Broker)
-   **Download the arduino sketch**  …link…

## Pinouts

| LC12S | ESP32 | Arduino |
|-------|-------|---------|
| GND   | GND   | GND     |
| CS    | D18   | D5      |
| SET   | D19   | D6      |
| TX    | D16   | D2      |
| RX    | D17   | D4      |
| VCC   | 3.3V  | 3.3V    |


**Picture:**  
[ESP32 to LC12s Pinsouts](https://ibb.co/tppN1LN)

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
-   Payload is the  **ON**  or  **OFF**  command.  **ON = 1, OFF = 0**

| Description                  | Topic String                     | Payload | Payload | Only Send Value |
|------------------------------|----------------------------------|---------|---------|-----------------|
| **Power**                    | IntexSpa/Cmd Power on off        | ON=1    | OFF=0   | -               |
| **Water Filter**             | IntexSpa/Cmd water filter on off | ON=1    | OFF=0   | -               |
| **Bubble**                   | IntexSpa/Cmd bubble on off       | ON=1    | OFF=0   | -               |
| **Heater**                   | IntexSpa/Cmd heater on off       | ON=1    | OFF=0   | -               |
| **Change Farenheit/Celsius** | IntexSpa/Cmd Farenheit Celsius   | F=1     | C=0     | -               |
| **Decrease the Temp.**       | IntexSpa/Cmd decrease            | UP=1    | -       | -               |
| **Increase the Temp.**       | IntexSpa/Cmd increase            | Down=1  | -       | -               |
| *Only for Spa #28458 #28462* |                                  |         |         |                 |
| **Water Jet**                | IntexSpa/Cmd water jet on off    | ON=1    | OFF=0   | -               |
| **Sanizer**                  | IntexSpa/Cmd sanizer on off      | ON=1    | OFF=0   | -               |
| **Send Farenheit Temp.**     | IntexSpa/Farenheit Celsius       | -       | -       | Yes             |
| **Setpoint Temp.**           | IntexSpa/Temperature Setpoint    | -       | -       | Yes             |
| **Send Actual Temp.**        | IntexSpa/Actual Temperature      | -       | -       | Yes             |
| **Send Error Message**       | IntexSpa/Error Number            | -       | -       | Yes             |


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

You can use OTA update (wireless) via Arduino IDE after the first upload via USB.  [Screenshot](https://ibb.co/sVQmcPv)

## Changing ID and automatically search

## Home Assistant

## Jeedom

## The official Intex PureSpa instructions

| Articel No. | English  | German   |
|-------------|----------|----------|
| 28462 & 28458      | [Download](https://drive.google.com/file/d/1DxWYF008gwoMBJz6GUzMcdQoPq15pnJM/view?usp=sharing) | [Download](https://drive.google.com/file/d/1_ugZ-NHHIyApSz-Kx2lPer2lYniRxMsm/view?usp=sharing) |
| 28442 & 28440       | [Download](https://drive.google.com/file/d/1PWCq753cWBZs-A_MyYc2H3ggdn6-7Fvi/view?usp=sharing) | [Download](https://drive.google.com/file/d/1PKFxE7ToB-wYHGv9uRoH_pc4w2nDUPUa/view?usp=sharing) |
