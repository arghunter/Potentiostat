#include <Arduino.h>

// Constants
const int analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17};
const int numPins = 18;
const int sampleRate = 10000; // 10kHz
const int bufferSize = numPins * 2; // Each reading is 2 bytes

// Buffers for double buffering
uint8_t buffer1[2000][bufferSize];
// uint8_t buffer2[bufferSize];
// volatile uint8_t* writeBuffer = buffer1;
// volatile uint8_t* sendBuffer = buffer2;
volatile bool bufferReady = false;
volatile int bufferHead=0;
volatile int bufferTail=2;
volatile int bufferIndex = 0;
bool flip=false;
#define USBSERIAL Serial
// Timer interrupt to read analog values
IntervalTimer sampleTimer;
uint16_t counter=0;
void sampleData() {
  for (int i = 0; i < numPins; i++) {
    int sensorValue = analogRead(analogPins[i]);
    buffer1[bufferTail][bufferIndex++] = counter & 0xFF; // Lower byte
    buffer1[bufferTail][bufferIndex++] = counter>>8; // Upper byte
    
  }
  counter+=1;
  bufferIndex=0;
  bufferTail++;
  if(bufferTail>=2000){
    bufferTail=0;
  }

}

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for the Serial port to be available

  // Initialize the analog pins
  for (int i = 0; i < numPins; i++) {
    pinMode(analogPins[i], INPUT);
  }

  // Configure the ADC for higher speed
  analogReadResolution(10); // 10-bit resolution
  analogReadAveraging(1);   // No averaging

  // Set up the timer interrupt to sample data at 10kHz
  sampleTimer.begin(sampleData, 1000000 / sampleRate);
  delay(2);
}

void loop() {
  
   if (true) {
    // Send the data over USB serial using Serial.write

    static uint8_t localBuffer[36];


 

    while(bufferHead!=bufferTail){
      // noInterrupts();
      // memcpy(localBuffer, buffer1[bufferHead], 36);
      // interrupts();
      Serial.write( buffer1[bufferHead],36);
      bufferHead++;
      if(bufferHead>=2000){
        bufferHead=0;
      }
    }
    // bufferReady = false;
  }
}
  // Wait a bit before checking the buffer again
  

