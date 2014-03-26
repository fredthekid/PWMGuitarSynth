const int lowEstring = 0;
const int fretboard = 1;
const int speaker = 9;

void setup()
{
  pinMode(speaker, OUTPUT);
  TCCR1A = 0b10000010; //clear oc1a on compare match, set oc1a at bottom. waveform generation mode 14
  TCCR1B = 0b00011010; //waveform generation mode 14, prescale by 8
  ICR1 = 6060;
  OCR1A = 0;
}

void loop()
{
  if(analogRead(lowEstring) < 750)
  {
    ICR1 = 6060-2*analogRead(fretboard);
    for(int i = 1; i < 1000; i+=10)
    {
      if(analogRead(lowEstring) < 750)
      {
        i = 1;
        ICR1 = 6060-2*analogRead(fretboard);
      }
      
      OCR1A = ICR1/(2*i);
      delay(20);
    }
    OCR1A = 0;
  }
}
