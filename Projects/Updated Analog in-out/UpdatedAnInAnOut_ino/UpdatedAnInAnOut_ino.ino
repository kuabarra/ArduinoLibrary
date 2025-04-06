#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*********Definitions**************/
/*** Timing Definitions ******/
#define TIME_1MS_10MS     10
#define TIME_CHECK_INPUTS TIME_1MS_10MS
#define TIME_1MS_1S  1000
#define TIME_1S_5S   5

// Initialize the LCD object, set the LCD I2C address to 0x27 for a 20x4 display
LiquidCrystal_I2C lcd(0x27, 20, 4);

/***** Global Variables ***********/
unsigned int g_uiGenericSecondTimer = 0;    //this will keep track of the number of seconds the program has run, until an overflow occurs....
unsigned int g_uiSendMsgTimer = 0;          //when this timer expires, send a message then reset timer
unsigned char gucLCDLine1[20];
unsigned char gucLCDLine2[20];
unsigned char gucLCDLine3[20];
unsigned char g_ucCheckInputsTimerMs = 0; // Timer used to keep track of input reading timing

//*** Inputs to be read in CheckInputs() ***
unsigned int g_uiAnInLightSnsr = 0;

/****** Start Code ****************/
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
}

void loop() {
  // put your main code here, to run repeatedly:
  UpdateTimers();
  if (!g_ucCheckInputsTimerMs)  //Timer set elapsed to set inputs
  {
    CheckInputs();
    g_ucCheckInputsTimerMs = TIME_CHECK_INPUTS;
  }
  SetOutputs();
  SetLCDOutput();
   
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

  if (g_ucCheckInputsTimerMs)
    g_ucCheckInputsTimerMs--;
  
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
    unsigned short 
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

void SetLCDOutput()
{
  // Set cursor to the top left corner and print the string on the first row
  lcd.setCursor(0, 0);
  lcd.print(" Hello, world! ");
  // Move to the second row and print the string
  lcd.setCursor(0, 1);
  lcd.print("Sys Uptime: ");
  lcd.print(g_uiGenericSecondTimer);
  lcd.print("s");
  // Move to the third row and print the string
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  // Move to the fourth row and print the string
  lcd.setCursor(0, 3);
  lcd.print("Light Snsr:");
  lcd.print(g_uiAnInLightSnsr); 
}
