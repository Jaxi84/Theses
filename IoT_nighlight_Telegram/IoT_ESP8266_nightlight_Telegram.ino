/**************************************************************************
*- JANI REKINEN - KTTIVI20EB OSAO 2022                                    *
*Based on Brian Lough's Universal Telegram Bot Library from GitHub		  	*
*https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot				    *
*https://github.com/witnessmenow/Simple-Home-Automation-With-Telegram	    *
*Idea from the videoes below                                       		  	*
*https://www.youtube.com/watch?v=yXnfpvCrU_I							              	*
*https://www.youtube.com/watch?v=-IC-Z78aTOs                             	*
****************************************************************************/
//Define used libraries such as used board and used wifi client
#include <ESP8266WiFi.h>        //used board here such as ESP32, Arduino MKR1010 etc
#include <WiFiClientSecure.h>   //Wifi libraries for used board; MKR1010 uses WiFiNINA.h
#include <ArduinoJson.h>        //Library used for parsing Json from the API responses
  // https://github.com/bblanchon/ArduinoJson
#include <UniversalTelegramBot.h>
  // https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

// Wifi network station credentials
#define WIFI_SSID "xxxx"        //Inser WIFI SSID name here
#define WIFI_PASSWORD "xxxx"    //Insert WIFI password here, use password, not key
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "xxxxxxxxxxxxxx"  //Insert Token key here

WiFiClientSecure secured_client;  // Wifi client that supports HTTPS
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

int delayBetweenChecks = 1000;
unsigned long lastTimeChecked;   //last time messages' scan has been done

//Define used pins such as LED, TMP, LDR etc...
#define LED_PIN 4
unsigned long lightTimerExpires;
boolean lightTimerActive = false;
#define tempPin 5
String temp;
int sensorTemp = 0;
#define ldrPin 2
int sensorLDR = 0;
#define sensorPin  A0


/********
 Setup
 ********/
void setup() {
  
  Serial.begin(115200);                    //!! with WiFi connection you are mostly using 115200 as baud rate!!
  Serial.println();

  // Define connections
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  pinMode(tempPin, OUTPUT);
  digitalWrite(tempPin, HIGH);
  pinMode(ldrPin, OUTPUT);
  digitalWrite(ldrPin, LOW);

  // Attempt to connect to Wifi network:
  configTime(0, 0, "time2.mikes.fi");      //in Finland we can use time2.mikes.fi as time server
  //configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  // Add root certificate for api.telegram.org
  secured_client.setTrustAnchors(&cert); 
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  
  bot.longPoll = 60;
}

/**********
 Messages
 **********/
void handleNewMessages(int numNewMessages) {
  
  for (int i = 0; i < numNewMessages; i++) {
    // If the type is a "callback_query", a inline keyboard button was pressed
    if (bot.messages[i].type ==  F("callback_query")) {
      String text = bot.messages[i].text;
      Serial.print("Call back button pressed with text: ");
      Serial.println(text);
      
      // Describe and define the buttons
      
      if (text == F("ON")) {                 //LED ON
        //digitalWrite(LED_BUILTIN, LOW);    //!! In used 8266 board Built-in LED works contrariwise as LOW as power-on and HIGH as power-off
        digitalWrite(LED_PIN, HIGH);
              
      } else if (text == F("OFF")) {         //LED OFF
        //digitalWrite(LED_BUILTIN, HIGH);   //!! In used 8266 board Built-in LED works contrariwise as LOW as power-on and HIGH as power-off
        digitalWrite(LED_PIN, LOW);

      
      } else if (text.startsWith("TIME")) {  //Timer section
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        text.replace("TIME", "");
        int timeRequested = text.toInt();
        //digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LED_PIN, HIGH);
        lightTimerActive = true;
        lightTimerExpires = millis() + (timeRequested * 1000 * 60);
        bot.sendMessage(chat_id, "Timer ON");
      
      } else if (text == F("TEMP")) {        //Temperature section code and conversion  
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        float sensorTemp = analogRead(sensorPin);
        Serial.println(sensorTemp);
        // converting that reading to voltage, for 3.3V arduino use 3.3 / 5V use 5.0
        float voltage = sensorTemp * (5.0 / 1024.0);    
        Serial.print(voltage); Serial.println(" volts");
        //converting from 10 mv per degree with 500 mV offset //to degrees ((voltage - 500mV) times 100)
        float temperature = (voltage - 0.2) * 10 ;      
        Serial.print(temperature); Serial.print(" \xC2\xb0"); Serial.println("C");  //in many cases using \xC2\xb0 C (°C) as degree symbol might arrange you some extra points
        temp = String(temperature);
        bot.sendMessage(chat_id, "Temperature is: " + temp + "°C"); 

      } else if (text == F("LDR")) {         //Light Dependent Resistor as photoresistor section code and limit & reactions  
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        digitalWrite(tempPin, LOW);         //with ESP8266 board we have just one Analog pin, so we need to write another users low
        digitalWrite(ldrPin, HIGH);         //before we write measurable pin high
        delay(500);
        float sensorLDR = analogRead(sensorPin);
        // Serial.println(sensorLDR);       //for serial printing and debuging
        
        if (sensorLDR < 200) {
            digitalWrite(LED_PIN, HIGH);
        //    Serial.print("dim ");Serial.println(sensorLDR); //for serial printing and debuging
            bot.sendMessage(chat_id, "It's dim");
        } else {
            digitalWrite(LED_PIN, LOW);
        
       //   Serial.print("bright ");Serial.println(sensorLDR);  //for serial printing and debuging
           bot.sendMessage(chat_id, "It's bright");
        }
        digitalWrite(ldrPin, LOW);        //remember to write analog pins as they were before this section
        digitalWrite(tempPin, HIGH);
     }
    } else {
      String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;
      
     if (text == F("/options")) {
        // Keyboard Json is an array of arrays.
        // The size of the main array is how many row options the keyboard has
        // The size of the sub arrays is how many coloums that row has
          // "The Text" property is what shows up in the keyboard
          // The "callback_data" property is the text that gets sent when pressed  
        String keyboardJson = F("[[{ \"text\" : \"ON\", \"callback_data\" : \"ON\" },{ \"text\" : \"OFF\", \"callback_data\" : \"OFF\" }],");
        keyboardJson += F("[{ \"text\" : \"10 Mins\", \"callback_data\" : \"TIME10\" }, { \"text\" : \"20 Mins\", \"callback_data\" : \"TIME20\" }, { \"text\" : \"30 Mins\", \"callback_data\" : \"TIME30\" }],");
        keyboardJson += F("[{ \"text\" : \"TEMP\", \"callback_data\" : \"TEMP\" }, { \"text\" : \"LDR\", \"callback_data\" : \"LDR\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "This is an IoT nightlight", "", keyboardJson);
    }
      // When a user first uses a bot they will send a "/start" command
      // So this is a good place to let the users know what commands are available
      if (text == F("/start")) {
        bot.sendMessage(chat_id, "/options : returns the inline keyboard\n", "Markdown");
      }
    }
  }
}


/******
 Loop
 ******/
void loop() {
  
  if (millis() > lastTimeChecked + delayBetweenChecks)  {
    // getUpdates returns 1 if there is a new message from Telegram
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
   if (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
   }
   
    lastTimeChecked = millis();
    
   if (lightTimerActive && millis() > lightTimerExpires) {
      lightTimerActive = false;
      digitalWrite(LED_PIN, LOW);
   //   Serial.println("Time's up! Lights off");  //for serial printing and debuging
   }
  }
}
