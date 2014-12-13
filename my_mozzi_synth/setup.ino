void setup(){
  
  lcd.begin(16, 2);
  
  for (byte i=0; i<128; i++){
    freqTable[i] = mtof(i);
    if(i==0){ freqTable[i] = 0; }
  }
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT);
  
 
  digitalWrite(encoderPinA, HIGH); 
  digitalWrite(encoderPinB, HIGH);  
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);


  pinMode(btn, OUTPUT);
  digitalWrite(btn, HIGH);  

  //
  if(DEBUG==1){
    Serial.begin(9600);
  } else {
    MIDI.begin(MIDI_CHANNEL_OMNI);
  }
  
  eventDelay.set(425);


  startMozzi(CONTROL_RATE);
  
  lpf.setResonance(32);
  lpf.setCutoffFreq(127);
  
  filterLFO.setFreq(phaseLFOSpeed);
  
  phaseLFO.setFreq(phaseLFOSpeed);
  filterLFO.setFreq(filterLFOSpeed);
  
  volEnv.setDecay(volEnvDecay);
  volEnv.setAttack(volEnvAttack);
  //volEnv.start();
    

  //oscA.setFreq(freqTable[ntsPld[0]]);
  //oscB.setFreq(freqTable[ntsPld[1]]);
  //oscC.setFreq(freqTable[ntsPld[2]]);
  
}
