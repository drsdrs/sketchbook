unsigned long previousMillis = 0;        // will store last time LED was updated
byte led = 13;


void setup() {
  pinMode(led, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
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
