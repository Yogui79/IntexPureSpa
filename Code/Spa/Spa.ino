/*
 *
 * Intex Purespa communication 
 * 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   Pin conection
 *                      
 *                       LC12S     Arduino     ESP
 * ___________________
 * |                  o| GNG       GND         GND
 * |                  o| CS        D5          D18
 * |                  o| SET       D6          D19
 * |                  o| TX        D2          D16
 * |                  o| RX        D4          D17
 * |__________________o| VCC       3.3V        3.3V

*/

/*******************************************************
*
*   S E L E C T    Y O U R   S P A   M O D E L 
*  
********************************************************/
#define _28458_28462_
//#define _28442_28440_

#if  defined (_28458_28462_) &&  defined (_28442_28440_)
  #error select only one SPA model
#endif
#if  !defined (_28458_28462_) &&  !defined (_28442_28440_)
  #error select a SPA model
#endif

/*******************************************************
*
*   S E L E C T  T H E   P R O T O C O L   Y O U   U S E
*  
********************************************************/
//#define _MY_SENSORS_
#define _MQTT_

#ifdef _MQTT_
const char* Myssid = "MySSID";
const char* Mypassword = "MyPassword";
#endif


#if defined (_MY_SENSORS_) &&  defined (_MQTT_)
   #error select between mysensors and MQTT 
#endif

#ifdef _MQTT_
#include "EspMQTTClient.h"

EspMQTTClient client(
  Myssid,
  Mypassword,
  "111.111.111.111",  // MQTT Broker server ip
  "MQTTUsername",   // Can be omitted if not needed
  "MQTTPassword",   // Can be omitted if not needed
  "IntexSpa",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);
#endif

//Enable following line to have some debug infos

//#define DEBUG_RECIEVED_DATA
#define DEBUG_SEND_COMMAND
#define DEBUG_PUMP_DATA
//#define DEBUG_CONFIG
#define DEBUG_MQTT
//#define DEBUG_SEND_VALUE_TO_HOME_AUTOMATION_SW


#ifdef _MY_SENSORS_
//#include <MySensors.h>
#endif
bool FirstSend;

#include <Timer.h>

#if defined (__AVR__) 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 4); // RX, TX
// Output defines
#define DO_SET    6
#define DO_CS     5
#elif defined (ESP32)
#define mySerial   Serial2
#define DO_SET    19
#define DO_CS     18
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif


//Command
#if defined (_28442_28440_)
#define FIRST_COMMAND_BYTE          0xC9
#define FIRST_PUMP_BYTE             0XC9
#elif defined (_28458_28462_)
#define FIRST_COMMAND_BYTE          0x1A
#define FIRST_PUMP_BYTE             0X1A
#endif

#define COMMAND_ON_OFF              0x0001    
#define COMMAND_WATER_FILTER        0x0010    
#define COMMAND_BUBBLE              0x0020    
#define COMMAND_HEATER              0x0040    
#define COMMAND_CELSIUS_FARENHEIT   0x0002    
#define COMMAND_DECREASE            0x0008    
#define COMMAND_INCREASE            0x0004    

#ifdef _28458_28462_
#define COMMAND_WATER_JET           0x0100    
#define COMMAND_SANITIZER           0x0080    
#endif
// Status Byte number
#define BYTE_STATUS_STATUS          2
#define BYTE_STATUS_COMMAND         4

#define BYTE_ACTUAL_TEMPERATURE     5
#define BYTE_SETPOINT_TEMPERATURE   7

#define BYTE_ERROR                  14

//Value Pump bytes
#define VALUE_CONTROLLER_ON           0x01
#define VALUE_BUBBLE_ON               0x10

#define VALUE_HEATER_STANDBY          0x02  //to check

#define VALUE_HEATER_ON               0x04  
#define VALUE_WATER_FILTER_ON         0x08  

#ifdef _28458_28462_
#define VALUE_WATER_JET_ON            0x80  
#define VALUE_SANITIZER_ON            0x20  
#endif

#define VALUE_FARENHEIT               0x01
#define VALUE_TEMP_WINDOWS_OPEN       0x02
#define VALUE_COMMAND_RECIVED         0x80

// Commande bytes
#define BYTE_CONTROLER_LOADING      3

// Value Controler bytes

//varibles for datamanagement
#define SIZE_CONTROLLER_DATA         8
unsigned char DataControler[SIZE_CONTROLLER_DATA +2];

//Id for mysensors and for memo
uint16_t MemValueSended[40];

#define ID_POWER_ON                 1
#define ID_BUBBLE_ON                2
#define ID_HEATER_ON                3
#define ID_WATER_FILTER_ON          4
#define ID_VALUE_WATER_JET_ON       5
#define ID_SANITIZER_ON             6

#define ID_FARENHEIT                15


#define ID_SETPOINT_TEMPERATURE     20
#define ID_ACTUAL_TEMPERATURE       21

#define ID_ERROR_CODE               35
#define ID_COM_PUMP                 36

#define SIZE_PUMP_DATA               17
unsigned char Data[SIZE_PUMP_DATA +2];
uint16_t DataCounter=0;

//internal variables
Timer t;
bool FinishPumpMessage;
bool FinishControllerMessage;
char ControllerLoadingState;
bool TempWindowsOpen;
uint16_t state;
uint16_t CommandToSend =0x0000;
bool CommandRecived;
long LastTimeReciveData;
bool ErrorCommunicationWithPump;
long LastTimeSendData;

void setup() {
 
  mySerial.begin(9600);
  Serial.begin(115200);
#ifdef ESP32
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(Myssid, Mypassword);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
#endif
  
#if  defined (_MQTT_) &&  defined (DEBUG_MQTT)
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
#endif  
  pinMode(DO_SET, OUTPUT);        // SET
  pinMode(DO_CS, OUTPUT);        // CS 
  delay(1000);
  
  digitalWrite(DO_CS, LOW);     
  digitalWrite(DO_SET, LOW);
  delay(100);
  // Configure LC12s
  SetSettings();
//configure send alive timer  
#ifdef  __AVR__    
  t.every(800, SendLifeFct, (void*)0); // only the alf time due to softwareserial
#endif
#ifdef  ESP32    
  t.every(1600, SendLifeFct, (void*)0);
#endif  
  LastTimeReciveData = LastTimeSendData= millis();
}

void loop() {
#ifdef DEBUG_RECIEVED_DATA   
   char res[5];  
#endif
   //update timer
   t.update();
#ifdef  __AVR__  
   mySerial.listen();
#endif
#ifdef ESP32
  ArduinoOTA.handle();
#endif
   
   if (!FinishPumpMessage)
   {
#ifdef __AVR__
    //in case of overflow
    if ( mySerial.overflow() ) 
    {
      state = 0;
      DataCounter=0;
      while ( mySerial.available())
        mySerial.read();
    }
    else
#endif    
    {
       if (mySerial.available() ) 
       {
        unsigned char c = mySerial.read();
        ReadData(c);
#ifdef DEBUG_RECIEVED_DATA
        sprintf(&res[0],"%02X",c);
        if (c == FIRST_COMMAND_BYTE || c == FIRST_PUMP_BYTE){
          Serial.println(" ");
        }
          
        Serial.print(res);
        Serial.print(" ");               
#endif      
       }
     }
   }
 
   //Manage pump message
   if (FinishPumpMessage)
   {
     LastTimeReciveData = millis();
 #ifdef _MQTT_ 
     if (client.isConnected())
 #endif
     {

       DataManagement();
       SendCommandManagement (&CommandToSend);

       state =0;
       FirstSend = true;
       FinishPumpMessage = false;  
     }
     
   }
  
   //Manage Controller message
   if (FinishControllerMessage)
   {
      ControllerLoadingState = DataControler[BYTE_CONTROLER_LOADING];
      FinishControllerMessage = false;
   }
#ifdef _MY_SENSORS_   
   MySensorsCommandManagement();
#endif   
#ifdef _MQTT_
  client.loop();
#endif
  // check communication pump timeout
  if (millis() - LastTimeReciveData > 2000 ){
    SendValue("IntexSpa/Communication with pump", false,ID_COM_PUMP); 
    ErrorCommunicationWithPump = true;
  }
  else{
    SendValue("IntexSpa/Communication with pump", true,ID_COM_PUMP); 
    ErrorCommunicationWithPump = false;
  }
  

}

void ReadData (unsigned char c)
{
  
   switch (state) 
   {
      case 0: //wait header
      {
        DataCounter=0;
        memset (&Data,0,sizeof(Data));
        memset (&DataControler,0,sizeof(DataControler));
        
        if (c == FIRST_PUMP_BYTE)
        {
          Data[DataCounter] =c;
          DataCounter++;
          state++;
        }
        break;
      }
      //wait all other data
      case 1:
      {
        Data[DataCounter] =c;
        DataCounter++;

        //it was a message from controler usefull in case of Controler message and pump message start with the same value
        //resete to second byte to continue to read store pump message
        if (c == FIRST_COMMAND_BYTE && DataCounter< SIZE_PUMP_DATA )
        {
       
          memcpy (&DataControler,&Data, 8);
          
          //Calclulate an check Checksum
          uint16_t crc_out = calc_crc((char*) DataControler,SIZE_CONTROLLER_DATA -2);      
          //Checksum ok
          if (   (Data[SIZE_CONTROLLER_DATA-2] == ((crc_out & 0xFF00) >> 8 ))
              && (Data[SIZE_CONTROLLER_DATA-1] == (crc_out & 0x00FF))
             )    
          {
            FinishControllerMessage = true;
          }          
          else
          {
            FinishControllerMessage = false;
          }
          
          
          DataCounter = 0;
          Data[DataCounter] =c;
          DataCounter++;
        }
        // wait until full message is recived
        if (DataCounter> SIZE_PUMP_DATA-1)
        {

          //Calclulate an check Checksum
          uint16_t crc_out = calc_crc((char*)Data,SIZE_PUMP_DATA -2);      
          //Checksum ok
          if (   (Data[SIZE_PUMP_DATA-2] == ((crc_out & 0xFF00) >> 8 ))
              && (Data[SIZE_PUMP_DATA-1] == (crc_out & 0x00FF))
             )    
          {
            state =0;
            FinishPumpMessage = true;
          }
          else{ // Wrong Cheksum
            state =0;
          }
          
        }
        break;
      }
     
   }
}  
#ifdef _MQTT_
void onConnectionEstablished()
{
   //manage command power on/off
   client.subscribe("IntexSpa/Cmd Power on off", [](const String & payload) {
    if (payload== "1" || payload== "0"){
      CommandToSend = COMMAND_ON_OFF ;
      LastTimeSendData = millis();
    }
  });

   //manage command water filter on/off
   client.subscribe("IntexSpa/Cmd water filter on off", [](const String & payload) {
   if (payload== "1" || payload== "0"){
      CommandToSend |= COMMAND_WATER_FILTER;
      LastTimeSendData = millis();
    }
  });

   //manage command Bubble on/off
   client.subscribe("IntexSpa/Cmd bubble on off", [](const String & payload) {
    if (payload== "1" || payload== "0"){
      CommandToSend |= COMMAND_BUBBLE;
      LastTimeSendData = millis();
    }
  });

   //manage command heater on/off
   client.subscribe("IntexSpa/Cmd heater on off", [](const String & payload) {
    if (payload== "1" || payload== "0"){
    CommandToSend |= COMMAND_HEATER;
      LastTimeSendData = millis();
    }    
  });  

   //manage command Farenheit/Celsius
   client.subscribe("IntexSpa/Cmd Farenheit Celsius", [](const String & payload) {
    if (payload== "1" || payload== "0"){
      CommandToSend |= COMMAND_CELSIUS_FARENHEIT;
      LastTimeSendData = millis();
    }      
  }); 

   //manage command decrease
   client.subscribe("IntexSpa/Cmd decrease", [](const String & payload) {
    if (payload== "1")
   {
      LastTimeSendData = millis();
      CommandToSend |= COMMAND_DECREASE;
      if (!TempWindowsOpen){
        delay (200);
        CommandToSend |= COMMAND_DECREASE;
      }
    }
  }); 

   //manage command increase
   client.subscribe("IntexSpa/Cmd increase", [](const String & payload) {
    if (payload== "1" ){
      LastTimeSendData = millis();
      CommandToSend |= COMMAND_INCREASE;
      if (!TempWindowsOpen){
      delay (200);
      CommandToSend |= COMMAND_INCREASE;
      }
    }
  });   


#ifdef _28458_28462_
   //manage command water jet on off
   client.subscribe("IntexSpa/Cmd water jet on off", [](const String & payload) {
    if (payload== "1" || payload== "0"){
      CommandToSend |= COMMAND_WATER_JET;
      LastTimeSendData = millis();
    }

  });   

   //manage command sanizer on off
   client.subscribe("IntexSpa/Cmd sanizer on off", [](const String & payload) {
    if (payload== "1"|| payload== "0"){
      CommandToSend |= COMMAND_SANITIZER;
      LastTimeSendData = millis();
    }
      
  }); 
#endif
  
}
#endif

void DataManagement (){
#ifdef DEBUG_PUMP_DATA
    char res[5]; 
  
    for (uint8_t i =0; i<SIZE_PUMP_DATA;i++){
      sprintf(&res[0],"%02X",Data[i]);
      Serial.print(res);
      Serial.print(" ");

    }
    Serial.println(" ");
#endif

  
   // Send power on
   SendValue("IntexSpa/Power on", (bool)(Data[BYTE_STATUS_COMMAND] & VALUE_CONTROLLER_ON),ID_POWER_ON); 

   //Send Bubble on 
   SendValue("IntexSpa/Bubble on", (bool)(Data[BYTE_STATUS_COMMAND] & VALUE_BUBBLE_ON),ID_BUBBLE_ON); 

   //Send heater on 
   SendValue("IntexSpa/heater on", (bool)(Data[BYTE_STATUS_COMMAND] & VALUE_HEATER_ON),ID_HEATER_ON); 

   //Send water filter on 
   SendValue("IntexSpa/filter on", (bool)(Data[BYTE_STATUS_COMMAND] & VALUE_WATER_FILTER_ON),ID_WATER_FILTER_ON); 
#ifdef _28458_28462_
   //Send water jet on 
   SendValue("IntexSpa/Water jet on", (bool)(Data[BYTE_STATUS_COMMAND] & VALUE_WATER_JET_ON),ID_VALUE_WATER_JET_ON); 

   //Send water sanizer on 
   SendValue("IntexSpa/Sanizer on", (bool)(Data[BYTE_STATUS_COMMAND] & VALUE_SANITIZER_ON),ID_SANITIZER_ON); 
   
#endif   
   
   //Send Farenheit selected
   SendValue("IntexSpa/Farenheit Celsius", (bool)(Data[BYTE_STATUS_STATUS] & VALUE_FARENHEIT),ID_FARENHEIT); 

   //Send temperature setpoint
   SendValue("IntexSpa/Temperature Setpoint", Data[BYTE_SETPOINT_TEMPERATURE],ID_SETPOINT_TEMPERATURE); 

   //Send actual temperature
   SendValue("IntexSpa/Actual Temperature", Data[BYTE_ACTUAL_TEMPERATURE],ID_ACTUAL_TEMPERATURE); 

  //send Error number
   SendValue("IntexSpa/Error Number", Data[BYTE_ERROR],ID_ERROR_CODE); 

  // manage temperature windows open
  if (Data[BYTE_STATUS_STATUS] & VALUE_TEMP_WINDOWS_OPEN){
    TempWindowsOpen = true;
  }
  else{
    TempWindowsOpen = false;
  }

  //Manage Command recived
  if (Data[BYTE_STATUS_STATUS] & VALUE_COMMAND_RECIVED){
    CommandRecived = true;
  }
  else{
    CommandRecived = false;
  }



}


#ifdef _MY_SENSORS_
void MySensorsCommandManagement(){


}

#endif

void SendCommandManagement (uint16_t *Command){
  
  if (!Command || !*Command)
    return;

  if(    CommandRecived
     ||  (millis() -LastTimeSendData> 600)
    )
  {
     *Command = 0x0000;
    return;
  }
  SendCommand(*Command);
  
}




void SendLifeFct(void *context)
{
#ifdef _MQTT_ 
    if (client.isConnected() && !ErrorCommunicationWithPump)
#endif
    {
      SendCommand(0x0000);
    }
}

void SendCommand(uint16_t Command){
  // Sample :C7 00 00 80 00 00 4D 2B
  unsigned char SendData[10];
#ifdef DEBUG_SEND_COMMAND 
  char res[5];
#endif  
  SendData[0] = FIRST_COMMAND_BYTE;
  SendData[1] = (Command & 0xFF00) >> 8;;
  SendData[2] = Command  & 0x00FFF;
  SendData[3] = ControllerLoadingState;  
  SendData[4] = 0x00;
  SendData[5] = 0x00;

  //checksum
  uint16_t crc_out = calc_crc((char*) SendData,6);
  SendData[6] = (crc_out & 0xFF00) >> 8;
  SendData[7] = crc_out & 0x00FFF;
 
  for (uint8_t i =0; i<SIZE_CONTROLLER_DATA;i++){
#ifdef DEBUG_SEND_COMMAND 
    sprintf(&res[0],"%02X",SendData[i]);
    Serial.print(res);
    Serial.print(" ");
#endif    
    mySerial.write(SendData[i]);
  }
#ifdef DEBUG_SEND_COMMAND   
    Serial.println(" ");
#endif    
}


void SetSettings(){
  byte Config[20];
#ifdef DEBUG_CONFIG  
  char res[5];
#endif  
  
  Config[1]=0xAA;  
  Config[2]=0x5A;
  
  //device ID
  Config[3]=0xB9; 
  Config[4]=0x46;

//Network ID
#ifdef _28442_28440_  
  Config[5]=0xFF;
  Config[6]=0XFF;
#endif

#ifdef _28458_28462_  
  Config[5]=0x0D;
  Config[6]=0X05;
#endif
   
  Config[7]=0x00;
  
  //RF Power
  Config[8]=0x03; 
  
  Config[9]=0x00; 

  // Baudrate
  Config[10]=0x04; 
  
  Config[11]=0x00;
  //chanel
#ifdef _28442_28440_   
  Config[12]=0x4A;
#endif
#ifdef _28458_28462_   
  Config[12]=FIRST_COMMAND_BYTE;
#endif
  
  Config[13]=0x00;
  Config[14]=0x00;
  Config[15]=0x00;
  Config[16]=0x12;
  Config[17]=0x00;
  //reset checksum
  Config[18] =0x0;

  //calculate Checsum
  for(int i=1;i<17;i++){

    Config[18] =Config[18] +Config[i];
  }
#ifdef DEBUG_CONFIG
  Serial.print (F(" Config "));
  for(int i=1;i<19;i++){
    sprintf(&res[0],"%02X",Config[i]);
    Serial.print(res);
    Serial.print(F(" "));    
  }
  Serial.println (F(""));
#endif
  digitalWrite(DO_SET, LOW);
  delay(500);

  for(int i=1;i<19;i++){

    mySerial.write(Config[i]);
  }
 delay(1000);
 //Change baudrate
 //mySerial.end();
 //mySerial.begin(19200);
 digitalWrite(DO_SET, HIGH);

}

//CRC calculation 
uint16_t calc_crc(char *msg,int n)
{
  // Initial value. xmodem uses 0xFFFF but this example
  // requires an initial value of zero.
  uint16_t x = 0;

  while(n--) {
    x = crc_xmodem_update(x, (uint16_t)*msg++);
  }
  return(x);
}

// See bottom of this page: http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
// Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
uint16_t crc_xmodem_update (uint16_t crc, uint8_t data)
{
  int i;

  crc = crc ^ ((uint16_t)data << 8);
  for (i=0; i<8; i++) {
    if (crc & 0x8000)
      crc = (crc << 1) ^ 0x1021; //(polynomial = 0x1021)
    else
      crc <<= 1;
  }
  return crc;

}



//---------------------------------------------------
//for sending values
// send uint8_t value only if changed
void SendValue(const String &topic, uint8_t value ,int SENSOR_ID){
  if (value != (uint8_t)MemValueSended[SENSOR_ID]||(!FirstSend && !ErrorCommunicationWithPump))
  {
#ifdef _MY_SENSORS_
 //   send( msg.setSensor( SENSOR_ID ).set( value) );
#endif    
#ifdef _MQTT_
    if (client.isConnected())
      client.publish(topic, String(value)); 
     
#endif
    
#ifdef DEBUG_SEND_VALUE_TO_HOME_AUTOMATION_SW    
    Serial.print(topic);
    Serial.print(" ");
    Serial.println (value);
#endif    
#ifdef _MQTT_
    if (client.isConnected())
#endif
    {
      MemValueSended[SENSOR_ID] = value;
    }
  }   
  
}
