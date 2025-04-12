#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

/*********Definitions**************/
/*** Timing Definitions ******/
#define TIME_1MS_100MS    100 
#define TIME_1MS_500MS    500
#define TIME_1MS_1S       1000
#define TIME_1S_5S        5
#define TIME_CHECK_INPUTS 100//TIME_1MS_100MS
#define TIME_REFRESH_LCD  TIME_1MS_500MS

/***Input and Sampling********/
#define NUM_ANALOG_SAMPLES  8
#define TRUE                1
#define FALSE               0

// Initialize the LCD object, set the LCD I2C address to 0x27 for a 20x4 display
hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip

// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

/***** Global Variables ***********/
unsigned int g_uiGenericSecondTimer = 0;    //this will keep track of the number of seconds the program has run, until an overflow occurs....
unsigned int g_uiSendMsgTimer = 0;          //when this timer expires, send a message then reset timer
char* g_cLCDLine1[20];// = " Hello World!! :)   ";
char g_cLCDLine2[20];
char g_cLCDLine3[20];
unsigned char g_ucCheckInputs = 0; // flag used to triggger input reads
unsigned char g_bRefreshLCD = TRUE; //flag set in UpdateTimers() set when it's time to call SetLCDOutput()

//*** Inputs to be read in CheckInputs() ***
unsigned int g_uiAnInLightSnsr = 0;

/****** Start Code ****************/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  lcd.begin(LCD_COLS, LCD_ROWS);
  //lcd.backlight();
  g_cLCDLine1[0] = " Hello World!! :)   ";
}

void loop() {
  // put your main code here, to run repeatedly:
  UpdateTimers();
  while(g_ucCheckInputs)  //Timer set elapsed to set inputs
  {
    g_ucCheckInputs--;
    CheckInputs();
  }
  if(g_bRefreshLCD)
  {
    g_bRefreshLCD = FALSE;
    SetLCDOutput();
  }
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
  static unsigned int r_uiMilliCounter = 0, r_uiInputReadTimer = 0, r_uiLCDRefreshTimer = 0;
  
  r_ulPrevMillis = ulCurMillis;
  
  r_uiMilliCounter += ulMillisUpdate;

  Serial.println(ulMillisUpdate);

  while (r_uiMilliCounter >= TIME_1MS_1S)    //Update second based timers here
  {
    r_uiMilliCounter -= TIME_1MS_1S;
    
    g_uiGenericSecondTimer++;
    
    if (g_uiSendMsgTimer)
      g_uiSendMsgTimer--;
  }

  r_uiInputReadTimer += ulMillisUpdate;
  while (r_uiInputReadTimer >= TIME_CHECK_INPUTS)    //Update input read timer
  {
    r_uiInputReadTimer -= TIME_CHECK_INPUTS;
    g_ucCheckInputs++;
  }

  r_uiLCDRefreshTimer += ulMillisUpdate;
  while (r_uiLCDRefreshTimer >= TIME_REFRESH_LCD)    //Update input read timer
  {
    r_uiLCDRefreshTimer -= TIME_REFRESH_LCD;
    g_bRefreshLCD = TRUE;
  }
}

void CheckInputs()
{
    static unsigned short g_usLightSnsrTotal = 0;
    static unsigned char g_ucAnalogSamples = NUM_ANALOG_SAMPLES;
    
    if (g_ucAnalogSamples--)
    {
      g_usLightSnsrTotal += analogRead(0);
    }
    else  //done samlping, get average
    {
      g_uiAnInLightSnsr = g_usLightSnsrTotal / NUM_ANALOG_SAMPLES;  //average the samples
      g_usLightSnsrTotal = 0;                   //reset for next roundof sampling
      g_ucAnalogSamples = NUM_ANALOG_SAMPLES;   //reset for next roundof sampling
    }
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
  lcd.print(g_cLCDLine1[0]);
  // Move to the second row and print the string
  lcd.setCursor(0, 1);
  lcd.print("Sys Uptime: ");
  lcd.print(g_uiGenericSecondTimer);
  lcd.print("s");
  // Move to the third row and print the string
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  // Move to the fourth row and print the string
  lcd.setCursor(0, 2);
  lcd.print("Light Snsr:");
  lcd.print(g_uiAnInLightSnsr); 
}
