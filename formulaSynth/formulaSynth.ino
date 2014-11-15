#include "TimerOne.h"  

unsigned long previousMillis = 0;        // will store last time LED was updated
byte led = 13;
byte ledState = 0;
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

byte channel=0;

byte selFunct = 0;
byte val;
byte mod=1;


long t;

void setup() {
  
  Serial.begin(31250);
    
  pinMode(led, OUTPUT);

  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS00);
  //TCCR2B = (TCCR2B & 0xf8) | 1;

  analogWrite(3, 1);

  Timer1.initialize(period);
  Timer1.attachInterrupt(genAudio);

}

// the loop function runs over and over again forever
void loop() {
  
   if (Serial.available() > 0) {
      processMidiMsg();
   }
   period = 32;//(analogRead(A1)>>1)+32;
   selFunct = (analogRead(A1)>>5);
   mod = (analogRead(A0)>>7)+1;

   if(period!=periodOld){
     Timer1.setPeriod(period);
     periodOld = period;
   }
   // testTone
   digitalWrite(led, ledState);
   ledState != ledState;
}

// t*((t>>12|t>>8)&63&t>>4)   // by viznut
 // ((-t&4095)*(255&t*(t&t>>13))>>12)+(127&t*(234&t>>8&t>>3)>>(3&t>>14)) // by tejeez
 // t*(t>>11&t>>8&123&t>>3)    // by tejeez
 // t*((t>>9|t>>13)&25&t>>6)   // by visy
 // (t*(t>>5|t>>8))>>(t>>16)   // by tejeez
 // ((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6) // by xpansive
 // ((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t)) // by skurk (raer's version)
 // (t>>7|t|t>>6)*10+4*(t&t>>13|t>>6) // by viznut, xpansive, varjohukka
 // t*5&(t>>7)|t*3&(t*4>>10) // by miiro
 // (t|(t>>9|t>>7))*t&(t>>11|t>>9) // by red
 // v=(v>>1)+(v>>4)+t*(((t>>16)|(t>>6))&(69&(t>>9))) // by pyryp
 // (t>>6|t|t>>(t>>16))*10+((t>>11)&7)  //by viznut
 // (t*(4|7&t>>13)>>((~t>>11)&1)&128) + ((t)*(t>>11&t>>13)*((~t>>9)&3)&127) // by stimmer
 
 byte v;

void genAudio(){
  t++;
  if(selFunct==0){ val = t&t>>mod; }
  else if(selFunct==1){ val = t&t<<mod; }
  else if(selFunct==2){ val = t^t>>mod; }
  else if(selFunct==3){ val = t^t<<mod; }
  else if(selFunct==4){ val = t*t>>mod; }
  else if(selFunct==5){ val = ((-t&4095)*(255&t*(t&t>>13))>>12)+(127&t*(234&t>>8&t>>3)>>(3&t>>14)); }
  else if(selFunct==6){ val = t*(t>>11&t>>8&123&t>>3); }
  else if(selFunct==7){ val = t*((t>>9|t>>13)&25&t>>6); }
  else if(selFunct==8){ val = ((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6); }
  else if(selFunct==9){ val = ((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t)); }
  else if(selFunct==10){ val = (t>>7|t|t>>6)*10+4*(t&t>>13|t>>6); }
  else if(selFunct==11){ val = (t|(t>>9|t>>7))*t&(t>>11|t>>9); }
  else if(selFunct==12){ val = v=(v>>1)+(v>>4)+t*(((t>>16)|(t>>6))&(69&(t>>9))); }
  else if(selFunct==13){ val = (t>>6|t|t>>(t>>16))*10+((t>>11)&7); }
  else if(selFunct==14){ val = (t*(4|7&t>>13)>>((~t>>11)&1)&128) + ((t)*(t>>11&t>>13)*((~t>>9)&3)&127); }

  else { val = (255557*mod)/(t%(255<<4)); }
  
  OCR2B = val;
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

//// one-line algorithmic music
//// see viznut's blog http://countercomplex.blogspot.com/2011/10/algorithmic-symphonies-from-one-line-of.html
//// and http://www.youtube.com/watch?v=GtQdIYUtAHg&feature=related
//
//// ported to arduino by stimmer
//// Audio out on pin 10
//
//void setup() {
// TCCR1B = (TCCR1B & 0xf8) | 1;
// analogWrite(10,1);
//}
//
//void putb(byte b)
//{
// static long m;
// long t;
// while((t=micros())-m < 125);
// m=t;
// 
// OCR1B=b;
//}
//
//void loop() {
// long v;
// for(long t=0;;t++)putb(
// 
// // uncomment ONE line below
// // or try making one of your own...
// 
// t*((t>>12|t>>8)&63&t>>4)   // by viznut
 // ((-t&4095)*(255&t*(t&t>>13))>>12)+(127&t*(234&t>>8&t>>3)>>(3&t>>14)) // by tejeez
 // t*(t>>11&t>>8&123&t>>3)    // by tejeez
 // t*((t>>9|t>>13)&25&t>>6)   // by visy
 // (t*(t>>5|t>>8))>>(t>>16)   // by tejeez
 // ((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6) // by xpansive
 // ((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t)) // by skurk (raer's version)
 // (t>>7|t|t>>6)*10+4*(t&t>>13|t>>6) // by viznut, xpansive, varjohukka
 // t*5&(t>>7)|t*3&(t*4>>10) // by miiro
 // (t|(t>>9|t>>7))*t&(t>>11|t>>9) // by red
 // v=(v>>1)+(v>>4)+t*(((t>>16)|(t>>6))&(69&(t>>9))) // by pyryp
 // (t>>6|t|t>>(t>>16))*10+((t>>11)&7)  //by viznut
 // (t*(4|7&t>>13)>>((~t>>11)&1)&128) + ((t)*(t>>11&t>>13)*((~t>>9)&3)&127) // by stimmer
 
 //);
 
//}

