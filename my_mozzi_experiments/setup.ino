void setup(){
  
  lcd.begin(16, 2);
  lcd.print("SaylerSynthv0.6");
  lcd.setCursor(0, 1);
  lcd.print("__-^-_-|-_-^-__");
  
  for (byte i=0; i<128; i++){
    freqTable[i] = mtof(i);
    if(i==0){ freqTable[i] = 0; }
  }
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  //pinMode(encoderPinA, INPUT); 
  //pinMode(encoderPinB, INPUT);
  
 
  //digitalWrite(encoderPinA, HIGH); 
  //digitalWrite(encoderPinB, HIGH);  
  //attachInterrupt(0, doEncoderA, CHANGE);
  //attachInterrupt(1, doEncoderB, CHANGE);


  pinMode(btnA, OUTPUT);
  pinMode(btnC, OUTPUT);
  pinMode(btnB, OUTPUT);
  digitalWrite(btnA, HIGH);  
  digitalWrite(btnC, HIGH);  
  digitalWrite(btnB, HIGH);  

  //
  if(DEBUG==1){
    Serial.begin(9600);
  } else {
    MIDI.begin(MIDI_CHANNEL_OMNI);
  }
  
  eventDelay.set(25);


  startMozzi(CONTROL_RATE);
  
  lpf.setResonance(63);
  lpf.setCutoffFreq(127);
  
  filterLFO.setFreq(phaseLFOSpeed);
  
  phaseLFO.setFreq(phaseLFOSpeed);
  filterLFO.setFreq(filterLFOSpeed);
  
  volEnv.setDecay(volEnvDecay);
  volEnv.setAttack(volEnvAttack);
  //volEnv.start();
    
  
  oscA.setFreq(freqTable[ntsPld[0]]);
  oscB.setFreq(freqTable[ntsPld[1]]);
  oscC.setFreq(freqTable[ntsPld[2]]);
  oscD.setFreq(freqTable[ntsPld[3]]);
  
}
