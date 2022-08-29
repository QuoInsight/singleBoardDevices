int pzBzrID = 8; // pin#8 piezo buzzer
int pBtnID = 2; // pin#2 push button

int notesFreq[] {0, 262, 294, 330, 349, 392, 440, 494};

int ledID[] = {
  3, 4, 5, 6, 7, 9, 10, 11, 12, 13
  // no pin#1 ! only 2-13, pin#2 & pin#8 linked to buzzer & button
};
int allLedState = -1;

int chrByte2NoteFreq(byte b) {
  int n=1, o=1; float f = 0;
  if (b>=97 && b<=122) { // a-z
    n = (b-97+6);
  } else if (b>=65 && b<=90) { // A-Z
    n = (b-65+6);
  } else if (b>=48 && b<=57) { // 0-9
    n = (b-48);
  } else {
    return 0;
  }
  o = ceil(n/7.0);  n = n % 7;  if (n==0) n=7;
  return notesFreq[n]*pow(2,o);
}

void playTone(int freq, int msDuration, int repeat) {
  toogleLED( ledID[freq % 10] );
  for (int i = 0; i < repeat; i++) {
    tone(pzBzrID, freq, msDuration); delay(250);
  }
  noTone(pzBzrID);
  toogleLED( ledID[freq % 10] );
}

void toogleLED(int id) {
  if (digitalRead(id) == HIGH) {
    digitalWrite(id, LOW);
  } else {
    digitalWrite(id, HIGH);
  }
}

void toggleAllLED() {
  bool init = false; if (allLedState == -1) init = true;
  if (allLedState == 0) allLedState = 1; else allLedState = 0;
  int ledCount = sizeof(ledID)/sizeof(ledID[0]);
  // https://www.arduino.cc/reference/en/language/variables/utilities/sizeof/
  for (int i = 0; i < ledCount; i++) {
    if (init) pinMode(ledID[i], OUTPUT); // init setup
    if (allLedState==1) digitalWrite(ledID[i], HIGH);
     else digitalWrite(ledID[i], LOW);
  }
}

void setup() {
  Serial.begin(9600); // https://www.arduino.cc/reference/en/language/functions/communication/serial/
  Serial.println("starting..");
  toggleAllLED();
  playTone(262, 200, 2);
  pinMode(pBtnID, INPUT_PULLUP); // init pushed button; pin#2 linked to both button & LED
  // above will also turn on LED2 and not allowing LED2 to be controlled separately !!
}

void loop() {
  if (Serial.available() > 0) {
    // https://www.arduino.cc/reference/en/language/functions/communication/serial/
    byte inByte = Serial.read();  Serial.write(inByte);
    playTone(chrByte2NoteFreq(inByte), 200, 1);
  }
  if (digitalRead(pBtnID) == LOW) {
    Serial.println("button pushed!");
    playTone(262, 200, 3);
    toggleAllLED();
  }
  delay(50);
}
