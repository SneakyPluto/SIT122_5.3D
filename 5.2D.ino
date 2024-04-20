#include <MeAuriga.h>
MeLightSensor lightSensorRight(PORT_11);  //Right hand lights sensor
MeLightSensor lightSensorLeft(PORT_12);   // Left hand light sensor

MeUltrasonicSensor ultrasonic(PORT_10); //

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
int16_t moveSpeed = 200;

// Threshold to determine light source direction
const int threshold = 6;  // Adjust as needed based on your environment

void Forward(void) {
  Encoder_1.setMotorPwm(-moveSpeed);  // setMotorPwm writes to the encoder controller
  Encoder_2.setMotorPwm(moveSpeed);   // so setting the speed change instantly
}
void Backward(void) {
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed);
}

void BackwardAndTurnLeft(void) {
  Encoder_1.setMotorPwm(moveSpeed / 4);
  Encoder_2.setMotorPwm(-moveSpeed);
}

void BackwardAndTurnRight(void) {
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed / 4);
}

void TurnLeft(void) {
  Encoder_1.setMotorPwm(-moveSpeed);
  Encoder_2.setMotorPwm(moveSpeed / 2);
}

void TurnRight(void) {
  Encoder_1.setMotorPwm(-moveSpeed / 2);
  Encoder_2.setMotorPwm(moveSpeed);
}

void TurnLeft1(void) {
  Encoder_1.setMotorPwm(-moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed);
}

void TurnRight1(void) {
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(moveSpeed);
}

void Stop(void) {
  Encoder_1.setMotorPwm(0);
  Encoder_2.setMotorPwm(0);
}

void ChangeSpeed(int16_t spd) {
  moveSpeed = spd;
}

void setup() {
  Serial.begin(115200);
  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

void loop() {
  //If there is an object within 15cm, move backwards
  if (ultrasonic.distanceCm() < 25) {
    Backward();
    ChangeSpeed(60);
    delay(500);
  }
  // Compare sensor values
  if (lightSensorRight.read() > lightSensorLeft.read() + threshold) {
    // Light source is on the left
    Serial.println("Light source detected on the right.");
    // Spin right (adjust motor control commands as needed)
    // Example: turn left at a certain speed;
    ChangeSpeed(80);
    // Perform the spin right
    TurnRight1();
    delay(600);
    TurnRight();
    delay(350);
  } else if (lightSensorLeft.read() > lightSensorRight.read() + threshold) {
    // Light source is on the right
    Serial.println("Light source detected on the left.");
    // Spin left (adjust motor control commands as needed)
    // Example: turn right at a certain speed
    ChangeSpeed(80);
    // Perform the spin left
    TurnLeft1();
    delay(600);
    TurnLeft();
    delay(350);
  } else {
    // No significant difference, keep spinning or stop
    Serial.println("No clear light source direction.");
    // Example: continue spinning or stop
    delay(1000);
    Stop();
  }
  // Print the results to the serial monitor
  //Serial.println(lightSensorRight.read()); // Brightness value from 0-1023
  // Serial.print("Left value = ");
  // Serial.println(lightSensorLeft.read());
  // Serial.print("Right value = ");
  // Serial.println(lightSensorRight.read());
  // delay(4000); // Wait 50 milliseconds before next measurement
}
