/* Functions */
void logger(String Message) {
#ifdef DEBUG
  Serial.println(Message);
#endif
}

void MatrixInitCoordinates() {
  switch (my_mode) {
    case ModeText:
      lhY = 16;
      rhY = 32;
      break;
    case ModeTrafficAdvisor:
      lhY = -sizeof(aTrafficAdvisorLeft);
      rhY = 16;
      break;
    case ModeEQ:
      lhY = 5;
      rhY = 5;
      break;
    case ModeScanner:
    case ModeEyes:
    default:
      lhY = 0;
      rhY = 0;
      break;
  }
}

void MatrixClear(bool doWrite = false) {
  Matrix[MatrixLeft].clear();
  Matrix[MatrixRight].clear();

  if (doWrite) {
    Matrix[MatrixLeft].writeDisplay();
    Matrix[MatrixRight].writeDisplay();
  }
}

void MatrixSetBrightness(int i) {
  if (my_brightness == i) { return; }
  my_brightness = i;
  Matrix[MatrixLeft].setBrightness(my_brightness);
  Matrix[MatrixRight].setBrightness(my_brightness);
}

void MatrixSetSpeed(int i) {
  if (my_speed == i) { return; }
  my_speed = i;
}

void MatrixSetText(int i) {
  if (my_text == i) { return; }
  my_text = i;
  MatrixInitCoordinates();
}

void MatrixSetMode(int i) {
  if (my_mode == i) { return; }
  my_mode = i;
  MatrixInitCoordinates();
}

void MatrixSetIsOn(bool b) {
  if (!b && isOn) { MatrixClear(true); }
  isOn = b;
}

void MatrixAllOn() {
  for (int i = 0; i < 32; i = i + 1) {
    if (i < 16) {
      Matrix[MatrixRight].drawBitmap(0, i, aEQ[8], 8, 2, LED_ON);
    }
    else {
      Matrix[MatrixLeft].drawBitmap(0, i - 16, aEQ[8], 8, 2, LED_ON);
    }
  }
  Matrix[MatrixLeft].writeDisplay();
  Matrix[MatrixRight].writeDisplay();
}

void MatrixTrafficAdvisor() {
  Matrix[MatrixLeft].drawBitmap(0, lhY, aTrafficAdvisorLeft , 8, (sizeof(aTrafficAdvisorLeft)), LED_ON);
  Matrix[MatrixLeft].writeDisplay();
  lhY++;
  if (lhY == 16) {
    lhY = -sizeof(aTrafficAdvisorLeft);
  }
  Matrix[MatrixRight].drawBitmap(0, rhY, aTrafficAdvisorRight , 8, (sizeof(aTrafficAdvisorRight)), LED_ON);
  Matrix[MatrixRight].writeDisplay();
  rhY--;
  if (rhY == -sizeof(aTrafficAdvisorRight)) {
    rhY = 16;
  }
}

void MatrixScanner() {
  Matrix[MatrixLeft].drawBitmap(-8, 0, aScanner[lhY], 16, 16, LED_ON);
  Matrix[MatrixLeft].writeDisplay();
  Matrix[MatrixRight].drawBitmap(0, 0, aScanner[lhY], 16, 16, LED_ON);
  Matrix[MatrixRight].writeDisplay();
  
  /* Sweep direction is stored in rhY */
  if (rhY == 0) {
    lhY++;
  }
  else {
    lhY--;
  }
  if (lhY == 32) {
    rhY = 1;
  }
  else if (lhY == 0) {
    rhY = 0;
  }
  else { }
}

void MatrixEQ() {
  for (int i = 0; i < 32; i = i + 2) {
    int r = random(1, 9);
    while (abs(lhY - r) > 2) {
      r = random(1, 9);
    }
    /* Previous value is stored in lhY */
    lhY = r;

    if (i < 16) {
      Matrix[MatrixRight].drawBitmap(0, i, aEQ[r], 8, 2, LED_ON);
    }
    else {
      Matrix[MatrixLeft].drawBitmap(0, i - 16, aEQ[r], 8, 2, LED_ON);
    }
  }
  Matrix[MatrixLeft].writeDisplay();
  Matrix[MatrixRight].writeDisplay();
}

void MatrixEyes() {
  Matrix[MatrixLeft].drawBitmap(-8, 0, aEyes[lhY], 16, 16, LED_ON);
  Matrix[MatrixLeft].writeDisplay();
  Matrix[MatrixRight].drawBitmap(0, 0, aEyes[lhY], 16, 16, LED_ON);
  Matrix[MatrixRight].writeDisplay();

  delay(SpeedSlow);
  
  /* Animation direction is stored in rhY */
  if (rhY == 0) {
    lhY++;
  }
  else {
    lhY--;
  }
  if (lhY == 3) {
    rhY = 1;

    if (!random(0,4)) { delay(750); }
    else { delay(5000); }
  }
  else if (lhY == 0) {
    rhY = 0;
  }
  else { }
}

void MatrixText() {
  int s = TextSize[my_text];
  Matrix[MatrixLeft].drawBitmap(0, lhY, TextData[my_text] , 8, s, LED_ON);
  Matrix[MatrixLeft].writeDisplay();
  Matrix[MatrixRight].drawBitmap(0, rhY, TextData[my_text] , 8, s, LED_ON);
  Matrix[MatrixRight].writeDisplay();
  lhY--;
  rhY--;
  if (lhY == -s) { lhY = 32; }
  if (rhY == -s) { rhY = 32; }
}

void MatrixDisplay() {
  switch (my_mode) {
    case ModeText:
      MatrixText();
      break;

    case ModeTrafficAdvisor:
      MatrixTrafficAdvisor();
      break;

    case ModeScanner:
      MatrixScanner();
      break;

    case ModeEQ:
      MatrixEQ();
      break;

    case ModeEyes:
      MatrixEyes();
      break;
  }
}
