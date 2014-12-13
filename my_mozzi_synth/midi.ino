void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if(channel==1){
    
    byte findNote = 0;
    
    if(ntsPldCnt==3){ // overwrite last played note with new one if more than 4 tones played
     lastNote = pitch;
    } else if(ntsPldCnt==4){ 
     findNote = lastNote;
     lastNote = pitch;
     ntsPldCnt--;
    }
    
    
    for(byte i=0; i<4; i++) {
      

      
      if(ntsPld[i] == findNote){ // search for empty note or last one
        ntsPld[i] = pitch;
        velPld[i] = velocity<<1;
      
        if(i==0){ oscA.setFreq(freqTable[pitch]); }
        else if(i==1){ oscB.setFreq(freqTable[pitch]); }
        else if(i==2){ oscC.setFreq(freqTable[pitch]); }
        else if(i==3){ oscD.setFreq(freqTable[pitch]); }
        
        volEnv.start();
        ntsPldCnt++;
        i = 4;
  
  
      }
    }
    

      
      // retrigger LFO's on NoteOn
    //if(ntsPldCnt==1){
      if(filterLFORetrigger==true){ filterLFO.setPhase(filterLFOPhase); }
      if(phaseLFORetrigger==true){ phaseLFO.setPhase(phaseLFOPhase); }
      // reset LFO dividers
       filterLFODivCnt=0;
       phaseLFODivCnt=0;
    //}
      


  } else if(channel==2) {
    filterLFOSpeed = freqTable[pitch]/4;
    filterLFO.setFreq(filterLFOSpeed);
    filterLFOAmount = velocity<<1;
    
  } else if(channel==3) {
    phaseLFOSpeed = freqTable[pitch]/4;
    phaseLFO.setFreq(phaseLFOSpeed);
    phaseLFOAmount = velocity<<1;
    
  } else if(channel==4) {
    phaseLFODiv = pitch;
  } else if(channel==4) {
    filterLFODiv = pitch;
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity){
 
  for(byte h=0; h<4; h++){
    if(ntsPld[h] == pitch){
      velPld[h] = 0;
      ntsPld[h] = 0;
      ntsPldCnt--;
      h = 4;
    }
  }
  
}
