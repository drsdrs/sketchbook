int updateAudio(){
    //Q15n16 vibrato = (Q15n16) intensity * aVibrato.next();
    //vibrato = (vibrato>>6)<<6;
    //holder = 0;
    //int res = (((int)oscA.next()*velPld[0])>>8) | (((oscB.next()*velPld[1])>>8)) | (((oscC.next()*velPld[2])>>8)>>1);
    //res *= ((oscB.next()*velPld[1])>>7);
    //res = res>>8;
    //res += ((oscC.next()*velPld[2])>>8)>>8;
    //res = res >> 14;
    //holder = lpf.next(holder>>3);
    //return ((int)(((oscA.next()*velPld[0])>>8)+((oscB.next()*velPld[1])>>8)+((oscC.next()*velPld[2]))>>8))>>3;
    //return ((int)((oscA.next())+(oscB.next())+(oscC.next()))>>3)-127;
    //return ((int)(oscA.next()+oscB.next()))>>1;;
    //return ((int)(oscA.next()*velPld[0]))>>8;
    
    //int synth = oscA.next()
    //return synth;//(synth*volEnvValue)>>8;
    //byte a = (((oscA.next()&velPld[0])+(oscB.next()&velPld[1])+(oscC.next()&velPld[2])+(oscD.next()&velPld[3]))>>2);
    
    //a = a^(t>>modVal[0])^(t<<modVal[1]);
    //t = a*(t>>modVal[2])-(t>>modVal[3]);
    //a = (((a&255))*volEnvValue)>>6;
    
    return (((oscA.next()&velPld[0])+(oscB.next()&velPld[1])+(oscC.next()&velPld[2])+(oscD.next()&velPld[3]))>>2);
      
}
