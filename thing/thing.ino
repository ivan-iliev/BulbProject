


//#define LOG64_ENABLED
#include <Log64.h>

uint8_t RELAY1_STATE; 



#define RELAY_PIN 15
#define LED_PIN 2


const int AOUTpin=0;

int limit;
int value;


#include "IoTThing.h"



bool FORCE_DATA_MESSAGE;

void received(uint8_t buf[], uint8_t size, int16_t rssi)
{
 
  if (size >= 1)
  {
    switch (buf[0])
    {
      case 0 :
        {
   
          LOG64_SET("RELAY OFF");
          LOG64_NEW_LINE;

          RELAY1_STATE = 0;

          digitalWrite(RELAY_PIN, LOW);
          digitalWrite(LED_PIN, LOW);

          FORCE_DATA_MESSAGE = true;
          break;
        }
      case 1 :
        {
  

          LOG64_SET("RELAY ON");
          LOG64_NEW_LINE;

          RELAY1_STATE = 1;

          digitalWrite(RELAY_PIN, HIGH);
          digitalWrite(LED_PIN, HIGH);

          FORCE_DATA_MESSAGE = true;
          break;
        }

    }

  }
}

#define gateway_id 4212
#define gateway_key "CF971A094C4820EAE257C56A93654D68"

#define ssid  "Wi-Fi_IoT"
#define pass "qwertyui"

#define thing_id 66

uint8_t thing_key[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
IoTThing thing(ssid, pass, thing_id, thing_key, gateway_id, gateway_key, received);


void setup()
{

  
  delay(2000);

 
  LOG64_INIT();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  FORCE_DATA_MESSAGE = true;

  RELAY1_STATE = 0;

  thing.init();

  Serial.begin(9600);
}




void loop()
{


value= analogRead(AOUTpin);
  Serial.println(value);
 if(value<200)
  {
      Serial.println("You are sober.");
  }
  if (value>=200 && value<280)
  {
      Serial.println("Alcohol detected");
  }
  if (value>=280 && value<350)
  {
      Serial.println("More than one drink going on here....");
  }
  if (value>=350 && value <450)
  {
      Serial.println("Serious Booze here! ");
  }
  if(value>450)
  {
     Serial.println("You are drunk!");
  }

  delay (500);




  if (FORCE_DATA_MESSAGE)
  {
    LOG64_SET("ABOUT TO SEND MESSAGE");
    LOG64_NEW_LINE;

    FORCE_DATA_MESSAGE = false;

    uint8_t msg[IoTThing_MAX_MESSAGE_LEN];
    uint8_t msg_size = 0;

    msg[msg_size++] = RELAY1_STATE;

    
    if (thing.is_ready())
    {
      thing.send(msg, msg_size);
    }
    else
    {
     
      thing.cancel();
      thing.send(msg, msg_size);

    }


    LOG64_SET("MESSAGE SENT");
    LOG64_NEW_LINE;

  }

  thing.work();

  yield();
}
