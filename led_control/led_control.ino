int i = 0;
int range = 0;

void setup() {
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  analogWrite(4, 70);
}

void loop() {
  if (Serial.available()){
    i = Serial.read();
    if (i != 10){
    range = map(i, 48, 57, 0, 255);
    Serial.println(range);
    analogWrite(4, range);
    }
  }
}
