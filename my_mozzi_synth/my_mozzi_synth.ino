/*  Example playing a sinewave with vibrato,
    using Mozzi sonification library.
  
    Demonstrates simple FM using phase modulation.
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.0/3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa.
*/

//#include <ADC.h>  // Teensy 3.0/3.1 uncomment this line and install http://github.com/pedvide/ADC

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/saw2048_int8.h> // table for Oscils to play
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <mozzi_midi.h> // for mtof
#include <LowPassFilter.h>

#include <mozzi_fixmath.h>

#define CONTROL_RATE 256 // powers of 2 please

Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscA(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscB(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscC(SAW2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aVibrato(COS2048_DATA);

LowPassFilter lpf;

float freqTable[128] = {};

byte cnt=0;
int mod=0;

byte ntsPld[3] = {0, 0, 0};
byte velPld[3] = {0, 0, 0};
byte ntsPldCnt = 0;

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  
  ntsPld[ntsPldCnt] = pitch;
  velPld[ntsPldCnt] = velocity;
  
  if(ntsPldCnt==0){ oscA.setFreq(freqTable[ntsPld[0]]); }
  else if(ntsPldCnt==1){ oscB.setFreq(freqTable[ntsPld[1]]); }
  else if(ntsPldCnt==2){ oscC.setFreq(freqTable[ntsPld[2]]); }
 
  if(ntsPldCnt>2) {
    ntsPldCnt = 2;
  } else {
    ntsPld[ntsPldCnt] = pitch;
    velPld[ntsPldCnt] = velocity;
    ntsPldCnt++;
  }
  
}

void handleNoteOff(byte channel, byte pitch, byte velocity){
  
  if(ntsPldCnt>0){
    ntsPldCnt--;
  } else {
    ntsPldCnt=0;
  }
  
  for(byte h=0; h<3; h++){
    if(ntsPld[h] == pitch){
      velPld[h] = 0;
      ntsPld[h] = 0;
    }
  }
  
  
}

void setup(){
  for (byte i=0; i<128; i++){
    freqTable[i] = mtof(i);
  }
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //Serial.begin(9600);


  startMozzi(CONTROL_RATE);
  
  lpf.setResonance(150);
  
  oscA.setFreq(freqTable[ntsPld[0]]);
  oscB.setFreq(freqTable[ntsPld[1]]);
  oscC.setFreq(freqTable[ntsPld[2]]);
  
}



void updateControl(){
  MIDI.read();

  //int val1 = mozziAnalogRead(0);
  //int val2 = mozziAnalogRead(1);
  
  //aVibrato.setFreq(val1);
  //mod = aVibrato.next();
  //mod = (mod*val2)>>8;

  //lpf.setCutoffFreq(127-mod);
  
  //float a = oscA.next();
  //Serial.println(a);

}

int updateAudio(){
    //Q15n16 vibrato = (Q15n16) intensity * aVibrato.next();
    //vibrato = (vibrato>>6)<<6;
    //holder = 0;
    int res = (((int)oscA.next()*velPld[0])>>8) | (((oscB.next()*velPld[1])>>8)) | (((oscC.next()*velPld[2])>>8)>>1);
    //res *= ((oscB.next()*velPld[1])>>7);
    //res = res>>8;
    //res += ((oscC.next()*velPld[2])>>8)>>8;
    //res = res >> 14;
    //holder = lpf.next(holder>>3);
    return (res);
}

void loop(){
  audioHook();
}
