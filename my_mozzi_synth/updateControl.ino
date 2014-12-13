byte ledWrite = 32;
byte ledWriteCnt = 0;


void updateControl(){
   
  
   if(ledWriteCnt>=ledWrite){
     
    modVal[0] = mozziAnalogRead(A0)>>2;
    modVal[1] = mozziAnalogRead(A1)>>7;
     
    lcd.setCursor(0, 0);
    lcd.print(menuState);
    
    lcd.setCursor(2, 0);
    lcd.print(modVal[0]);
    lcd.setCursor(6, 0);
    lcd.print(modVal[1]);   
 
    lcd.setCursor(0, 1);
    lcd.print((int)filterLFOSpeed);
    lcd.setCursor(8, 1);
    lcd.print((int)filterLFOAmount);
    
    ledWriteCnt = 0;
  } else {
    ledWriteCnt++;
  }
  
  // Switch Menu
  if(digitalRead(btn)==LOW){
    if(debounceBtn==0){
      menuState++;
      encoderPos=0;
      debounceBtn=1;
      
      volEnv.start();

    }
  } else { debounceBtn=0; };
  

  if(menuState==0) {   // MENU 0 - filterLFOSpeed
    filterLFOSpeed += encoderPos;
    encoderPos = 0;
    if(filterLFOSpeed<0){ filterLFOSpeed=0; }
    filterLFO.setFreq(filterLFOSpeed+encoderPos);
  
  } else if(menuState==1) { // MENU 1 - filterLFOAmount
    filterLFOAmount += encoderPos;
    if(filterLFOAmount>150){ filterLFOAmount=150; }
    encoderPos=0;
    
  } else if(menuState==2) { // MENU 2 - filterLFOPhase
    filterLFOPhase += encoderPos;
    if(filterLFOPhase>=2048){ filterLFOPhase=2048; }
    if(filterLFOPhase<=0){ filterLFOPhase=0; }
    encoderPos=0;
    
 } else if(menuState==3){ // MENU 3 - phaseLFOSpeed
    phaseLFOSpeed += encoderPos/4;
    if(phaseLFOSpeed<0){ phaseLFOSpeed=0; }
    phaseLFO.setFreq(phaseLFOSpeed);
    encoderPos=0;
    
 } else if(menuState==4){ // MENU 4 - phaseLFOAmount
    phaseLFOAmount += encoderPos;
    if(phaseLFOAmount<=0){ phaseLFOAmount=0; }
    encoderPos=0;
    
 } else if(menuState==5){ // MENU 5 - phaseLFOPhase
    phaseLFOPhase += encoderPos;
    if(phaseLFOPhase>=2048){ phaseLFOPhase=2048; }
    if(phaseLFOPhase<=0){ phaseLFOPhase=0; }
    encoderPos=0;
    
  } else if(menuState==6){ // MENU 6 - volEnvAttack
    volEnvAttack += encoderPos*10;
    //if(volEnvAttack>=2048){ volEnvAttack=2048; }
    if(volEnvAttack<=0){ volEnvAttack=0; }
    volEnv.setAttack(volEnvAttack);
    encoderPos=0;   
  
   } else if(menuState==7){ // MENU 7 - volEnvDecay
    volEnvDecay += encoderPos*10;
    //if(volEnvDecay>=2048){ volEnvDecay=2048; }
    if(volEnvDecay<=0){ volEnvDecay=0; }
    volEnv.setDecay(volEnvDecay);
    encoderPos=0;
   } else if(menuState==8){ // MENU 8 - pitchLFODiv
    phaseLFODiv += encoderPos;
    if(phaseLFODiv>=64){ phaseLFODiv=64; }
    if(phaseLFODiv<=0){ phaseLFODiv=0; }
    encoderPos=0;
   } else if(menuState==9){ // MENU 9 - filterLFODiv
    filterLFODiv += encoderPos;
    if(filterLFODiv>=64){ filterLFODiv=64; }
    if(filterLFODiv<=0){ filterLFODiv=0; }
    encoderPos=0; 
   } else { menuState=0; }

  if(DEBUG==0){
    MIDI.read();
  } else {
   if(eventDelay.ready()){

//      int a = 127-(((int)filterLFO.next()*val2)>>8);
//      if (a>maxi){ maxi = a; } else if (a<mini) {mini = a;}
//      Serial.print("min: ");
//      Serial.print(mini);
//      Serial.print("___max: ");
//      Serial.print(maxi);
//      Serial.print(" ____ ");
      Serial.print(menuState, DEC);
      Serial.print(" - ");
      Serial.print(volEnvAttack, DEC);
      Serial.print(" - ");
      Serial.println(volEnvDecay, DEC);
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
  }
  
  int filterMod = filterLFO.next();
  
  if(filterLFODivCnt>=filterLFODiv){
    lpf.setCutoffFreq( 127-((filterMod*filterLFOAmount)>>8) );
  }
  
  filterLFODivCnt++;
  phaseLFODivCnt++;


   
   //oscB.setPhaseFractional((oscB.getPhaseFractional((int)freqTable[ntsPld[1]]+deltime)));
   //oscC.setPhaseFractional((oscC.getPhaseFractional((int)freqTable[ntsPld[2]]+deltime)));
  volEnvValue = volEnv.next()>>1;
  
}
