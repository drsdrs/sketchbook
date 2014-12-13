int updateAudio(){

    byte a = (oscA.next()-modVal[0]);
    byte b = (oscB.next()-modVal[0]);
    byte c = (oscC.next()-modVal[0]);
    byte d = (oscD.next()-modVal[0]);
    
    //tt |= (tt>>modVal[0]);
    //tt |= (tt<<modVal[1]);
    
    //a ^= a^(a<<modVal[1]);//|(a<<modVal[0]);
    //b ^= b^(b<<modVal[1]);//|(b<<modVal[0]);
    //c ^= c^(c<<modVal[1]);//|(c<<modVal[0]);
    //d ^= d^(d<<modVal[1]);//|(d<<modVal[0]);

    
    //t = a*(t>>modVal[2])-(t>>modVal[3]);
    //a = (((a&255))*volEnvValue)>>6;
    
    a &= velPld[0];
    b &= velPld[1];
    c &= velPld[2];
    d &= velPld[3];
    
    a = ((a+b+c+d)>>2);
    
    // L+R bitshift wave cleanout 
    //a = a>>modVal[1]<<modVal[1];

    return //lpf.next(
     (  a*volEnvValue)>>6;
    //);
      
}
