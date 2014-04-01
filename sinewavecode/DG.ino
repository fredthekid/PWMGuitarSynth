//Using Timer 0 and Timer 2
//Pin 6  for D speaker
//Pin 11 for G speaker


//ANALOG PINS (INPUTS)
const int Dstring = 0;
const int Gstring = 1;
const int Dfret = 2;
const int Gfret = 3;

//DIGITAL PINS (OUTPUTS)
const int Dspeaker = 6;
const int Gspeaker = 11;

//For controlling the sine lookup table
byte sine[500];
int indexcntD = 0;
int indexcntG = 0;

char strings_plucked = 0;

void setup()
{
  //Setting up speakers for output.
  pinMode(Dspeaker,OUTPUT);
  pinMode(Gspeaker,OUTPUT);
  
  //Setting up timers
  
  //Enable interrupts
  interrupts();
  
}

void loop()
{
  strings_plucked = 0x03 & read_strings();
  
  switch(strings_plucked)
  {
    case 0b00:
    
    case 0b01:
    
    case 0b10:
    
    case 0b11:
    
    default:
}

char read_strings()
{
    // returned 00 = no strings plucked
    // returned 01 = Dstring plucked only
    // returned 10 = Gstring plucked only
    // returned 11 = Dstring and Gstring plucked
    
    char D_string = (analogRead(Dstring) < 695) ? 0b01 : 0b00;
    char G_string = (analogRead(Gstring) < 770) ? 0b10 : 0b00;
    return (D_string | G_string);
}













void calcSine()
{
  for(int n = 0; n < 500; n++)
  {
    sine[n] = 127*sin(pi * n / 255) + 128;
  }
}

ISR(TIMER0_OVF_vect) //for D
{
  if(indexcntG >= 499)
  {
    indexcntG = 0;
  }
  
  else
  {
    indexcntG += 1;
  }
}

ISR(TIMER2_OVF_vect) //for G
{
  if(indexcntD >= 499)
  {
    indexcntD = 0;
  }
  
  else
  {
    indexcntD += 1;
  } 
}
