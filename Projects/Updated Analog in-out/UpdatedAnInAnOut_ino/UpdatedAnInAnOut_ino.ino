/*** Timing Definitions ******/
#define TIME_1MS_1S  1000
#define TIME_1S_5S   5


/***** Global Variables ***********/
unsigned int g_uiGenericSecondTimer = 0;    //this will keep track of the number of seconds the program has run, until an overflow occurs....
unsigned int g_uiSendMsgTimer = 0;          //when this timer expires, send a message then reset timer

//*** Inputs to be read in CheckInputs() ***
unsigned int g_uiAnInLightSnsr = 0;

/****** Start Code ****************/
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  UpdateTimers();
  CheckInputs();
  SetOutputs();
   
  if (!g_uiSendMsgTimer)
  { 
    Serial.println(g_uiGenericSecondTimer);
    g_uiSendMsgTimer = TIME_1S_5S;
  }
}

void UpdateTimers()
{
  static unsigned long r_ulPrevMillis = 0;
  unsigned long ulCurMillis = millis();
  unsigned long ulMillisUpdate = ulCurMillis - r_ulPrevMillis;
  static unsigned int r_uiMilliCounter = 0;
  
  r_ulPrevMillis = ulCurMillis;
  
  r_uiMilliCounter += ulMillisUpdate;
  
  while (r_uiMilliCounter >= TIME_1MS_1S)    //Update second based timers here
  {
    r_uiMilliCounter -= TIME_1MS_1S;
    
    g_uiGenericSecondTimer++;
    
    if (g_uiSendMsgTimer)
      g_uiSendMsgTimer--;
  }
}

void CheckInputs()
{
    g_uiAnInLightSnsr = analogRead(0);
}

unsigned int CalculateAnalogOut(unsigned int uiInput)
{
    return (uiInput / 4);
}

void SetOutputs()
{
  analogWrite(3,CalculateAnalogOut(g_uiAnInLightSnsr));
}