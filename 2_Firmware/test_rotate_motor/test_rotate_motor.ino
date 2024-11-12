// ================================================================================================
//! @file   test_rotate_motor.ino
//!
//! @brief  Test motor program for the hard drive clock v1
//! @author ALumineMotus
//! @date   06/04/2024
// ================================================================================================

// ================================================================================================
// =========================================== Constants ==========================================
// ================================================================================================

// Motor pin
#define MOTOR_U_PIN 8
#define MOTOR_V_PIN 12
#define MOTOR_W_PIN 13

const bool SPEED_TEST = false;

// ================================================================================================
// =========================================== Variables ==========================================
// ================================================================================================

int delayMicro=16000;

// ================================================================================================
// ============================================ Setup =============================================
// ================================================================================================

void setup()
{
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
  digitalWrite(MOTOR_U_PIN, HIGH);
  digitalWrite(MOTOR_V_PIN, LOW);
  digitalWrite(MOTOR_W_PIN, LOW);
  if (SPEED_TEST == true)
  {
    delayMicroseconds(delayMicro);
    increaseSpeed();
  }
  else
  {
    delay(1000);
  }

  digitalWrite(MOTOR_U_PIN, HIGH);
  digitalWrite(MOTOR_V_PIN, HIGH);
  digitalWrite(MOTOR_W_PIN, LOW);
  if (SPEED_TEST == true)
  {
    delayMicroseconds(delayMicro);
    increaseSpeed();
  }
  else
  {
    delay(1000);
  }

  digitalWrite(MOTOR_U_PIN, LOW);
  digitalWrite(MOTOR_V_PIN, HIGH);
  digitalWrite(MOTOR_W_PIN, LOW);
  if (SPEED_TEST == true)
  {
    delayMicroseconds(delayMicro);
    increaseSpeed();
  }
  else
  {
    delay(1000);
  }

  digitalWrite(MOTOR_U_PIN, LOW);
  digitalWrite(MOTOR_V_PIN, HIGH);
  digitalWrite(MOTOR_W_PIN, HIGH);
  if (SPEED_TEST == true)
  {
    delayMicroseconds(delayMicro);
    increaseSpeed();
  }
  else
  {
    delay(1000);
  }

  digitalWrite(MOTOR_U_PIN, LOW);
  digitalWrite(MOTOR_V_PIN, LOW);
  digitalWrite(MOTOR_W_PIN, HIGH);
  if (SPEED_TEST == true)
  {
    delayMicroseconds(delayMicro);
    increaseSpeed();
  }
  else
  {
    delay(1000);
  };

  digitalWrite(MOTOR_U_PIN, HIGH);
  digitalWrite(MOTOR_V_PIN, LOW);
  digitalWrite(MOTOR_W_PIN, HIGH);
  if (SPEED_TEST == true)
  {
    delayMicroseconds(delayMicro);
    increaseSpeed();
  }
  else
  {
    delay(1000);
  }
}

// ================================================================================================
// =========================================== Functions ==========================================
// ================================================================================================

void increaseSpeed()
{
  if(delayMicro>400)
  {
    int delayToReduce = delayMicro/1000;
    if (delayToReduce == 0)
    {
      delayToReduce = 1;
    }

    delayMicro -= delayToReduce;
  }
}
