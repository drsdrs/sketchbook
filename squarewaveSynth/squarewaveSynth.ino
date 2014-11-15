#include "TimerOne.h"  

unsigned long previousMillis = 0;        // will store last time LED was updated
byte led = 13;
int periodOld = 100;
int period = 100;
byte r = 0;

byte incomingByte ;
byte action = 0;
byte note = 0;
byte velocity = 0;

byte i =0;

byte channels = 4;
int freq[4]= {0, 0, 0, 0};
byte cnt[4]= {0, 0, 0, 0};
byte state[4]= {0, 0, 0, 0};
byte spkState = 0;
byte bitState = 0b0000;

void setup() {
  
  Serial.begin(31250);
    
  pinMode(led, OUTPUT);
  DDRB = DDRB | 0b111111;
  Timer1.initialize(period);
  Timer1.attachInterrupt(genAudio);

}

void loop() {
  
   if (Serial.available() > 0) {
      processMidiMsg();
   }
   
   period = (analogRead(A1)>>1)+32;

   if(period!=periodOld){
     Timer1.setPeriod(period);
     periodOld = period;
   }
}

byte channel=0;

void genAudio(){
  byte i =0;
  for(i=0; i<channels; i++) {
    if (freq[i]!=255){ // mute if freq == 255
      if(cnt[i]>=freq[i]) {
        cnt[i]= 0;
        state[i]= !state[i];
      } else {
        cnt[i]++;
      }
      bitState = bitState | state[i]<<i;
      spkState = spkState ^ state[i];
    }
  }
  
  PORTB = bitState;
  bitState = 0;
  //digitalWrite(audio, spkState);
}

void processMidiMsg(){
   // read the incoming byte:
    incomingByte = Serial.read();

    if (incomingByte== 144){ // note on message starting starting
      action = 1;    
      channel = 1;
    } else if (incomingByte== 128){
      action = 0;
      channel = 1;
    } else if (incomingByte== 145){
      action = 1;    
      channel = 2;
    } else if (incomingByte== 129){
      action = 0;
      channel = 2;
    } else if (incomingByte== 146){
      action = 1;    
      channel = 3;
    } else if (incomingByte== 130){
      action = 0;
      channel = 3;   
    } else if (incomingByte== 147){
      action = 1;    
      channel = 4;
      digitalWrite(led, HIGH);
    } else if (incomingByte== 131){
      action = 0;
      channel = 4;
      digitalWrite(led, LOW);

// if we received a "note off", we wait for which note (databyte)  
    } else if (action == 0){ 
      freq[channel] = 255;
      action=3;
      
// if we received a "note on", we wait for the note (databyte)
    } else if (action == 1){ 
      freq[channel] = incomingByte;
      action = 2;

// ...and then the velocity
    } else if  (action == 2) {
      //playvelocity[channel] = incomingByte;
      if (incomingByte==0){ freq[channel] = 0; }
      action = 3;
    }
   
}
