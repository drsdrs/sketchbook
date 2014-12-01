/*  Example of pulse width modulation,
    using Mozzi sonification library.
     
    Based Miller Puckette's j03.pulse.width.mod example 
    in the Pure Data documentation, subtracting 2 sawtooth 
    waves with slightly different tunings to produce a 
    varying phase difference.
  
    Demonstrates Phasor().
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.0/3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa.
*/
#define INPUT_PIN 0 // analog control input
#define LED 13


#include <MozziGuts.h>
#include <Phasor.h>
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

#define CONTROL_RATE 64 // powers of 2 please

Phasor <AUDIO_RATE> aPhasor1;
Phasor <AUDIO_RATE> aPhasor2;
Phasor <AUDIO_RATE> aPhasor3;
Phasor <AUDIO_RATE> aPhasor4;
Phasor <AUDIO_RATE> aPhasor5;
Phasor <AUDIO_RATE> aPhasor6;
Phasor <AUDIO_RATE> aPhasor7;
Phasor <AUDIO_RATE> aPhasor8;


byte ntsPld[8];
byte ntsPldCnt = 0;
byte n, nn;

void handleNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(LED, LOW);

  byte found = 0;
  for (nn=0; nn<ntsPldCnt; nn++) {
    if (ntsPld[nn]==note) {
      found = 1;
    }
   
    if (found==1) {
      if (nn==ntsPldCnt-1){
        ntsPld[nn] = 0;
      } else {
        ntsPld[nn] = ntsPld[nn+1];
      }
    
    }
  }
  ntsPldCnt--;
}

void handleNoteOn(byte channel, byte note, byte velocity)
{
  int freq = Q16n16(note);
  aPhasor1.setFreq(freq);
  digitalWrite(LED, HIGH);
  ntsPld[ ntsPldCnt ] = note;
  ntsPldCnt++;
}

void setup(){
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin();
  
  pinMode(LED, OUTPUT);
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
}


void updateControl(){
  MIDI.read();
//  int aVal = mozziAnalogRead(INPUT_PIN);
 // int freq = aVal;
//
  //aPhasor1.setFreq(440);
//  aPhasor2.setFreq(freq+1);  
//  aPhasor3.setFreq(freq-1);  
//  aPhasor4.setFreq(freq<<1); 
//  aPhasor5.setFreq(freq>>1); 
//  aPhasor6.setFreq(freq<<2); 
//  aPhasor7.setFreq(freq>>2); 
}


int updateAudio(){
  unsigned long t = (aPhasor1.next()>>8)&(0xffff);
  byte val = (t&t>>8);
  
  return val;///ntsPldCnt;
}


void loop(){
  audioHook(); // required here
}







