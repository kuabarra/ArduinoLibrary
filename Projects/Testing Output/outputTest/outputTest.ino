void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(2,HIGH);
  unsigned int uiLightRead = analogRead(0);
  if (digitalRead(2))
     analogWrite(3,(uiLightRead / 4));
   else
     analogWrite(3,(0));

  Serial.println(uiLightRead);
  delay (100);
}

