#include <Arduino.h>
#if defined(ESP32)

  #include <WiFi.h>

#elif defined(ESP8266)

  #include <ESP8266WiFi.h>

#endif

#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#include <Servo.h>
// Insert your network credentials
#define WIFI_SSID "TP-Link_2678"
#define WIFI_PASSWORD "17248853"
// Insert Firebase project API Key
#define API_KEY "AIzaSyCOc0N402Kx_ou0s7VhDTYKUhdFAOpxirA"
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://hranilica-f9c5a-default-rtdb.europe-west1.firebasedatabase.app/"
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
Servo myServo;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;



bool signupOK = false;

void setup() {

  pinMode(D1, OUTPUT);
  myServo.attach(D3);

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");

    delay(300);

  }

  Serial.println();

  Serial.print("Connected with IP: ");

  Serial.println(WiFi.localIP());

  Serial.println();
  /* Assign the api key (required) */

  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */

  config.database_url = DATABASE_URL;

  /* Sign up */

  if (Firebase.signUp(&config, &auth, "", "")) {

    Serial.println("ok");

    signupOK = true;

  }

  else {

    Serial.printf("%s\n", config.signer.signupError.message.c_str());

  }
  /* Assign the callback function for the long running token generation task */

  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);
randomSeed(analogRead(0));
}


void loop() {
 
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 500 || sendDataPrevMillis == 0)) {

    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getInt(&fbdo, "/hranilica/int")) {

      if (fbdo.dataType() == "int") {

        intValue = fbdo.intData();

        Serial.println(intValue);

        if (intValue >= 1) {

          digitalWrite(D1, HIGH);
          Serial.println("TRUE");  
         if(intValue==1)
          myServo.write(120);
          else{
          myServo.write(intValue); 
          }
          if(intValue==7){
          for(int i=0; i<=7; i++){
            if(i%2==0){
            digitalWrite(D1, HIGH);
          Serial.println("TRUE");
           myServo.write(120); 
              }
                   else {
                    digitalWrite(D1, LOW);
                     Serial.println("false"); 
                     myServo.write(0);
                      }
                     }
                        }
        }

        else if (intValue==0) {
          digitalWrite(D1, LOW);
          Serial.println("false"); 
         myServo.write(0);
         

        }

      }

    }

    else {

      Serial.println(fbdo.errorReason());

    }

 
}
}
