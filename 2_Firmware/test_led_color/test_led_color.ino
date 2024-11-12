// ================================================================================================
//! @file   test_led_color.ino
//!
//! @brief  Test LED program for the hard drive clock v1
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

// ================================================================================================
// ============================================ Setup =============================================
// ================================================================================================

void setup()
{
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
}

// ================================================================================================
// =========================================== Main loop ==========================================
// ================================================================================================

void loop()
{
  turnLEDOff();

  turnLEDWhite();
    
  digitalWrite(LED0_PIN, LOW);
  delay(500);
  digitalWrite(LED1_PIN, LOW);
  delay(500);
  digitalWrite(LED2_PIN, LOW);
  delay(500);
  digitalWrite(LED3_PIN, LOW);
  delay(500);
  digitalWrite(LED4_PIN, LOW);
  delay(500);
  digitalWrite(LED5_PIN, LOW);
  delay(500);
  digitalWrite(LED6_PIN, LOW);
  delay(500);
  digitalWrite(LED7_PIN, LOW);
  delay(500);

  for (int hue = 0; hue<255; hue++)
  {
    RgbColor rgb = HsvToRgb({hue, 255, 255});
  
    analogWrite(COLOR_BLUE_PIN, rgb.b);
    analogWrite(COLOR_GREEN_PIN, rgb.g);
    analogWrite(COLOR_RED, rgb.r);
  
    delay(50);
  }
}

// ================================================================================================
// =========================================== Functions ==========================================
// ================================================================================================

void turnLEDWhite()
{
  digitalWrite(COLOR_BLUE_PIN,  LOW);
  digitalWrite(COLOR_GREEN_PIN, LOW);
  digitalWrite(COLOR_RED,       LOW);
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
