#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 10, 4, 5, 6, 7);


#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/saw2048_int8.h> // table for Oscils to play
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <tables/triangle512_int8.h> // table for Oscils to play
#include <mozzi_midi.h> // for mtof
#include <LowPassFilter.h>
#include <EventDelay.h>
#include <mozzi_fixmath.h>
#include <Ead.h>


#define CONTROL_RATE 256 // powers of 2 please

Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscA(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscB(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscC(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscD(SAW2048_DATA);

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> filterLFO(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> phaseLFO(COS2048_DATA);

LowPassFilter lpf;
EventDelay eventDelay;
Ead volEnv(CONTROL_RATE);

byte DEBUG = 0;


///// midi stuff //////
byte ntsPld[4] = {0, 0, 0, 0};
byte velPld[4] = {0, 0, 0, 0};
float freqTable[128] = {};
//byte ntsPld[4] = {60+12, 64+12, 55+12, 72+12};
//byte velPld[4] = {255, 255, 255, 255};

byte ntsPldCnt = 0;
int mini = 255;
int maxi = 0;


/// menu stuff
byte debounceBtnA = 0;
byte debounceBtnB = 0;
byte debounceBtnC = 0;

byte menuState = 1;

// HARDWARE I/O
#define encoderPinA 2
#define encoderPinB 3

#define btnA 4
#define btnB 3
#define btnC 2

int encoderPos = 0;
boolean A_set = false;
boolean B_set = false;

  /// synth params ///
float filterLFOSpeed = 50; 
byte filterLFOAmount = 50;
int filterLFOPhase = 514;// 1236
boolean filterLFORetrigger = true;
byte filterLFODiv = 8;
  ///////////////////
float phaseLFOSpeed = 0; 
byte phaseLFOAmount = 0;
int phaseLFOPhase = 514;// 1236
boolean phaseLFORetrigger = true;
byte phaseLFODiv = 0;
  //////////////////
unsigned int volEnvAttack = 755;
unsigned int volEnvDecay = 2255;
byte volEnvValue;
//
//

int modVal[4] = {0, 0, 0, 0};

//
byte phaseLFODivCnt = 0;
byte filterLFODivCnt = 0;


void doEncoderA(){
  A_set = digitalRead(encoderPinA) == HIGH; // and adjust counter + if A leads B
  encoderPos += (A_set != B_set) ? +1 : -1;
}

void doEncoderB(){
  B_set = digitalRead(encoderPinB) == HIGH; // and adjust counter + if B follows A
  encoderPos += (A_set == B_set) ? +1 : -1;
}

byte lastNote = 0;


void loop(){
  audioHook();
}
