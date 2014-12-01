#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

byte led = 13;

byte channel=0;
byte oct;

int t=0;


long cycles[128] =
{
  61156,57723,54483,51425,48539,45815,43243,40816,38525,36363,34322,32396,30578,28861,27241,25712,24269,22907,21621,20408,19262,18181,17161,16198,15289,14430,13620,12856,12134,11453,10810,10204,9631,9090,8580,8099,7644,7215,6810,6428,6067,5726,5405,5102,4815,4545,4290,4049,3822,3607,3405,3214,3033,2863,2702,2551,2407,2272,2145,2024,1911,1803,1702,1607,1516,1431,1351,1275,1203,1136,1072,1012,955,901,851,803,758,715,675,637,601,568,536,506,477,450,425,401,379,357,337,318,300,284,268,253,238,225,212,200,189,178,168,159,150,142,134,126,119,112,106,100,94,89,84,79,75,71,67,63,59,56,53,50,47,44,42,39
};


byte ntsPld[64];
byte ntsPldCnt = 0;
byte n, nn;

int ntsFreqCnt[64];
int pinState[64];

void handleNoteOff(byte channel, byte note, byte velocity)
{
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
  ntsPld[ ntsPldCnt ] = note;
  ntsFreqCnt [ ntsPldCnt ] = 0;
  pinState [ ntsPldCnt ] = 0;
  ntsPldCnt++;
}



void setup() {

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin();

  pinMode(led, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  DDRB = DDRB | 0b11111111;

  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  // set compare match register to desired timer count:
  OCR1A = 32;
  OCR1B = 1;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS11);
  //TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  //TIMSK1 |= (1 << OCIE1B);
//
//  TCCR2A = 0;     // set entire TCCR2A register to 0
//  TCCR2B = 0;     // same for TCCR2B
//  // set compare match register to desired timer count:
//  OCR2A = 128;
//  // turn on CTC mode:
//  TCCR2A |= (1 << WGM21);
//  // Set CS20 and CS22 bits for 1024 prescaler:
//  TCCR2B |= (1 << CS20);
//  //TCCR2B |= (1 << CS21);
//  //TCCR2B |= (1 << CS22);
//  // enable timer compare interrupt:
//  TIMSK2 |= (1 << OCIE2A);
//  //TIMSK2 |= (1 << OCIE2B);
//  
//  //To use Fast PWM on Timer 0
//  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
//  TCCR0B = 0;
//  // prescaler
//  TCCR0B |= (1 << CS00);
//  //TCCR0B |= (1 << CS01);
//  //TCCR0B |= (1 << CS02);
  
}

long reg = 0x55aa55aaL;
int b31, b29, b25, b24, lobit, val1;

void loop() {
  MIDI.read();
  //oct = ((analogRead(A0)>>6));
  //int val1 = ((analogRead(A1)>>2));
  //OCR2A = ((analogRead(A0)>>2));
  //OCR2B = ((analogRead(A1)));
  //OCR0B = (t*t/2)&255;
}


ISR(TIMER1_COMPA_vect) {
  byte state = 0;
  for (n=0; n<ntsPldCnt; n++) {
    if (ntsFreqCnt[n]> cycles[ntsPld[n]]) {
      pinState[n]=!pinState[n];
      ntsFreqCnt[n] = 0;
    } else {
      ntsFreqCnt[n]++;
    }
    state ^= pinState[n];
  }
  PORTB ^= (state<<0) ;
}

//ISR(TIMER1_COMPB_vect) {
//  PORTB ^= (1<<0);
//}
//
//ISR(TIMER2_COMPA_vect) {
//  OCR0A = (t^t%val1)&255;
//  t++;
//}





