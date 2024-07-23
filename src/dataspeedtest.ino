// Create an IntervalTimer object
#define USBSERIAL Serial  
IntervalTimer myTimer;
int counter=0;
const int ledPin = LED_BUILTIN;  // the pin with a LED
const int numChannels=18;
const int bufferLength=1000;
int flip=0;
int buffer[bufferLength][numChannels];
int bufferTail=0;
int bufferHead=0;
void setup() {
  pinMode(ledPin, OUTPUT);
  USBSERIAL.begin(9600);
  USBSERIAL.setTimeout(0);
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
  for (int i=0;i<numChannels;i++){
    buffer[bufferTail][i]=counter;
  }
  bufferTail++;
  if(bufferTail>=bufferLength){
    bufferTail=0;
  }
  counter++;
  if(bufferHead==bufferTail){

flip=1;}
  
}

// The main program will print the blink count
// to the Arduino Serial Monitor
void loop() {
  if(flip==0){
  while(bufferTail!=bufferHead){
    String out="[";
    for(int i=0;i<numChannels;i++){
      USBSERIAL.write(buffer[bufferHead][i]);
      // out+=",";
    }
    // USBSERIAL.write(out);
    bufferHead++;
    if(bufferHead>=bufferLength){
      bufferHead=0;
    }
    USBSERIAL.println("");
  }
  }else{
    USBSERIAL.println("DFJKHFJKHESADFJKLAGSDFJLGAWKUFYGEFHBHMDSNHBVFJHTDSFVJHYKSDGFKUYWAEYGFOULEGUKfGKYTFE");
  }



  // noInterrupts();
  // // blinkCopy = blinkCount;
  // interrupts();

  
  // delay(100);
}
