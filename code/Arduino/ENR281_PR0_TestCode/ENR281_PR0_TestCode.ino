


//Motor Front: PWM=D27, Dir=D26
//Motor Rear: PWM=D25, Dir=D33
int R_PWM = 12;
int R_DIR = 14;
int L_PWM = 27;
int L_DIR = 26;

int TX2 = 32;
int RX2 = 33;

const int freq = 20000;
const int rightChannel=0;
const int leftChannel=1;
const int resolution = 15;

int R=0;
int L=0;
int N=0; //keeps track if the previous character was \n

String inString = "";

void setup() {
  //Set control pins to output
  pinMode(R_PWM, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(L_DIR, OUTPUT);
  //initialize pwm channels
  ledcSetup(rightChannel, freq, resolution);
  ledcSetup(leftChannel, freq, resolution);
  ledcAttachPin(R_PWM, rightChannel);
  ledcAttachPin(L_PWM, leftChannel);
  Serial.begin(9600);
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
    if (inChar == '-') {
      N = 1;
    }

    if (inChar == 'R') {
      if (N == 1) {
        digitalWrite(R_DIR, HIGH);
      }
      if (N == 0) {
        digitalWrite(R_DIR, LOW);
      }
      
      int y=(1/32767)*(inString.toInt())*(inString.toInt());
      ledcWrite(rightChannel,(inString.toInt()));
      N = 0; 
      inString = "";
    }
    if (inChar == 'L') {
      if (N == 1) {
        digitalWrite(L_DIR, HIGH);
      }
      if (N == 0) {
        digitalWrite(L_DIR, LOW);
      }
      
      int y=(1/32767)*(inString.toInt())*(inString.toInt());
      ledcWrite(leftChannel,(inString.toInt()));
      N = 0; 
      inString = "";
    }
/*    if (inChar == '\n') {
       //Serial.print("Value:");
       //Serial.println(inString.toInt());
       //Serial.print("String: ");
       //Serial.println(inString);
       
       int y=(1/32767)*(inString.toInt())*(inString.toInt());
       ledcWrite(ledChannel,(inString.toInt()));
       N = 1; 
       inString = "";
       
    }*/
  }
}
