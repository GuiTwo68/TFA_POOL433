#include <TFA_POOL433.h>

TFA433 tfa = TFA_POOL433(); 

void setup() {
  Serial.begin(57600);
  tfa.start(2);//Input pin where 433 receiver is connected.
}

void loop() {
  if (tfa.isDataAvailable()) {
	char txt[100];
//Pointer results:
//    byte id = 0, channel = 0, humidity = 0;
//    int temperature = 0;
//    bool battery = false;
//    tfa.getData(id, channel, humidity, temperature, battery);
//    sprintf(txt, "id: %d, channel: %d, temperature: %d.%d C, battery: %s\n", id, channel, temperature / 100, temperature % 100, (battery ? "OK" : "NOK"));
//    Serial.print(txt);
//Struct results:
	tfaResult result = tfa.getData();
	sprintf(txt, "id: %d, channel: %d,  temperature: %d.%d C, battery: %s\n", result.id, result.channel,  result.temperature / 100, result.temperature % 100, (result.battery ? "OK" : "NOK"));
	Serial.print(txt);
  }
}
