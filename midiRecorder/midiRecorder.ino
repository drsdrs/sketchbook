unsigned long previousMillis = 0;   
long interval = 32;
byte metronomTick = 4;
byte metronomCnt = 0;
byte tick = 0;

byte incomingByte ;
byte action = 0;
byte note = 0;
byte velocity = 0;
byte led = 13;
byte audio = 8;

byte beatLength = 4;

byte btnA = 4;
byte btnB = 3;
byte btnC = 2;

byte btnAUp = 0;
byte btnBUp = 0;
byte btnCUp = 0;

byte rec = 0;

byte record[32][2];

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(31250);
  pinMode(led, OUTPUT);
  pinMode(audio, OUTPUT);
  pinMode(btnA, INPUT);
  pinMode(btnB, INPUT);
  pinMode(btnC, INPUT);
  digitalWrite(btnA, HIGH);
}

void loop() {

  if (Serial.available() > 0) {
    processMidiMsg();
 }


  if (digitalRead(btnA)==LOW) {
    if(btnAUp==0) {
      rec = !rec;
      digitalWrite(led, rec);
      btnAUp = 1;
    }
  } else {
    btnAUp = 0;
  }
  

  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;
    interval = (analogRead(0)>>2)+24;
    beatLength = (analogRead(1)>>6)+1;
    
    if (record[tick][0]!=0) {
      noteOn(record[tick][0], record[tick][1], record[tick][2]);
      digitalWrite(led, HIGH);
    }else {
      digitalWrite(led, LOW);
    }
  
    if (tick>32){ tick = 0;}


    if (metronomCnt>metronomTick){
      noteOn(153, 32, 50);      
      //noteOn(137, 32, 0);
      metronomCnt = 0;
    }
    metronomCnt++;
    tick++;

  }
  
}

void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd); Serial.write(pitch); Serial.write(velocity);
}

void processMidiMsg(){
   // read the incoming byte:
   incomingByte = Serial.read();

   // wait for as status-byte, channel 1, note on or off
   if (incomingByte== 144){ // note on message starting starting
     action = 1;
     
   }else if (incomingByte== 128){ // note off message starting
     action = 0;
     
   }else if ( (action==0)&&(note==0) ){ // if we received a "note off", we wait for which note (databyte)
     note = incomingByte;
     record[tick][0] = 144;
     record[tick][1] = note;
     record[tick][2] = 0;
     action = 2;
     
   }else if ( (action==1)&&(note==0) ){ // if we received a "note on", we wait for the note (databyte)
     note = incomingByte;

   }else if ( (action==1)&&(note!=0) ){ // ...and then the velocity
     velocity = incomingByte;
     record[tick][0] = 144;
     record[tick][1] = note;
     record[tick][2] = velocity;
     action = 0;
   }
   
}
