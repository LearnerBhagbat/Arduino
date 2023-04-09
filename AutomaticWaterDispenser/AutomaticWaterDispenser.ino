void setup() {
  // put your setup code here, to run once:
pinMode(4,INPUT);
pinMode(0,OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(4) == 1){
  Serial.println(digitalRead(4));
  digitalWrite(0,HIGH);
}

else{
  digitalWrite(0,LOW);
}

}
