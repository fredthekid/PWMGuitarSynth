#define lowEfreq 24096 //used with 16 bit timer 1, prescale 8. final clock speed at 2MHz.
#define Afreq 142 //used with 8 bit timer 0, prescale 1024. final clock speed at 15.6kHz.
#define Dfreq 106 //used with 8 bit timer 2, prescale 1024. final clock speed at 15.6Hz.


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

//
unsigned int read_strings();




void setup()
{
  pinMode(lowEspeaker, OUTPUT);
  pinMode(Aspeaker, OUTPUT);
  pinMode(Dspeaker, OUTPUT);

 //setup for timers
  TCCR2A = 0x63;
  TCCR2B = 0x0F;
  
  TCCR0A = 0x63;
  TCCR0B = 0x0F;
  
  TCCR1A = 0b10100010; //clear oc1a on compare match, set oc1a at bottom. waveform generation mode 14
  TCCR1B = 0b00011010; //waveform generation mode 14, prescale by 8
  OCR1A = 0;
}

void loop()
{
  unsigned int strings_plucked = 0;
  strings_plucked = read_strings();
  
  switch(strings_plucked)
  {
    case 0b000: break;
    
    case 0b001: ICR1 = lowEfreq-2*analogRead(fretboard);
                for(int i = 1; i < 1000; i+=10)
                {
                  OCR1A = ICR1/(2*i);
                  delay(20);
                }
                OCR1A = 0;
                break;
                
    case 0b010: OCR0A = Afreq - map(analogRead(Afret) , 0 , 1023 , 0 , 35);
                for(int i = 0; i!=(OCR0A/2); i++)
                {
                  OCR0B = (OCR0A / 2) + i ;
                }
                break;
                
    case 0b100: OCR2A = Dfreq - map(analogRead(Dfret) , 0 , 1023 , 0 , 35);
                for(int i = 0; i!=(OCR2A/2); i++)
                {
                  OCR2B = (OCR2A / 2) + i ;
                }
                break; 
    }
}

unsigned int read_strings()
{
  unsigned int E_string = (analogRead(lowEstring) < 750) ? 0b001 : 0b000;
  unsigned int A_string = (analogRead(Astring) < 750) ? 0b010 : 0b000;
  unsigned int D_string = (analogRead(Dstring) < 750) ? 0b100 : 0b000;
  
  return (E_string | A_string | D_string);
}
  
