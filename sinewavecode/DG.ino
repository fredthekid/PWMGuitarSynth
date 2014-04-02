//Using Timer 1 and Timer 2
//Pin 9 for D speaker
//Pin 11 for G speaker

//BASE TCNTX COUNTS
const int Dcount = 65482;
const int Gcount = 215;

//CONSTANT PI VALUE
const float pi = 3.141592;

//ANALOG PINS (INPUTS)
const int Dstring = 0;
const int Gstring = 1;
const int Dfret = 2;
const int Gfret = 3;

//DIGITAL PINS (OUTPUTS)
const int Dspeaker = 9;
const int Gspeaker = 11;

//LOOKUP TABLE VARIABLES
byte sine[500];
int indexcntD = 0;
int indexcntG = 0;

//SEE WHICH STRINGS ARE PLUCKED
char strings_plucked = 0;

//FOR D
ISR(TIMER1_OVF_vect)
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

//FOR G
ISR(TIMER2_OVF_vect)
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

void setup()
{
    //Create sine lookup table
    calcSine();

    //Setting up speakers for output.
    pinMode(Dspeaker,OUTPUT);
    pinMode(Gspeaker,OUTPUT);

    //////////////////////
    //SETTING UP TIMER 0//
    //////////////////////

    //bits 7,6 is clears OCR0A on compare match and sets OCR0A at bottom
    //bits 1,0 is WGM11, WGM10. WGM11 and WGM10 works with bits in TCCR1B to determine mode. FAST PWM in this config with 0xFF as top.
    //bits not mentioned are not used
    TCCR1A = 0b10000001;

    //bits 2,1,0 determine prescaling. no prescale in this config
    //bit 4,3 is WGM13. WGM12 works with bits in TCCR0A to determine mode. FAST PWM in this config with 0xFF as top.
    //bits not mentioned are not used 
    TCCR1B = 0b00001001;

    //Not Used;
    TCCR1C = 0b00000000;

    //timer/counter interrupt mask register
    //last bit is enabled to allow overflow interrupts;
    //bits not mentioned are not used
    TIMSK1 = 0b00000001;

    //////////////////////
    //SETTING UP TIMER 2//
    //////////////////////

    //bits 7,6 is clears OCR2A on compare match and sets OCR2A at bottom
    //bits 1,0 is WGM1, WGM0. WGM2 and WGM1 works with bits in TCCR2B to determine mode. FAST PWM in this config with 0xFF as top.
    //bits not mentioned are not used
    TCCR2A = 0b10000011; 

    //bits 2,1,0 determine prescaling. no prescale in this config
    //bit 3 is WGM2. WGM2 works with bits in TCCR2A to determine mode. FAST PWM in this config with 0xFF as top.
    //bits not mentioned are not used
    TCCR2B = 0b00000001;

    //timer/counter interrupt mask register
    //last bit is enabled to allow overflow interrupts;
    //bits not mentioned are not used
    TIMSK2 = 0b00000001;

    //Enable interrupts
    interrupts();
}

void loop()
{
    strings_plucked = 0x03 & read_strings(); 
    switch(strings_plucked)
    {
        //Nothing Pressed. Reset counter and make duty cycle = 0
        case 0b00: 
            playNothingAndReset();
            break;

            //Dstring plucked         
        case 0b01: 
            D();
            break;

            //Gstring plucked           
        case 0b10: 
            G();
            break;

            //Dstring and Gstring plucked
        case 0b11: 
            DG();
            break;

        default:   
            playNothingAndReset();
            break;
    }
}
void calcSine()
{
    for(int n = 0; n < 500; n++)
    {
        sine[n] = 127*sin(pi * n / 255) + 128;
    }
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

byte readDfret()
{
    if((analogRead(Dfret) > 50) && (analogRead(Dfret) <= 250)) // third fret
    {
        return 9;
    }
    else if(analogRead(Dfret) > 400 && analogRead(Dfret) <= 600) // second fret
    {
        return 5;
    }
    else if(analogRead(Dfret) > 700 && analogRead(Dfret) <= 1023) // first fret
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

byte readGfret()
{
    if((analogRead(Gfret) > 50) && (analogRead(Gfret) <= 250)) // third fret
    {
        return 9;
    }
    else if(analogRead(Gfret) > 400 && analogRead(Gfret) <= 600) // second fret
    {
        return 5;
    }
    else if(analogRead(Gfret) > 700 && analogRead(Gfret) <= 1023) // first fret
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

void playNothingAndReset()
{
    TCNT1 = 0;
    TCNT2 = 0; 
    OCR1A = 0;
    OCR2A = 0;
    indexcntD = 0;
    indexcntG = 0;
}

void D()
{
    TCNT1 = Dcount + readDfret();
    OCR1A = sine[indexcntD];
}

void G()
{
    TCNT2 = Gcount + readGfret();
    OCR2A = sine[indexcntG];
}

void DG()
{
    TCNT1 = Dcount + readDfret();
    TCNT2 = Gcount + readGfret();
    OCR1A = sine[indexcntD];
    OCR2A = sine[indexcntG];
}

