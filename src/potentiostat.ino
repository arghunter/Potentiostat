#include <Arduino.h>
#define USBSERIAL Serial
// Constants
const int analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17};
const int numPins = 18;
const int sampleRate =10000; // 10kHz
const int bufferSize = numPins * 2; // Each reading is 2 bytes
const int F=2;//Frequency
const int Fs = 500;//Sample Rate
const int N=500;//Num Samples
const int pwmPinOut = 8;

const int sampleInterval=1000000/(F*N);


uint8_t buffer1[2000][bufferSize];
volatile bool bufferReady = false;
volatile int bufferHead=0;
volatile int bufferTail=2;
volatile int bufferIndex = 0;
volatile uint8_t samples[N];

// Timer interrupt to read analog values
IntervalTimer sampleTimer;
IntervalTimer outTimer;
uint16_t counter=0;
void sampleData() {
  for (int i = 0; i < numPins; i++) {
    int sensorValue = analogRead(analogPins[i]);
    buffer1[bufferTail][bufferIndex++] = sensorValue & 0xFF; // Lower byte
    buffer1[bufferTail][bufferIndex++] = sensorValue>>8; // Upper byte
  }

  counter+=1;
  bufferIndex=0;
  bufferTail++;
  if(bufferTail>=2000){
    bufferTail=0;
  }

}
void sendSignal(){
  for(int i=0;i<N;i++){
    analogWrite(pwmPinOut, samples[i]);
  }
}
void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for the Serial port to be available
  pinMode(8, OUTPUT);
  int t;
  for(int i=0;i<N;i++){
    t = (float) i/Fs;
    samples[i]= (uint8_t) (127.0 * sin(2*3.1415*F*t) + 127.0);
  }
  // Initialize the analog pins
  for (int i = 0; i < numPins; i++) {
    pinMode(analogPins[i], INPUT);
  }
  // Configure the ADC for high speed
  analogReadResolution(12); // 12-bit resolution
  analogReadAveraging(1);   // No averaging

  // Set up the timer interrupt to sample data at the samplerate
  sampleTimer.begin(sampleData, 1000000 / sampleRate);
  outTimer.begin(sendSignal, sampleInterval);
  delay(2);
}

void loop() {
  
  
    while(bufferHead!=bufferTail){

      Serial.write( buffer1[bufferHead],36);
      bufferHead++;
      if(bufferHead>=2000){
        bufferHead=0;
      }
    }

}
  // Wait a bit before checking the buffer again
  
