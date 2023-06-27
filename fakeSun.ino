/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: ENVIRONMENTAL CONTROLS
*/


void fakeSunsetup() {
  pinMode(sun, OUTPUT);
}

void fakeSunON() {
  digitalWrite(sun, HIGH);
}

void fakeSunOFF() {
  digitalWrite(sun, LOW);
}