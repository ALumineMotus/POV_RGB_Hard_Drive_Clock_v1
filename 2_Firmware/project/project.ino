// ================================================================================================
//! @file   project.ino
//!
//! @brief  Main program for the hard drive clock v1
//! @author ALumineMotus
//! @date   06/04/2024
// ================================================================================================

// ================================================================================================
// =========================================== Includes ===========================================
// ================================================================================================

#include "rgb_hsv.h"

// ================================================================================================
// =========================================== Constants ==========================================
// ================================================================================================

// Parameters
#define MOTOR_STEPS 36 // Number of motor steps

// Buttons pin
#define BUTTON_HOUR_PIN   A5
#define BUTTON_MINUTE_PIN A4
#define BUTTON_SECOND_PIN A3
#define BUTTON_STEP_PIN   A2
#define BUTTON_OFFSET_PIN A1
#define BUTTON_COLOR_PIN  A0

// LED color pin
#define COLOR_BLUE_PIN 10
#define COLOR_GREEN_PIN 9
#define COLOR_RED 11

// LED activation pin
#define LED0_PIN 1
#define LED1_PIN 0
#define LED2_PIN 2
#define LED3_PIN 3
#define LED4_PIN 4
#define LED5_PIN 5
#define LED6_PIN 6
#define LED7_PIN 7

// Motor pin
#define MOTOR_U_PIN 8
#define MOTOR_V_PIN 12
#define MOTOR_W_PIN 13

// ================================================================================================
// =========================================== Variables ==========================================
// ================================================================================================

uint32_t initTime          = millis(); // Time initialized at start of the Arduino
uint8_t  hourOffset        = 0;        // Offset time used to display correct time
uint8_t  minuteOffset      = 0;        // Offset time used to display correct time
uint8_t  secondOffset      = 0;        // Offset time used to display correct time
uint16_t durationStepMicros    = 10000;
uint16_t durationStepMicrosMin = 1500;
uint16_t delayOffsetMicros = 0;        // Delay offset used to shift when to display time
uint8_t  motorStep         = 0;        // Motor step used to follow the hard disk position
uint8_t  motorStepOffset   = 0;        // Motor step offset used to shift when to display time
uint8_t  colorMode         = 0;        // Color LED mode (White, Green, ..., rainbow)
bool     rainbowActivated  = false;    // Boolean used for led color variation when activated

// ================================================================================================
// ============================================ Setup =============================================
// ================================================================================================

void setup()
{
  // ===== Buttons =====
  
  pinMode(BUTTON_HOUR_PIN,   INPUT);
  pinMode(BUTTON_MINUTE_PIN, INPUT);
  pinMode(BUTTON_SECOND_PIN, INPUT);
  pinMode(BUTTON_STEP_PIN,   INPUT);
  pinMode(BUTTON_OFFSET_PIN, INPUT);
  pinMode(BUTTON_COLOR_PIN,  INPUT);
    
  // ===== LED =====
  
  pinMode(LED0_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);
  pinMode(LED6_PIN, OUTPUT);
  pinMode(LED7_PIN, OUTPUT);
  
  // Handle LED color
  pinMode(COLOR_BLUE_PIN,  OUTPUT);
  pinMode(COLOR_GREEN_PIN, OUTPUT);
  pinMode(COLOR_RED,       OUTPUT);

  // Pin 9, 10, 11 PWM frequency set to about 42khz
  setPwmFrequency(COLOR_BLUE_PIN,  1);
  setPwmFrequency(COLOR_GREEN_PIN, 1);
  setPwmFrequency(COLOR_RED,       1);

  turnLEDWhite();
  
  // ===== Motor =====
  
  pinMode(MOTOR_U_PIN, OUTPUT);
  pinMode(MOTOR_V_PIN, OUTPUT);
  pinMode(MOTOR_W_PIN, OUTPUT);
}

// ================================================================================================
// =========================================== Main loop ==========================================
// ================================================================================================

void loop() 
{
  // Rotate motor of 1/12 rotation
  rotateMotor();

  // Display time
  if ((motorStep + motorStepOffset) * 12 % MOTOR_STEPS == 0)
  {
    delayMicroseconds(delayOffsetMicros);
    int mesureBegin = micros();
    displayTime();
    delayMicroseconds(200);
    turnLEDOff();
    int mesureEnd = micros();
    delayMicroseconds(durationStepMicros + mesureBegin - mesureEnd - delayOffsetMicros);
  }
  else
  {
    delayMicroseconds(durationStepMicros);
  }

  if(rainbowActivated == true && motorStep == 0)
  {
    rainbow();
  }
  
  // Read interruptors state
  readInputs();
}

// ================================================================================================
// =========================================== Functions ==========================================
// ================================================================================================

// ================================================================================================
//! @brief Read interruptors value and execute corresponding actions
// ================================================================================================
void readInputs()
{
  static int sprev = 0;
  int periode =int(millis()/200);
  int scurrent=int(periode%10);
  
  if (sprev != scurrent)
  {
  // Read if interruptors are pushed
  int buttonHourPushed   = digitalRead(BUTTON_HOUR_PIN);
  int buttonMinutePushed = digitalRead(BUTTON_MINUTE_PIN);
  int buttonSecondPuched = digitalRead(BUTTON_SECOND_PIN);
  int buttonStepPushed   = digitalRead(BUTTON_STEP_PIN);
  int buttonOffsetPuched = digitalRead(BUTTON_OFFSET_PIN);
  int buttonColorPushed  = digitalRead(BUTTON_COLOR_PIN);
  
  // Do corresponding actions
  if(buttonHourPushed==HIGH)   incrementHour();
  if(buttonMinutePushed==HIGH) incrementMinute();
  if(buttonSecondPuched==HIGH) incrementSecond();
  if(buttonStepPushed==HIGH)   incrementMotorStepOffet();
  if(buttonOffsetPuched==HIGH) incrementDisplayOffset();
  if(buttonColorPushed==HIGH)  changeColor();
  }
  
  sprev=scurrent;
}

// ================================================================================================
//! @brief Function called when user press "H" button -> Hour button
//!        Add 1 hour on displayed time
// ================================================================================================
void incrementHour()
{
  if (hourOffset < 23)
  {
    hourOffset++;
  }
  else
  {
    hourOffset = 0;
  }
}

// ================================================================================================
//! @brief Function called when user press "M" button -> Minute button
//!        Add 1 minute on displayed time
// ================================================================================================
void incrementMinute()
{
  if (minuteOffset < 59)
  {
    minuteOffset++;
  }
  else
  {
    minuteOffset = 0;
  }
}

// ================================================================================================
//! @brief Function called when user press "S" button -> Second button
//!        Add 1 second on displayed time
// ================================================================================================
void incrementSecond()
{
  if (secondOffset < 59)
  {
    secondOffset++;
  }
  else
  {
    secondOffset = 0;
  }
}

// ================================================================================================
//! @brief Function called when user press "P" button -> Motor step offset button
//!        Add one "virtual" step to hard disk position, in order to shift displayed numbers
// ================================================================================================
void incrementMotorStepOffet()
{
  if (motorStepOffset < MOTOR_STEPS)
  {
    motorStepOffset++;
  }
  else
  {
    motorStepOffset = 0;
  }
}

// ================================================================================================
//! @brief Function called when user press "O" button -> Display offset button
//!        Shift displayed number in order to align numbers to LED
// ================================================================================================
void incrementDisplayOffset()
{
  static int offsetAAjouter = 10;
  if (delayOffsetMicros >= durationStepMicrosMin - 800)
  {
    offsetAAjouter = -10;
  }
  else if (delayOffsetMicros <= 0)
  {
    offsetAAjouter = 10;
  }

  delayOffsetMicros += offsetAAjouter;
}

// ================================================================================================
//! @brief Function called when user press "C" button -> Color button
//!        Change displayed color
// ================================================================================================
void changeColor()
{
  // Change color mode (White, Green, ..., rainbow)
  if (colorMode < 7)
  {
    colorMode = colorMode + 1;
  }
  else
  {
    colorMode = 0;
  }

  // Change LED color according to choosen color mode
  switch (colorMode)
  {
    case 0:
      turnLEDWhite();
      rainbowActivated = false;
      break;
    case 1:
      turnLEDGreen();
      rainbowActivated = false;
      break;
    case 2:
      turnLEDRed();
      rainbowActivated = false;
      break;
    case 3:
      turnLEDBlue();
      rainbowActivated = false;
      break;
    case 4:
      turnLEDMagenta();
      rainbowActivated = false;
      break;
    case 5:
      turnLEDCyan();
      rainbowActivated = false;
      break;
    case 6:
      turnLEDYellow();
      rainbowActivated = false;
      break;
    case 7:
      rainbowActivated = true;
      break;
  }
}

void turnLEDWhite()
{
  digitalWrite(COLOR_BLUE_PIN,  LOW);
  digitalWrite(COLOR_GREEN_PIN, LOW);
  digitalWrite(COLOR_RED,       LOW);
}

void turnLEDGreen()
{
  digitalWrite(COLOR_BLUE_PIN,  HIGH);
  digitalWrite(COLOR_GREEN_PIN, LOW);
  digitalWrite(COLOR_RED,       HIGH);
}

void turnLEDRed()
{
  digitalWrite(COLOR_BLUE_PIN,  HIGH);
  digitalWrite(COLOR_GREEN_PIN, HIGH);
  digitalWrite(COLOR_RED,       LOW);
}

void turnLEDBlue()
{
  digitalWrite(COLOR_BLUE_PIN,  LOW);
  digitalWrite(COLOR_GREEN_PIN, HIGH);
  digitalWrite(COLOR_RED,       HIGH);
}

void turnLEDMagenta()
{
  digitalWrite(COLOR_BLUE_PIN,  LOW);
  digitalWrite(COLOR_GREEN_PIN, HIGH);
  digitalWrite(COLOR_RED,       LOW);
}

void turnLEDCyan()
{
  digitalWrite(COLOR_BLUE_PIN,  HIGH);
  digitalWrite(COLOR_GREEN_PIN, LOW);
  digitalWrite(COLOR_RED,       LOW);
}

void turnLEDYellow()
{
  digitalWrite(COLOR_BLUE_PIN,  LOW);
  digitalWrite(COLOR_GREEN_PIN, LOW);
  digitalWrite(COLOR_RED,       HIGH);
}

// ================================================================================================
//! @brief Change LED color according to the HSV wheel
// ================================================================================================
void rainbow()
{  
  static int hue = 0;
  
  RgbColor rgb = HsvToRgb({hue, 255, 255});

  analogWrite(COLOR_BLUE_PIN,  rgb.b);
  analogWrite(COLOR_GREEN_PIN, rgb.g);
  analogWrite(COLOR_RED,       rgb.r);

  if (hue < 255)
  {
    hue++;
  }
  else
  {
    hue = 0;
  }
}

// ================================================================================================
//! @brief Rotate motor of 1/12 rotation
// ================================================================================================
void rotateMotor()
{
  incrementMotorStep();

  // Rotate motor of 1/12 rotation
  switch (motorStep % 6)
  {
    case 0:
      digitalWrite(MOTOR_U_PIN, HIGH);
      digitalWrite(MOTOR_V_PIN, LOW);
      digitalWrite(MOTOR_W_PIN, LOW);
      break;
    case 1:
      digitalWrite(MOTOR_U_PIN, HIGH);
      digitalWrite(MOTOR_V_PIN, HIGH);
      digitalWrite(MOTOR_W_PIN, LOW);
      break;
    case 2:
      digitalWrite(MOTOR_U_PIN, LOW);
      digitalWrite(MOTOR_V_PIN, HIGH);
      digitalWrite(MOTOR_W_PIN, LOW);
      break;
    case 3:
      digitalWrite(MOTOR_U_PIN, LOW);
      digitalWrite(MOTOR_V_PIN, HIGH);
      digitalWrite(MOTOR_W_PIN, HIGH);
      break;
    case 4:
      digitalWrite(MOTOR_U_PIN, LOW);
      digitalWrite(MOTOR_V_PIN, LOW);
      digitalWrite(MOTOR_W_PIN, HIGH);
      break;
     case 5:
      digitalWrite(MOTOR_U_PIN, HIGH);
      digitalWrite(MOTOR_V_PIN, LOW);
      digitalWrite(MOTOR_W_PIN, HIGH);
      break;
  }

  // Increase motor speed
  if (durationStepMicros > durationStepMicrosMin)
  {
    int delayToRemove = durationStepMicros/1000;

    if (delayToRemove == 0)
    {
      delayToRemove = 1;
    }
    durationStepMicros -= delayToRemove;
  }
}

// ================================================================================================
//! @brief Increment hard drive disk position
// ================================================================================================
void incrementMotorStep()
{
  if (motorStep < MOTOR_STEPS - 1)
  {
    motorStep++;
  }
  else
  {
    motorStep = 0;
  }
}

// ================================================================================================
//! @brief Determine which LED to turn on depending on the time to display, the LED position,
//!        and the hard drive disk position
// ================================================================================================
void displayTime()
{
  // ===== Determin time to display =====
  uint32_t timeOffset = hourOffset * 3600000 + minuteOffset * 60000 + secondOffset * 1000;
  unsigned long timeToDisplay =(millis()+timeOffset-initTime)/1000;

  // ===== Retrieve digit for Hours, Minutes, Secondes =====
  
  // Hour:
  unsigned long hours = (timeToDisplay / 3600)%24;
  int hd=int(hours/10);                 // Tens
  int hu=int (hours%10);                // Units
  timeToDisplay = timeToDisplay % 3600; // Retrieve time left (less than 1 hour)

  // Minutes:
  unsigned long minutes = timeToDisplay / 60;
  int md=int(minutes/10);             // Tens
  int mu=int(minutes%10);             // Units
  timeToDisplay = timeToDisplay % 60; // Retrieve time left (less than 1 minute)

  // Seconds:
  unsigned long seconds = timeToDisplay;
  int sd=int(seconds/10);  // Tens
  int su=int(seconds%10);  // Units

  // ===== Determine LED to turn on =====

  int diskPosition = (motorStep + motorStepOffset) * 12 / MOTOR_STEPS % 12;

  bool displayHourTens       = !(hd == (diskPosition + 12 - 8) % 12); // Colomn 8
  bool displayHourUnits      = !(hu == (diskPosition + 12 - 7) % 12); // Colomn 7
  bool displayTimeSeparator1 = !(diskPosition == 4);                  // Colomn 6
  bool displayMinuteTens     = !(md == (diskPosition + 12 - 5) % 12); // Colomn 5
  bool displayMinuteUnits    = !(mu == (diskPosition + 12 - 4) % 12); // Colomn 4
  bool displayTimeSeparator2 = !(diskPosition == 1);                  // Colomn 3
  bool displaySecondTens     = !(sd == (diskPosition + 12 - 2) % 12); // Colomn 2
  bool displaySecondUnits    = !(su == (diskPosition + 12 - 1) % 12); // Colomn 1

  digitalWrite(LED7_PIN, displayHourTens);
  digitalWrite(LED6_PIN, displayHourUnits);
  digitalWrite(LED5_PIN, displayTimeSeparator1);
  digitalWrite(LED4_PIN, displayMinuteTens);
  digitalWrite(LED3_PIN, displayMinuteUnits);
  digitalWrite(LED2_PIN, displayTimeSeparator2);
  digitalWrite(LED1_PIN, displaySecondTens);
  digitalWrite(LED0_PIN, displaySecondUnits);
}

// ================================================================================================
//! @brief Turn LED off
// ================================================================================================
void turnLEDOff()
{
  digitalWrite(LED7_PIN, HIGH);
  digitalWrite(LED6_PIN, HIGH);
  digitalWrite(LED5_PIN, HIGH);
  digitalWrite(LED4_PIN, HIGH);
  digitalWrite(LED3_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED0_PIN, HIGH);
}

// ================================================================================================
//! @brief  Configure PWM output depending on pin and desired divisor
//!
//! @param[in] pin     (int) : Pin ID of the pin to configure
//! @param[in] divisor (int) : Divisor used to configure PWM frequency
// ================================================================================================
void setPwmFrequency(int pin, int divisor)
{
   byte mode;
   if(pin == 5 || pin == 6 || pin == 9 || pin == 10)
   {
      switch(divisor)
      {
         case 1: mode = 0x01; break;
         case 8: mode = 0x02; break;
         case 64: mode = 0x03; break;
         case 256: mode = 0x04; break;
         case 1024: mode = 0x05; break;
         default: return;
      }
      if(pin == 5 || pin == 6)
      {
         TCCR0B = (TCCR0B & 0b11111000) | mode;
      }
      else
      {
         TCCR1B = (TCCR1B & 0b11111000) | mode;
      }
   }
   else if(pin == 3 || pin == 11)
   {
      switch(divisor)
      {
         case 1: mode = 0x01; break;
         case 8: mode = 0x02; break;
         case 32: mode = 0x03; break;
         case 64: mode = 0x04; break;
         case 128: mode = 0x05; break;
         case 256: mode = 0x06; break;
         case 1024: mode = 0x7; break;
         default: return;
      }
      TCCR2B = (TCCR2B & 0b11111000) | mode;
   }
}
