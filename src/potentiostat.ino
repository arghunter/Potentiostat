#include <Arduino.h>
#include <avr/pgmspace.h>
// #define USBSERIAL Serial
#define BUF_LENGTH 128  /* Buffer for the incoming command. */

static bool do_echo = true;
// Constants
const int analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17};
const int numPins = 18;
const int bufferSize = numPins * 2; // Each reading is 2 bytes

const int pwmPinOut = 12;

// const int sampleInterval=1000000/(F*N);

int F=1000;//Frequency
int Fs = 3000;//Sample Rate
int N=(int)(Fs/F);//Num Samples
int sampleRate =10000; // 10kHz

uint8_t buffer1[2000][bufferSize];
volatile bool bufferReady = false;
volatile int bufferHead=0;
volatile int bufferTail=2;
volatile int bufferIndex = 0;
volatile uint16_t samples[1000];

// Timer interrupt to read analog values
IntervalTimer sampleTimer;
IntervalTimer outTimer;
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
void sendSignal() {
  static int index = 0;

  analogWrite(pwmPinOut, (int)samples[index]); // Output the current sample

  index++;
  if (index >= N) {
    index = 0; // Loop back to the beginning of the sine wave
  }
}
void setPWM(int frequency, int samplerate){
  F=frequency;
  Fs=samplerate;
  N=(int)(Fs/F);
  for(int i=0;i<1000;i++){
    if(i<N){
    samples[i] = 2048 + 2047 * sin(2 * PI * i / N);
    }else{
      samples[i]=0;
    }
  }
  outTimer.update(1000000 / Fs);
}
void setSampleRate(int samplerate){
  sampleRate=samplerate;
  sampleTimer.update(1000000 / sampleRate);
}
static void exec(char *cmdline)
{
    char *command = strsep(&cmdline, " ");

    if (strcmp_P(command, PSTR("help")) == 0) {
        SerialUSB1 .println(F(
            "mode <pin> <mode>: pinMode()\r\n"
            "read <pin>: digitalRead()\r\n"
            "aread <pin>: analogRead()\r\n"
            "write <pin> <value>: digitalWrite()\r\n"
            "awrite <pin> <value>: analogWrite()\r\n"
            "pwm <frequency> <samplerate(>=2*frequency  <1000*frequency)> : setPWM()\r\n"
            "readrate <samplerate>: setSampleRate()\r\n"
            "echo <value>: set echo off (0) or on (1)"));
    } else if (strcmp_P(command, PSTR("mode")) == 0) {
        int pin = atoi(strsep(&cmdline, " "));
        int mode = atoi(cmdline);
        pinMode(pin, mode);
    } else if (strcmp_P(command, PSTR("read")) == 0) {
        int pin = atoi(cmdline);
        SerialUSB1 .println(digitalRead(pin));
    } else if (strcmp_P(command, PSTR("aread")) == 0) {
        int pin = atoi(cmdline);
        SerialUSB1 .println(analogRead(pin));
    } else if (strcmp_P(command, PSTR("write")) == 0) {
        int pin = atoi(strsep(&cmdline, " "));
        int value = atoi(cmdline);
        digitalWrite(pin, value);
    } else if (strcmp_P(command, PSTR("awrite")) == 0) {
        int pin = atoi(strsep(&cmdline, " "));
        int value = atoi(cmdline);
        analogWrite(pin, value);
    } else if (strcmp_P(command, PSTR("pwm"))==0){
        int frequency = atoi(strsep(&cmdline, " "));
        int samplerate = atoi(cmdline);
        setPWM(frequency,samplerate);
    } else if (strcmp_P(command, PSTR("readrate")) == 0) {
        int samplerate = atoi(strsep(&cmdline, " "));
        setSampleRate(samplerate);
    } else if (strcmp_P(command, PSTR("echo")) == 0) {
        do_echo = atoi(cmdline);
    } else {
        SerialUSB1 .print(F("Error: Unknown command: "));
        SerialUSB1 .println(command);
    }
}
void setup() {
  Serial.begin(115200);
  SerialUSB1.begin(9600);
  

  for(int i=0;i<N;i++){
    
    samples[i] = 2048 + 2047 * sin(2 * PI * i / N);
  }
  // Initialize the analog pins
  for (int i = 0; i < numPins; i++) {
    pinMode(analogPins[i], INPUT);
  }
  // Configure the ADC for high speed
  analogReadResolution(12); // 12-bit resolution
  analogReadAveraging(1);   // No averaging
  analogWriteResolution(12);
  // Set up the timer interrupt to sample data at the samplerate
  outTimer.begin(sendSignal,1000000 / Fs);
  sampleTimer.begin(sampleData, 1000000 / sampleRate);

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
  while (SerialUSB1.available()) {
        static char buffer[BUF_LENGTH];
        static int length = 0;

        int data = SerialUSB1.read();
        if (data == '\b' || data == '\177') {  // BS and DEL
            if (length) {
                length--;
                if (do_echo) SerialUSB1.write("\b \b");
            }
        }
        else if (data == '\r') {
            if (do_echo) SerialUSB1.write("\r\n");    // output CRLF
            buffer[length] = '\0';
            if (length) exec(buffer);
            length = 0;
        }
        else if (length < BUF_LENGTH - 1) {
            buffer[length++] = data;
            if (do_echo) SerialUSB1.write(data);
        }
    }

}
 
  
