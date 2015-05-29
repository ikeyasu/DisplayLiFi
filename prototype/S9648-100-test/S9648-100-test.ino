int gPreviousTime = 0;
int gPreviousSignal = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  int val=analogRead(0);
  //val = (val*5000)/1024;
  //Serial.println(val, DEC);
  if (val > 3 && gPreviousSignal <= 3) {
    int diff = millis() - gPreviousTime;
    Serial.print(diff > 200 ? "HIGH: " : "LOW: ");
    Serial.println(diff, DEC);
    gPreviousTime = millis();
  }
  gPreviousSignal = val;
  //delay(1);
  val = 0;
}
