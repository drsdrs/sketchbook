byte ledWrite = 32;
byte ledWriteCnt = 0;

byte curPos = 0;

void updateControl(){
  
  if(ledWriteCnt>=ledWrite){
    ledWriteCnt = 0;
    lcd.setCursor(curPos-1, 1);
    lcd.print(" ");
    lcd.setCursor(curPos, 1);
    lcd.print("-*");
    curPos++;
    if(curPos>15){curPos=0;}
  } else {
    ledWriteCnt++;
  }
  
  
  // Switch Menu
  if(digitalRead(btnB)==LOW){
   volEnv.start();
    if(debounceBtnB==0){
      menuState++;
      if(menuState>=4){menuState=0;}
      debounceBtnB=1;
    }
  } else { debounceBtnB=0; };
  
  if(digitalRead(btnA)==LOW){
    if(debounceBtnA==0){
      modVal[menuState]++;
      if(modVal[menuState]>8){modVal[menuState]=0;}
      debounceBtnA=1;
    }
  } else { debounceBtnA=0; };
  
   if(digitalRead(btnC)==LOW){
    if(debounceBtnC==0){
      modVal[menuState]--;
      if(modVal[menuState]<0){modVal[menuState]=8;}
      debounceBtnC=1;
 
    }
  } else { debounceBtnC=0; }; 
  

  if(encoderPos>8){
    encoderPos=8;
  } else if (encoderPos<0){
    encoderPos=0;
  }


  if(DEBUG==0){
    MIDI.read();
  } else {
   if(eventDelay.ready()){

      byte a =  volEnvValue;//(((byte)(oscA.next()&velPld[0])+(oscB.next()&velPld[1])+(oscC.next()&velPld[2])+(oscD.next()&velPld[3]))>>2);
      if (a>maxi){ maxi = a; } else if (a<mini) {mini = a;}
      Serial.print("min: ");
      Serial.print(mini);
      Serial.print("___max: ");
      Serial.print(maxi);
      Serial.print(" ____ ");
      Serial.println(volEnvValue);
//      Serial.print(menuState, DEC);
//      Serial.print(" - ");
//      Serial.print(volEnvAttack, DEC);
//      Serial.print(" - ");
//      Serial.println(volEnvDecay, DEC);
      eventDelay.start();
    } 
  }
  
  int pitchMod = phaseLFO.next();
  
  if(phaseLFODivCnt>=phaseLFODiv){
    phaseLFODivCnt=0;
    pitchMod = (pitchMod*phaseLFOAmount)>>8;
    oscA.setFreq((freqTable[ntsPld[0]]+pitchMod));
    oscB.setFreq((freqTable[ntsPld[1]]+pitchMod));
    oscC.setFreq((freqTable[ntsPld[2]]+pitchMod));
    oscD.setFreq((freqTable[ntsPld[3]]+pitchMod));
  }
  
  int filterMod = filterLFO.next();
  
  if(filterLFODivCnt>=filterLFODiv){
    lpf.setCutoffFreq( 128+((filterMod*filterLFOAmount)>>9) );
     //byte cut = mozziAnalogRead(A0)>>2;

    //lpf.setCutoffFreq( cut );
  }
  
  filterLFODivCnt++;
  phaseLFODivCnt++;
   
  //byte res = mozziAnalogRead(A1)>>2;
   //lpf.setResonance(64);

   
   //oscB.setPhaseFractional((oscB.getPhaseFractional((int)freqTable[ntsPld[1]]+deltime)));
   //oscC.setPhaseFractional((oscC.getPhaseFractional((int)freqTable[ntsPld[2]]+deltime)));
  volEnvValue = (byte)volEnv.next()>>1;
  
}


void printFloat(float value, int places) {
  // this is used to cast digits 
  int digit;
  float tens = 0.1;
  int tenscount = 0;
  int i;
  float tempfloat = value;

    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
  // if this rounding step isn't here, the value  54.321 prints as 54.3209

  // calculate rounding term d:   0.5/pow(10,places)  
  float d = 0.5;
  if (value < 0)
    d *= -1.0;
  // divide by ten for each decimal place
  for (i = 0; i < places; i++)
    d/= 10.0;    
  // this small addition, combined with truncation will round our values properly 
  tempfloat +=  d;

  // first get value tens to be the large power of ten less than value
  // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

  if (value < 0)
    tempfloat *= -1.0;
  while ((tens * 10.0) <= tempfloat) {
    tens *= 10.0;
    tenscount += 1;
  }
 // write out the negative if needed
  if (value < 0)
    Serial.print('-');

  if (tenscount == 0)
    Serial.print(0, DEC);

  for (i=0; i< tenscount; i++) {
    digit = (int) (tempfloat/tens);
    Serial.print(digit, DEC);
    tempfloat = tempfloat - ((float)digit * tens);
    tens /= 10.0;
  }

  // if no places after decimal, stop now and return
  if (places <= 0)
    return;

  // otherwise, write the point and continue on
  Serial.print('.');  

  // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
  for (i = 0; i < places; i++) {
    tempfloat *= 10.0; 
    digit = (int) tempfloat;
    Serial.print(digit,DEC);  
    // once written, subtract off that digit
    tempfloat = tempfloat - (float) digit; 
  }
}
