byte led = 13;
byte cnt;

byte eventSize = 6;
byte events [6][4] = {
  {0,1,1,1},
  {64,1,1,1},
  {64,1,1,1},
  {128,0,1,1},
  {192,0,1,1},
  {192,0,16,61}
};

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  delay(400);

  cnt = cnt%eventSize;
  
  Serial.print(sizeof(events)/4);
  Serial.print(" : ");
  Serial.print(cnt);
  Serial.print(" : ");
  Serial.print(events[cnt][0]);
  Serial.print(" - ");
  Serial.print(events[cnt][1]);
  Serial.print(" - ");
  Serial.print(events[cnt][2]);
  Serial.print(" - ");
  Serial.println(events[cnt][3]);
  
  cnt++;
  if(cnt==eventSize-1){ eventSize++; };

}

void addSort(byte tick, byte noteOn, byte note, byte vel){
  byte newEvents [eventSize+1][4];
  byte j = 0;
  for(byte i=0; i<eventSize; i++){
    if(events[i][0]>tick){
      newEvents[i];
      newEvents[i][0] = 12;
      newEvents[i][1] = 12;
      newEvents[i][2] = 12;
      newEvents[i][3] = 12;
    } else {
      newEvent[i] = events[j];
      j++;
    }
    events = newEvents;
    eventSize++;
  }
};
