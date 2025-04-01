byte byteRead;
bool outputState = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);

}

void loop() {
  //First,check com for input
  if (Serial.available())
  {
    byteRead = Serial.read();
    Serial.println(byteRead);
  }

  if (byteRead == 49)
    outputState = 1;
  else if (byteRead == 48)
    outputState = 0;
  
  unsigned int uiLightRead = analogRead(0);
  if (outputState)//digitalRead(2))
    analogWrite(3,(uiLightRead / 4));
   else
     analogWrite(3,(0));
  Serial.println(uiLightRead);
  delay (1000);
}
