#define RXD2 26 //33, 26
#define TXD2 27 //32, 27
//#define HC12 Serial2

void setup() {
  

  Serial.begin(9600);
  //Serial2.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
 
}

void loop() {
   while (Serial2.available()) {
     Serial.write(Serial2.read());
   }
   while (Serial.available()) {
    Serial2.write(Serial.read());
   }

}
