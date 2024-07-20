// Create an IntervalTimer object 
IntervalTimer myTimer;
int counter=0
const int ledPin = LED_BUILTIN;  // the pin with a LED
int numChannels=18;
int bufferLength=10000
int buffer[numChannels][bufferLength]
int bufferTail=0;
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  myTimer.begin(getData, 100);  // blinkLED to run every 0.15 seconds
}

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
// int ledState = LOW;
// volatile unsigned long blinkCount = 0; // use volatile for shared variables

// functions called by IntervalTimer should be short, run as quickly as
// possible, and should avoid calling other functions if possible.
void getData() {
// Put data into buffer
for (int i=0;i<numChannels;i++)
  buffer[i][buffertail]=counter-i;
}
bufferTail++;

// The main program will print the blink count
// to the Arduino Serial Monitor
void loop() {
  unsigned long blinkCopy;  // holds a copy of the blinkCount


  noInterrupts();
  // blinkCopy = blinkCount;
  interrupts();

  
  delay(100);
}
