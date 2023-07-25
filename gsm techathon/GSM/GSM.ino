#include <SoftwareSerial.h>

SoftwareSerial mySerial(1,0); //Rx Tx

void setup()
{
  mySerial.begin(9600);//GSM
  Serial.begin(9600); // Serial monitor 
  delay(100);
}

void loop()
{
  mySerial.println("AT+CMGF=1");   // text mode on
  delay(1000);                      
  mySerial.println("AT+CMGS=\"+919003781372\"\r"); 
  delay(1000);
  mySerial.println("SOS");
  delay(100);
  mySerial.println((char)26);
  delay(1000);
}
