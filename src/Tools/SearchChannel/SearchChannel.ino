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
 *   Pin connection
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

uint8_t test= 0x00;



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
#endif


void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);

  pinMode(DO_SET, OUTPUT);        // SET
  pinMode(DO_CS, OUTPUT);        // CS - можно просто соединить с GND
  delay(1000);
  
  digitalWrite(DO_CS, LOW);     // притягиваем к массе 
  digitalWrite(DO_SET, LOW);
  delay(100);

 // querryversion();


}

void loop() {

  SetSettingsChannel(test++);
  Serial.print("Channel "); 
  Serial.println(test-1, HEX);
  delay(1000);
}





void SetSettingsChannel(uint8_t channelid){
  byte Config[20];
  char res[5];
  
  
  Config[1]=0xAA;  
  Config[2]=0x5A;
  
  //device ID
  Config[3]=0xB9; 
  Config[4]=0x46;


//Network ID
  Config[5]=0x00;
  Config[6]=0X00;
   
  Config[7]=0x00;
  
  //RF Power
  Config[8]=0x03; 
  
  Config[9]=0x00; 

  // Baudrate
  Config[10]=0x04; 
  
  Config[11]=0x00;
  //channel
  Config[12]=channelid;
  
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
/*
  Serial.print (F(" Config "));
  for(int i=1;i<19;i++){
    sprintf(&res[0],"%02X",Config[i]);
    Serial.print(res);
    Serial.print(F(" "));    
  }
  Serial.println (F(""));
*/
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
