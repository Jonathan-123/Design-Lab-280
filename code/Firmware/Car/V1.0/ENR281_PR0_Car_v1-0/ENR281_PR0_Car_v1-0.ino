//Control Firmware for Prototype 0 for the car for ENR 281
//  -a basic tank drive system
//  -uses ESP-32 11
//  -to be used in conjunction with the shell scripts found in software - rightJoystick.sh, leftJoystick.sh, and remote.sh 

//Written by Jonathan Ganzer 2/6/2023

int Hours=23;



int R_PWM = 12;     //Right Motor PWM Signal pin = D12
int R_DIR = 14;     //Right Motor Direction Pin = D14
int L_PWM = 27;     //Left Motor PWM Signal pin = D27
int L_DIR = 26;     //Left Motor Direction Pin = D26

int TX2 = 33;       //Green Wire, goes to HC12 RX
int RX2 = 32;       //Yellow Wire, goes to HC12 TX

const int freq = 20000;     //PWM Frequency
const int rightChannel=0;   //Right Motor PWM Channel
const int leftChannel=1;    //Left Motor PWM Channel
const int resolution = 15;  //PWM Signal Resolution = 15bit

int R=0;      //tracks whether the input is for the Right Motor
int L=0;      //tracks whether the input is for the Left Motor
int N=0;      //tracks if the number is negative

String inString = "";     //Stores the input

void setup() {
  
  pinMode(R_PWM, OUTPUT);       //Set control pins to output
  pinMode(R_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(L_DIR, OUTPUT);
  
  ledcSetup(rightChannel, freq, resolution);     //initialize pwm channels
  ledcSetup(leftChannel, freq, resolution);
  ledcAttachPin(R_PWM, rightChannel);            //attach the PWM Channels to the appropriate pins
  ledcAttachPin(L_PWM, leftChannel);
  Serial.begin(9600);                            //initialize Serial Ports
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  //Serial.begin(230400);
  while (!Serial) {
     ;
  }
  Serial.println("Waiting for input");


}

void loop() {

  if ( Serial2.available() > 0){
    int inChar = Serial2.read();
    if (isDigit(inChar)) {
                                      //convert incoming byte to a char and add it to the string
      inString += (char)inChar;
    }
    if (inChar == '-') {              //if the character is -, set N=1 to signify the number will be negative
      N = 1;
    }

    if (inChar == 'R') {              //if the character is R, set R_DIR depending on the sign of the number
      if (N == 1) {
        digitalWrite(R_DIR, HIGH);
      }
      if (N == 0) {
        digitalWrite(R_DIR, LOW);
      }
      
      ledcWrite(rightChannel,(inString.toInt()));     //PWM Output
      N = 0;                                          //Reset Variables
      inString = "";
    }
    
    if (inChar == 'L') {              //if the character is L, set L_DIR depending on the sign of the number
      if (N == 1) {
        digitalWrite(L_DIR, HIGH);
      }
      if (N == 0) {
        digitalWrite(L_DIR, LOW);
      }
      
      ledcWrite(leftChannel,(inString.toInt()));      //PWM Output
      N = 0;                                          //Reset Variables
      inString = "";
    }
  }
}
