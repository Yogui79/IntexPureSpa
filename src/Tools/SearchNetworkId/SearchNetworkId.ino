/*
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
 *                      
 *                      
 *                       LC12S     Arduino     ESP
 *  ___________________
 * |                  o| GNG       GND         GND
 * |                  o| CS        D5          D18
 * |                  o| SET       D6          D19
 * |                  o| TX        D2          D16
 * |                  o| RX        D4          D17
 * |__________________o| VCC       3.3V        3.3V
*/



uint8_t Channel =0x47; 
//#define SEARCH_NETWORK_ID
uint16_t idtosend =0xFFFC;



#if defined (__AVR__) 
#include <SoftwareSerial.h>
// Output defines
SoftwareSerial mySerial(2, 4); // RX, TX
// Output defines
#define DO_SET    6
#define DO_CS     5
#elif defined (ESP32)
#define mySerial   Serial2
#define DO_SET    19
#define DO_CS     18
#endif

unsigned long LastTimeReciveData;
uint16_t SearchNetworkIdDataCount; 
bool datareceived;
bool NetworkIdOk;



void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);
  pinMode(DO_SET, OUTPUT);        // SET
  pinMode(DO_CS, OUTPUT);        // CS 
  delay(1000);
  
  digitalWrite(DO_CS, LOW);     
  digitalWrite(DO_SET, LOW);
  Serial.println(F(""));
  Serial.println(F("-----------------------------------------------------------")); 
  Serial.println(F(""));
#ifdef SEARCH_NETWORK_ID
  Serial.println (F("          Search Network id active"));
#else
  Serial.println (F("          Check Channel"));
  Serial.println (F(""));
  Serial.println (F("          LED on LC12s should blink fast"));  
#endif
  Serial.println(F(""));
  Serial.println(F("-----------------------------------------------------------"));

  ChangeSettings(idtosend++);
 


  //querryversion();
}

void loop() {
char res[5];  
 #ifdef __AVR__
   mySerial.listen();
   if ( mySerial.overflow() ) 
   {
        Serial.println("overflow");
        while ( mySerial.available() )
          mySerial.read();
   }
   else
#endif  
   {   
     if (mySerial.available()) 
     {
        uint8_t c =mySerial.read();
      
      if (!NetworkIdOk){
        sprintf(&res[0],"%02X",c);
        Serial.write(res);
        Serial.print(" ");
        datareceived = true;
        SearchNetworkIdDataCount++;
      }     
     if (SearchNetworkIdDataCount >1500 && !NetworkIdOk){
        NetworkIdOk = true;
        uint16_t YourNetworkId = idtosend-1;
        sprintf(&res[0],"%02X",YourNetworkId);
        Serial.println (F(""));
        Serial.println (F("-----------------------------------------------------------")); 
        Serial.println (F(""));
        Serial.println (F("          Your network id is :"));
        Serial.println (F(""));
        Serial.print (F("                 0x"));
        Serial.write(res);
        Serial.println (F(""));
        Serial.println (F(""));
        Serial.println (F("-----------------------------------------------------------")); 
     }
      LastTimeReciveData =millis(); 
     }
   }
   
#ifdef SEARCH_NETWORK_ID
   if ((millis()-LastTimeReciveData  > 1000 ) && datareceived ){ 
      Serial.println("");
      datareceived = false;
      SearchNetworkIdDataCount = 0;
      ChangeSettings(idtosend++);
   }

   if (millis()-LastTimeReciveData  > 120000 ){
       Serial.println("");
       Serial.println("Timeout");
      SearchNetworkIdDataCount = 0;
      datareceived = false;
      ChangeSettings(idtosend++);
   }
  #endif 

}


void ChangeSettings(uint16_t id){
  byte Config[20];
  char res[5];
    
  //byte checksum;
  Config[1]=0xAA;  
  Config[2]=0x5A;
  Config[3]=0xB9; 
  Config[4]=0x46; 
  Config[5]=id>>8;
  Config[6]=id;
  Config[7]=0x00;
  Config[8]=0x03; 
  Config[9]=0x00; 
  Config[10]=0x04; 
  Config[11]=0x00;
  Config[12]=Channel;
  Config[13]=0x00;
  Config[14]=0x00;
  Config[15]=0x00;
  Config[16]=0x12;
  Config[17]=0x00;
  Config[18] =0x0;

  //calculate Checksum
  for(int i=1;i<17;i++){
    Config[18] =Config[18] +Config[i];
  }
  
  Serial.println (F(""));                   
  Serial.print (F("Config "));
  for(int i=1;i<19;i++){
    sprintf(&res[0],"%02X",Config[i]);
    Serial.print(res);
    Serial.print(F(" "));    
  }
  Serial.println (F(""));                    

  
  digitalWrite(DO_SET, LOW);
  delay(500);

  for(int i=1;i<19;i++){

    mySerial.write(Config[i]);
  }
 delay(1000);
 digitalWrite(DO_SET, HIGH);
 
}

void querryversion(){
  byte Config[20];
  char res[5];
    
  //byte checksum;
  Config[1]=0xAA;  
  Config[2]=0x5D;
  Config[3]=0x00; 
  Config[4]=0x00; 
  Config[5]=0x00;
  Config[6]=0x00;
  Config[7]=0x00;
  Config[8]=0x00; 
  Config[9]=0x00; 
  Config[10]=0x00; 
  Config[11]=0x00;
  Config[12]=0x00;
  Config[13]=0x00;
  Config[14]=0x00;
  Config[15]=0x00;
  Config[16]=0x00;
  Config[17]=0x00;
  Config[18] =0x07;
  
  Serial.print (F("Querry version "));
  for(int i=1;i<19;i++){
    sprintf(&res[0],"%02X",Config[i]);
    Serial.print(res);
    Serial.print(" ");    
  }

  Serial.println (F(""));
  digitalWrite(DO_SET, LOW);
  delay(500);

  for(int i=1;i<19;i++){

    mySerial.write(Config[i]);
  }
 delay(1000);
 digitalWrite(DO_SET, HIGH);
 
 
}
