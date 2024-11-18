#include "Keyboard.h"
#include <avr/eeprom.h>

char konto[64];

uint8_t digitalReadByte()
{
  return (digitalRead(A3) << 3) | (digitalRead(A2) << 2) | (digitalRead(A1) << 1) | digitalRead(A0);   
}

void handleInterrupt() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) { 
    previousMillis = currentMillis;
    eeprom_read_block (konto, (void *)(digitalReadByte() * sizeof (konto)), sizeof (konto));
    Keyboard.print(konto);
    delay(400);
    Keyboard.write(KEY_RETURN);
  }
}

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  pinMode (7, INPUT_PULLUP);
  pinMode (A0, INPUT_PULLUP);
  pinMode (A1, INPUT_PULLUP);
  pinMode (A2, INPUT_PULLUP);
  pinMode (A3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(7), handleInterrupt, FALLING);
  eeprom_read_block (konto, (void *)(digitalReadByte() * sizeof (konto)), sizeof (konto)); 
}

void loop() {
  //Serial.println (digitalReadByte());
  //delay (5000);
  if (Serial.available() > 0) {
    int bytesRead = Serial.readBytesUntil('\n', konto, sizeof (konto) - 1);
    konto[bytesRead] = '\0';
    for (int i = 0; i < strlen(konto); i++){
      if (konto[i] == '\\') konto[i] = '\t';
    }
    eeprom_update_block (konto, (void *)(digitalReadByte() * sizeof (konto)), sizeof (konto));
  }
}
