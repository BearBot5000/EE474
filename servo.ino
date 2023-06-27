/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: ENVIRONMENTAL CONTROLS
*/


void SERVOsetup() {
  myservo.attach(servopin);
  myservo.write(90);  // move servos to center position -> 90°
}

void openLID() {
  myservo.write(200);
  lidOpen = true;
}

void closeLID() {
  myservo.write(90);
  lidOpen = false;
}
