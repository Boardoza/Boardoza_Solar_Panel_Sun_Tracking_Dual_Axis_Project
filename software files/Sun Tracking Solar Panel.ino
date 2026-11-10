
#include <avr/io.h>
#include <avr/interrupt.h>

// =====================================================
// LDR PINS
// =====================================================

const int LDR_LEFT   = A0;   // LEFT
const int LDR_BOTTOM = A1;   // BOTTOM
const int LDR_RIGHT  = A2;   // RIGHT
const int LDR_TOP    = A3;   // TOP


// =====================================================
// SERVO ANGLES
// =====================================================

int xAngle = 90;   // Horizontal servo - D9
int yAngle = 90;   // Vertical servo - D10


// =====================================================
// PERCENTAGE MOVEMENT THRESHOLDS
// =====================================================

const float START_THRESHOLD = 15.0;
const float STOP_THRESHOLD  = 10.0;


// =====================================================
// SERVO ANGLE LIMITS
// =====================================================

const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;


// =====================================================
// TIMER1
// =====================================================

const uint16_t TIMER_TOP = 39999;


// =====================================================
// LDR READING
// =====================================================

const int LDR_SAMPLES = 8;


// =====================================================
// MOVEMENT STATUS
// =====================================================

bool xMoving = false;
bool yMoving = false;


// =====================================================
// ANGLE TO PWM CONVERSION
// =====================================================

uint16_t angleToTicks(int angle)
{
  angle = constrain(
      angle,
      MIN_ANGLE,
      MAX_ANGLE
  );

  uint16_t pulseUs =
      700 + ((uint32_t)angle * 1600UL) / 180;

  return pulseUs * 2;
}


// =====================================================
// TIMER1 CONFIGURATION
// =====================================================

void setupTimer1()
{
  cli();

  // D9  = PB1 = OC1A
  // D10 = PB2 = OC1B

  DDRB |=
      (1 << DDB1) |
      (1 << DDB2);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  ICR1 = TIMER_TOP;

  OCR1A = angleToTicks(xAngle);
  OCR1B = angleToTicks(yAngle);

  // Fast PWM Mode 14

  TCCR1A =
      (1 << COM1A1) |
      (1 << COM1B1) |
      (1 << WGM11);

  TCCR1B =
      (1 << WGM13) |
      (1 << WGM12) |
      (1 << CS11);

  sei();
}


// =====================================================
// LDR READING FUNCTION
// =====================================================

int readLDR(int pin)
{
  long total = 0;

  for (int i = 0; i < LDR_SAMPLES; i++)
  {
    total += analogRead(pin);
  }

  return total / LDR_SAMPLES;
}


// =====================================================
// PERCENTAGE DIFFERENCE CALCULATION
// =====================================================

float calculatePercentDifference(
    int value1,
    int value2
)
{
  float average =
      (value1 + value2) / 2.0;

  if (average <= 0)
  {
    return 0;
  }

  float difference =
      abs(value1 - value2);

  return (difference / average) * 100.0;
}


// =====================================================
// MOVEMENT STEP CALCULATION
// =====================================================

int calculateMovementStep(float percent)
{
  if (percent < START_THRESHOLD)
  {
    return 0;
  }

  if (percent < 20.0)
  {
    return 1;
  }

  if (percent < 30.0)
  {
    return 2;
  }

  if (percent < 45.0)
  {
    return 4;
  }

  if (percent < 60.0)
  {
    return 6;
  }

  return 8;
}


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(9600);

  setupTimer1();

  Serial.println("================================");
  Serial.println("       SOLAR TRACKER");
  Serial.println("================================");

  Serial.println("A0 = LEFT");
  Serial.println("A1 = BOTTOM");
  Serial.println("A2 = RIGHT");
  Serial.println("A3 = TOP");

  Serial.println("--------------------------------");

  Serial.println("X Servo : D9");
  Serial.println("Y Servo : D10");
  Serial.println("PWM : 700 - 2300 us");

  Serial.print("Start Threshold = ");
  Serial.print(START_THRESHOLD);
  Serial.println("%");

  Serial.print("Stop Threshold = ");
  Serial.print(STOP_THRESHOLD);
  Serial.println("%");

  Serial.println("================================");

  delay(1000);
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // ===================================================
  // READ LDR VALUES
  // ===================================================

  int leftValue =
      readLDR(LDR_LEFT);

  int bottomValue =
      readLDR(LDR_BOTTOM);

  int rightValue =
      readLDR(LDR_RIGHT);

  int topValue =
      readLDR(LDR_TOP);


  // ===================================================
  // CALCULATE PERCENTAGE DIFFERENCES
  // ===================================================

  float xPercent =
      calculatePercentDifference(
          rightValue,
          leftValue
      );

  float yPercent =
      calculatePercentDifference(
          topValue,
          bottomValue
      );


  // ===================================================
  // X-AXIS MOVEMENT STATUS
  // ===================================================

  if (!xMoving)
  {
    if (xPercent >= START_THRESHOLD)
    {
      xMoving = true;
    }
  }
  else
  {
    if (xPercent <= STOP_THRESHOLD)
    {
      xMoving = false;
    }
  }


  // ===================================================
  // Y-AXIS MOVEMENT STATUS
  // ===================================================

  if (!yMoving)
  {
    if (yPercent >= START_THRESHOLD)
    {
      yMoving = true;
    }
  }
  else
  {
    if (yPercent <= STOP_THRESHOLD)
    {
      yMoving = false;
    }
  }


  // ===================================================
  // X-AXIS - LEFT / RIGHT
  // ===================================================
  //
  // CORRECTED DIRECTION:
  // Higher right LDR reading -> xAngle increases
  // Higher left LDR reading  -> xAngle decreases
  //

  int xStep =
      calculateMovementStep(xPercent);

  if (xMoving)
  {
    if (rightValue > leftValue)
    {
      xAngle += xStep;
    }
    else if (leftValue > rightValue)
    {
      xAngle -= xStep;
    }
  }


  // ===================================================
  // Y-AXIS - TOP / BOTTOM
  // ===================================================
  //
  // The previous Y-axis direction is preserved.
  //

  int yStep =
      calculateMovementStep(yPercent);

  if (yMoving)
  {
    if (topValue > bottomValue)
    {
      yAngle += yStep;
    }
    else if (bottomValue > topValue)
    {
      yAngle -= yStep;
    }
  }


  // ===================================================
  // ANGLE LIMITS
  // ===================================================

  xAngle =
      constrain(
          xAngle,
          MIN_ANGLE,
          MAX_ANGLE
      );

  yAngle =
      constrain(
          yAngle,
          MIN_ANGLE,
          MAX_ANGLE
      );


  // ===================================================
  // CALCULATE PWM VALUES
  // ===================================================

  uint16_t xTicks =
      angleToTicks(xAngle);

  uint16_t yTicks =
      angleToTicks(yAngle);


  // ===================================================
  // UPDATE PWM OUTPUTS
  // ===================================================

  uint8_t oldSREG = SREG;

  cli();

  OCR1A = xTicks;
  OCR1B = yTicks;

  SREG = oldSREG;


  // ===================================================
  // SERIAL MONITOR OUTPUT
  // ===================================================

  Serial.print("LEFT=");
  Serial.print(leftValue);

  Serial.print(" RIGHT=");
  Serial.print(rightValue);

  Serial.print(" X=");
  Serial.print(xPercent, 1);
  Serial.print("%");

  Serial.print(" XStep=");
  Serial.print(xStep);

  Serial.print(" XAngle=");
  Serial.print(xAngle);

  Serial.print(" || ");

  Serial.print("TOP=");
  Serial.print(topValue);

  Serial.print(" BOTTOM=");
  Serial.print(bottomValue);

  Serial.print(" Y=");
  Serial.print(yPercent, 1);
  Serial.print("%");

  Serial.print(" YStep=");
  Serial.print(yStep);

  Serial.print(" YAngle=");
  Serial.println(yAngle);


  // ===================================================
  // LOOP TIMING
  // ===================================================

  delay(20);
}
