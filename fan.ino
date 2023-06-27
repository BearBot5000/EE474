/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: ENVIRONMENTAL CONTROLS
*/


void fansetup() {
  pinMode(fan, OUTPUT);
}

void fanON() {
  digitalWrite(fan, HIGH);
}

void fanOFF() {
  digitalWrite(fan, LOW);
}