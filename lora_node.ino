/*
LoRa_Sender_MQTT:
Support Devices: LoRa Shield + Arduino
Requires Library:
 https://github.com/sandeepmistry/arduino-LoRa

Example sketch sends a message every 30 seconds using a simple protocol
which will be processed by the Dragino Gateway device to send the payload
on to a host IoT platform.
The End node will send out a message string:
 "<device_ID>field1=${tem}&field2=${hum}" (ThingSpeak format)
or
 “<device_ID>Temp=${tem} Hum=${hum}” (General format)
When the LG01/LG02 gateway gets the data, it will parse the data string and
forward the data to the IoT platform via MQTT protocol.
Message information is also output to the Arduino Serial Monitor.
Modified Nov 3 2019
by Dragino Technology Co., Limited <support@dragino.com>

mosquitto_pub.exe -t dragino/367680/data -m 7456;0

mqtt_process.sh

*/
#include <SPI.h>
#include <LoRa.h>
long tem,hum;
int count=1;

int device_id=9000; // ID of this End node.
 // Match to Gateway Channel Local ID definition

int pinOut = 4;
 
void setup() {
 Serial.begin(115200);
 while (!Serial);
 Serial.println("LoRa Sender");
 if (!LoRa.begin(868000000)) { // Match to Gateway frequency setting
 Serial.println("Starting LoRa failed!");
 while (1);
 }
 pinMode(4, OUTPUT);
 
 LoRa.setSyncWord(0x34); // Match to Gateway sync setting
 LoRa.onReceive(onReceive); 
 LoRa_rxMode();
}

void loop() {

 if (runEvery(10000)) { // repeat every 60 seconden
    LoRa_sendMessage();
  }

}

void LoRa_sendMessage() {

  tem = random(25,35); // Generate a random temperature.
  hum = random(85,95); // Generate a random humidity.
  //Serial.print("Sending packet: "); Serial.print(count);
  //Serial.print(" device_id: "); Serial.print(device_id);
  //Serial.print(" tem: "); Serial.print(tem);
  //Serial.print(" hum: "); Serial.println(hum);
/*
  Serial.print(" Data: <");
  Serial.print(device_id);
  // LoRa.print(">field1="); // ThingSpeak
  Serial.print(">Temp="); // General
  Serial.print(tem);
  // LoRa.print("&field2="); // ThingSpeak
  Serial.print(" Hum="); // General
  //Serial.print(";"); // General
  Serial.println(hum);
  Serial.print(" Count="); // General
  Serial.println(count); // General
  */
   Serial.print("<");
  Serial.print(device_id);
  Serial.print(">"); 
  //Serial.print("\"");
  Serial.print(tem);
  Serial.print(";");
  Serial.println(hum);
  //Serial.println("\"");
  LoRa_txMode();                        // set tx mode
  // compose and send packet
  LoRa.beginPacket();

  //LoRa.print("<");
  //LoRa.print(device_id);
  // LoRa.print(">field1="); // ThingSpeak
  //LoRa.print(">Temp="); // General
  //LoRa.print(tem);
  //LoRa.print("&field2="); // ThingSpeak
  //LoRa.print(" Hum="); // General
  //LoRa.print(hum);
  
  //LoRa.print(" Count="); // General
  //LoRa.print(count); // General
  
  LoRa.print("<");
  LoRa.print(device_id);
  LoRa.print(">"); 
  //LoRa.print("\"");
  LoRa.print(tem);
  LoRa.print(";");
  LoRa.print(hum);
  //LoRa.print("\"");
  // count++;
  // LoRa.print(counter);
  LoRa.endPacket();
  LoRa_rxMode();                        // set rx mode

}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");
  String bericht = "";
  char message;
  int ignore = 0;
  
  // read packet
  for (int i = 0; i < packetSize; i++) {

    message = (char)LoRa.read();
    Serial.print(message);
    
    
    
    if (message != ';' & ignore == 0) {
      bericht.concat(message);

    } else {
      ignore = 1;
    }
    
  }
   
   Serial.println();
   Serial.println("---");
   Serial.println(bericht);

   Serial.println(message);

   if( strcmp("7456","7456") == 0){
     if (message == '1') {
       digitalWrite(pinOut, LOW);
       Serial.println("LOW");
     }
     else {
       digitalWrite(pinOut, HIGH);
       Serial.print("HIGH");
     } 
   }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());



}

void LoRa_rxMode(){
  //LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  //LoRa.disableInvertIQ();               // normal mode
}


boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
// End of code
