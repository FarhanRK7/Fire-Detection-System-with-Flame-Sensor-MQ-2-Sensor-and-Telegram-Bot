#include "CTBot.h"
#include <WiFi.h>
#include <Arduino.h>
#include <ArduinoJson.h>

CTBot myBot;

String ssid = "OkBos";
String pass = "halo1234";
String token = "6456268874:AAFCsVCYsJASZGlAjMdpmVnPObszYxP3JWg";

#define Api 26
#define Gas 25
#define Buzzer 27

bool buzzerActive = false;
unsigned long lastNotificationTime = 0;
const unsigned long notificationInterval = 30000; // 30 seconds

void activateBuzzer() {
  if (!buzzerActive) {
    Serial.println("Activating Buzzer");

    for (int i = 1000; i <= 3000; i += 100) {
      tone(Buzzer, i);
      delay(50);
    }

    delay(3000);

    for (int i = 3000; i >= 1000; i -= 100) {
      tone(Buzzer, i);
      delay(50);
    }

    noTone(Buzzer);
    buzzerActive = true;
  }
}

void sendMessageIfNeeded(const String &message) {
  if (myBot.testConnection()) {
    myBot.sendMessage(1468043335, message);
    myBot.sendMessage(5336458356, message);
    myBot.sendMessage(6240041610, message);
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Failed to send message - no connection");
  }
}

void setup() {
  pinMode(Api, INPUT);
  pinMode(Gas, INPUT);
  pinMode(Buzzer, OUTPUT);
  Serial.begin(9600);
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("\nConnected");
  else
    Serial.println("\nNot Connected");
}

void loop() {
  int bacasensorapi = digitalRead(Api);
  int bacasensorgas = digitalRead(Gas);

  Serial.print("Api : ");
  Serial.print(bacasensorapi);
  Serial.print("   Gas : ");
  Serial.println(bacasensorgas);

  if (bacasensorgas == LOW && !buzzerActive) {
    Serial.println("Gas detected, activating buzzer and sending message");
    activateBuzzer();
    sendMessageIfNeeded("Kebocoran Gas!!! Kebocoran Gas!!! Kebocoran Gas!!! Kebocoran Gas!!!");
  } else if (bacasensorgas == HIGH && buzzerActive) {
    Serial.println("Gas cleared, deactivating buzzer");
    buzzerActive = false;
  }

  if (bacasensorapi == LOW && !buzzerActive) {
    Serial.println("Fire detected, activating buzzer and sending message");
    activateBuzzer();
    sendMessageIfNeeded("Kebakaran!!! Kebakaran!!! Kebakaran!!! Kebakaran!!!");
  } else if (bacasensorapi == HIGH && buzzerActive) {
    Serial.println("Fire cleared, deactivating buzzer");
    buzzerActive = false;
  }

  // Send periodic notifications every 30 seconds
  unsigned long currentTime = millis();
  if (currentTime - lastNotificationTime >= notificationInterval) {
    Serial.println("Sending periodic notification");
    sendMessageIfNeeded("Sistem berjalan normal.");
    lastNotificationTime = currentTime;
  }

  delay(500);
}
