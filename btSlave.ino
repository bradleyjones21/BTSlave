#include <SoftwareSerial.h>

SoftwareSerial BT(6,10); // RX, TX

#define INTER_START "INTR"
#define REAC_START  "REAC"
#define MISS_MODE   "MISS"
#define TIMEOUT_SLOW 10
#define TIMEOUT_MED 5
#define TIMEOUT_FAST 2.5

const int buttonPin = 9;
int buttonState;
int temp = 0;
boolean miss_mode = false;
int ovfl = 0;
int timer = 0;
boolean had_vibration = false;
String tempStr = "";
void setup() {
  String c;
  pinMode(buttonPin, INPUT);
  buttonState = 0;
  Serial.begin(115200);
  BT.begin(115200);
  delay(100);
  BT.write("AT+DEFAULT\r\n");
  delay(100);
  BT.write("AT+BAUD4\r\n");
  delay(100);
  Serial.end();
  BT.end();
  Serial.begin(9600);
  BT.begin(9600);
  delay(1000);
  BT.write("AT+DEFAULT\r\n");
  delay(100);
  BT.write("AT+STARTEN0\r\n");
  delay(100);
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= 0b00000001;
  TIMSK1 &= 0b00000000;
  TCNT1 = 0;
  //pcint1
  PCICR |= 0b00000111;    // turn on all ports
  PCMSK2 |= 0b00000100;
  sei();
  
  
  
}

ISR(TIMER1_OVF_vect){
  ovfl++;
}

ISR(PCINT0_vect){
  buttonState = 1;
}


void loop() {
  String c;
  if(buttonState==1){
      Serial.print("Button pressed\n");
  }
  if (Serial.available()) {
    //c = Serial.readString();
    //BT.print(c);
  }
  if (BT.available()) {
    c = BT.readString();
    c = c + "\n";
    Serial.print(c);
    BT.print(c);
    delay(100);
    temp = c.indexOf(REAC_START);
    if (temp == 0){
      miss_mode = false;
      //buzzer on 
      //led on
      //start/reset timer
      TIMSK1 |= 0b00000001;
      TCNT1   = 0;
      ovfl    = 0;
      while(true){
        Serial.print("r\n");
        delay(10);
        timer = (TCNT1 + ovfl*65536)/16000; //time in ms
        buttonState = digitalRead(buttonPin);
        if (timer > TIMEOUT_SLOW*1000){
          //BT.print(TIMEOUT_SLOW,"\n");
          TIMSK1 &= 0b00000000;
          timer+="\n";
          BT.print(timer);
          Serial.print(timer);
          break;
        }
        if(buttonState=1){
          timer+="\n";
          BT.print(timer);
          TIMSK1 &= 0b00000000;
          break;
        }
      }
      //turn on led and buzzer
      //start timer
      //wait until either device vibration or timeout
      //return time message to master
    }
    temp = c.indexOf(INTER_START);
    if (temp == 0){
      miss_mode = false;
      //turn on led and buzzer
      //delay by the amount received
      //end
    }
    temp = c.indexOf(MISS_MODE);
    if (temp == 0){
      miss_mode = true;
    }
  }
}
