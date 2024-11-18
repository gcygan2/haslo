#include "Keyboard.h"
#include <avr/eeprom.h>

char konto[64];

void handleInterrupt() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis(); // Pobranie aktualnego czasu
  if (currentMillis - previousMillis >= 1000) { 
    previousMillis = currentMillis;
    Keyboard.print(konto);
    delay(400);
    Keyboard.write(KEY_RETURN);
  }
}

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  pinMode (7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(7), handleInterrupt, FALLING);
  eeprom_read_block (konto, (void *)0, sizeof (konto)); 
}

void loop() {
  if (Serial.available() > 0) {
    int bytesRead = Serial.readBytesUntil('\n', konto, sizeof (konto) - 1);
    konto[bytesRead] = '\0';
    for (int i = 0; i < strlen(konto); i++){
      if (konto[i] == '\\') konto[i] = '\t';
    }
    eeprom_update_block (konto, (void *)0, sizeof (konto));
  }
}
/*
 * 
 */
