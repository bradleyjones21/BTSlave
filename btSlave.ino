#include <SoftwareSerial.h>

SoftwareSerial BT(6,10); // RX, TX

#define INTER_START "INTR"
#define REAC_START  "REAC"
#define TIMEOUT_SLOW 10
#define TIMEOUT_MED 5
#define TIMEOUT_FAST 2.5
int temp = 0;
int temp1 = 0;

void setup() {
  String c;
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
}

void loop() {
  String c;

  if (Serial.available()) {
    c = Serial.readString();
    BT.print(c);
  }
  if (BT.available()) {
    c = BT.readString();
    Serial.print(c);
    temp = c.indexOf(INTER_START);
    temp1 = c.indexOf(REAC_START);
    
    BT.print(c);
    
  }
}
