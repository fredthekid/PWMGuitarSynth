const int lowEstring = 0;
const int fretboard = 1;
const int speaker = 11;

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = 0x63;
  TCCR2B = 0x0F;
  
  OCR2A = 40;
  OCR2B = OCR2A/2;
}

void loop()
{
  /*if(analogRead(lowEstring) < 750)
  {
    OCR2A = 47;
    for(int i = 1; i < 1000; i+=10)
    {
      if(analogRead(lowEstring) < 750)
      {
        i = 1;
        OCR2A = 47;
      }
            delay(20);
    }
    OCR2A = 0;
  }
}
*/
}
