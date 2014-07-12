void flashLed() {
  for(int i=0; i<5; i++) {
    digitalWrite(13, HIGH);
    delay(5);
    digitalWrite(13, LOW);
    delay(5);
  }
}
