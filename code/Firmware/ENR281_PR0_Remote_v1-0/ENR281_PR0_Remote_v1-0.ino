//Remote Firmware for Prototype 0 for ENR 281
//  -Takes an input from Serial0 (the USB port)
//   and passes it along to Serial1 (the HC12)
//  -Uses ESP-32 1
//      -Note: ESP-32 1 has a suspected dead Serial Channel
//       (Serial2).  Try to avoid using unless necessary
//          *Correction: it appears to be functioning now.


//Written by Jonathan Ganzer 2/6/2023

int Hours = 2;


#define RXD2 26     //Yellow Wire, goes to HC12 TX - 32, 26
#define TXD2 27     //Green Wire, goes to HC12 RX  - 33, 27
//#define HC12 Serial2

void setup() {
  

  Serial.begin(9600);       //initialize the Serial Ports
  
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
 
}

void loop() {
   while (Serial1.available()) {      //passes data from Serial0 to Serial2
     Serial.write(Serial1.read());    
   }
   while (Serial.available()) {       //passes data from Serial2 to Serial0.  
    Serial1.write(Serial.read());     // -Used for receiving data from car 
   }                                  // -will likely never be used

}
