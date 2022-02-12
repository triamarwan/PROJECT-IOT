#include <ESP8266WiFi.h>
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
const char *ssid =  "A83";     
const char *pass =  "gocengdulu"; 

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "triamelia"
#define MQTT_PASS "aio"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200);
Servo tap_servo;
Servo tap_servo2;

int sensormasuk = D5;
int sensormasuk2 = D6;
int Entryservo = D8;
int Exitservo = D7; 
int sensorkeluar = D3;
int sensorkeluar2 = D4;
int slot1IrPin = D0; 
int slot2IrPin = D1;
int slot3IrPin = D2;
int count = 3;
int  hh, mm, ss;

String h, m,Greetings,Slot_1,Slot_2,Slot_3,EntryTimeSlot1,ExitTimeSlot1,EntryTimeSlot2,ExitTimeSlot2,EntryTimeSlot3,ExitTimeSlot3;
boolean entrysensor, exitsensor,entrysensor2, exitsensor2,s1,s2,s3;

boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Set up the feed you're publishing to
Adafruit_MQTT_Publish EntrySlot1 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/EntrySlot1");
Adafruit_MQTT_Publish ExitSlot1 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitSlot1");
Adafruit_MQTT_Publish EntrySlot2 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/EntrySlot2");
Adafruit_MQTT_Publish ExitSlot2 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitSlot2");
Adafruit_MQTT_Publish EntrySlot3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/EntrySlot3");
Adafruit_MQTT_Publish ExitSlot3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/ExitSlot3");
Adafruit_MQTT_Publish Slot1 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Slot1");
Adafruit_MQTT_Publish Slot2 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Slot2");
Adafruit_MQTT_Publish Slot3 = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Slot3");
Adafruit_MQTT_Publish Greeting = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Greeting");

void setup(){
  Serial.begin(9600);
  timeClient.begin(); 
  tap_servo.attach(Entryservo);
  tap_servo2.attach(Exitservo);
  pinMode(sensormasuk,INPUT);
  pinMode(slot1IrPin, INPUT);
  pinMode(slot2IrPin, INPUT);
  pinMode(slot3IrPin, INPUT);
  WiFi.begin(ssid, pass);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(ssid);                          // display ssid
  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");                          // if not connected print this
  delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());       
}

void loop(){
 MQTT_connect();
 timeClient.update();
 hh = timeClient.getHours();
 mm = timeClient.getMinutes();
 ss = timeClient.getSeconds();
 h= String(hh); 
 m= String(mm);
 h +" :" + m;

 entrysensor= !digitalRead(sensormasuk);
 exitsensor = !digitalRead(sensorkeluar);
 entrysensor2= !digitalRead(sensormasuk2);
 exitsensor2 = !digitalRead(sensorkeluar2);
 
 /////Pintu Masuk
    if (entrysensor == 1){
    tap_servo.write(160);
  }
    if (entrysensor2 == 1){
  tap_servo.write(45);
  count=  count-1;
  Serial.println("Slot Tersedia = ");
  Serial.println(count);
  }
  
  //////PINTU KELUAR
   if (exitsensor == 1){
  tap_servo2.write(160);
  }
  if (exitsensor2 == 1)  {
  tap_servo2.write(45);
  count= count+1; 
    Serial.println(count);
  }
   
// Check Slot 
 s1 = digitalRead(slot1IrPin);
 s2 = digitalRead(slot2IrPin);
 s3 = digitalRead(slot3IrPin);

   // Check Slot 1
  if (s1_occupied == false && s1 == 1) {                     
       Slot_1= "Available"; 
       Serial.println(" Slot 1 = ");
       Serial.println(Slot_1);      
if (! Slot1.publish((char*) Slot_1.c_str())){ }

        ExitTimeSlot1 =  h +" :" + m;
        Serial.println("ExitTimeSlot1 = ");
        Serial.print(ExitTimeSlot1);
        
        s1_occupied = true;
        if (! ExitSlot1.publish((char*) ExitTimeSlot1.c_str())){}
    }
  if( s1_occupied == true && s1 == 0 ) {
       Slot_1= "Full"; 
       Serial.println(" Slot 1= ");
       Serial.println(Slot_1);      
if (! Slot1.publish((char*) Slot_1.c_str())){ }

       EntryTimeSlot1 =  h +" :" + m;
       Serial.println("EntrytTimeSlot1= ");
       Serial.println(EntryTimeSlot1);
     
       s1_occupied = false;
        if (! EntrySlot1.publish((char*) EntryTimeSlot1.c_str())){} 
}
    // Check Slot 2
if ( s2_occupied == false && s2 == 1) {                     
      Slot_2= "Available"; 
      Serial.println(" Slot 2 = ");
      Serial.println(Slot_2);      
if (! Slot2.publish((char*) Slot_2.c_str())){ }

      ExitTimeSlot2 =  h +" :" + m;
      Serial.println("ExitTimeSlot2 = ");
      Serial.println(ExitTimeSlot2);
     
      s2_occupied = true;
      if (! ExitSlot2.publish((char*) ExitTimeSlot2.c_str())){}
}
      if(s2_occupied == true && s2 == 0 ) {
      Slot_2= "Full"; 
      Serial.println("Slot 2 = ");
      Serial.println(Slot_2);      
if (! Slot2.publish((char*) Slot_2.c_str())){ }
       
       EntryTimeSlot2 =  h +" :" + m;
       Serial.println("EntryTimeSlot2 = ");
       Serial.println(EntryTimeSlot2);
       
       s2_occupied = false;
        if (! EntrySlot2.publish((char*) EntryTimeSlot2.c_str())){}
  }
    
   // Check Slot 3
if (s3_occupied == false && s3 == 1) {                     
      Slot_3= "Available "; 
      Serial.println("Slot 3 = ");
      Serial.println(Slot_3);      
if (! Slot3.publish((char*) Slot_3.c_str())){ }
      
      ExitTimeSlot3 =  h +" :" + m;
      Serial.println("ExitTimeSlot3 = ");
      Serial.println(ExitTimeSlot3);
      s3_occupied = true;
       if (! ExitSlot3.publish((char*) ExitTimeSlot3.c_str())){}
    }
  if( s3_occupied == true && s3 == 0 ) {
      Slot_3= "Full "; 
      Serial.println(" Slot 3 = ");
      Serial.println(Slot_3); 
 if (! Slot3.publish((char*) Slot_3.c_str())){ }     
  
       EntryTimeSlot3 =  h +" :" + m;
       Serial.println("EntryTimeSlot3 = ");
       Serial.println(EntryTimeSlot3);
       s3_occupied = false;
        if (! EntrySlot3.publish((char*) EntryTimeSlot3.c_str())){ }
  } 


//Case Park Full
  if (count > 0){
     Greetings= "Wellcome To Our Parking Slot";
     Serial.println("Wellcome To Our Parking Slot");
     if (! Greeting.publish((char*) Greetings.c_str())){ }
     delay(5000);
  }
  
  if( count == 0) {
     tap_servo.write(45);
     Serial.println("I'm Sorry, Our Parking is Full.No Available Slots");  //print slot1 and slo2 available
     Greetings= "I'm Sorry, Our Parking is Full.No Available Slots";
     if (! Greeting.publish((char*) Greetings.c_str())){ }
     delay(5000);
     }
         }  

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       mqtt.disconnect();
       delay(3000);  // wait 3 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}
  
