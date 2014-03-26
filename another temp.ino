
#define lowEfreq 6060 //used with 16 bit timer, prescale 8. final clock speed at 2MHz.
#define Afreq 6060 //used with 16 bit timer, prescale 8. final clock speed at 2MHz.
#define Dfreq 6060 //used with 16 bit timer, prescale 8. final clock speed at 2MHz.


//Analog Pins
const short int lowEstring = 0;
const short int lowEfret = 1;
const short int Astring = 2;
const short int Afret = 3;
const short int Dstring = 4;
const short int Dfret = 5;

//Digital Pins (used for pwm)
const short int lowEspeaker = 9;
const short int Aspeaker = 10;
const short int Dspeaker = 11;


void setup()
{
  pinMode(speaker, OUTPUT);
  TCCR1A = 0b10100010; //clear oc1a on compare match, set oc1a at bottom. waveform generation mode 14
  TCCR1B = 0b00011010; //waveform generation mode 14, prescale by 8
  
  OCR1A = 0;
}

void loop()
{
  if(analogRead(lowEstring) < 750)
  {
    ICR1 = lowEfreq-2*analogRead(fretboard);
    for(int i = 1; i < 1000; i+=10)
    {
      if(analogRead(lowEstring) < 750)
      {
        i = 1;
        ICR1 = lowEfreq-2*analogRead(fretboard);
      }
      
      OCR1A = ICR1/(2*i);
      delay(20);
    }
    OCR1A = 0;
  }
}







void checkstring()
{
  if(
