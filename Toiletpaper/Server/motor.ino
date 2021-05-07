AccelStepper *stepper;

void motorSetup()
{
  stepper = new AccelStepper(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
  delay(2000);
}

void motorLoop()
{
  if (stepper->distanceToGo() == 0)
  {
    // Random change to speed, position and acceleration
    // Make sure we dont get 0 speed or accelerations
    delay(1000);
    stepper->moveTo(rand() % 200);
    stepper->setMaxSpeed((rand() % 200) + 1);
    stepper->setAcceleration((rand() % 200) + 1);
  }
  stepper->run();
}
