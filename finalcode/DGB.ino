//Note base frequency
#define Dfreq 27 // timer 0, prescaled to 15.6Hz
#define Gfreq 2551 // timer 1, prescaled to 2MHz
#define Bfreq 16 //timer 2, prescaled to 15.6Hz

//ANALOG PINS (INPUTS)
const int Dstring = 0;
const int Gstring = 1;
const int Bstring = 2;

const int Dfret = 3;
const int Gfret = 4;
const int Bfret = 5;

//DIGITAL PINS (OUTPUTS)
const int Dspeaker = 5; // w/timer 0, OC0B output (pin 5)
const int Gspeaker = 9; // w/timer 1, OC1A output (pin 9)
const int Bspeaker = 3; // w/timer 2, OC2B output (pin 3)

//loop() variable
char strings_plucked = 0;

void setup()
{
    pinMode(Dspeaker, OUTPUT);
    pinMode(Gspeaker, OUTPUT);
    pinMode(Bspeaker, OUTPUT);

    //setup timers
    TCCR0A = 0b00100011;
    TCCR0B = 0b00001101;

    TCCR1A = 0b10100010; 
    TCCR1B = 0b00011010;
    OCR1A  = 0;

    TCCR2A = 0b00100011;
    TCCR2B = 0b00001111;
    
    Serial.begin(9600);
}

void loop()
{
    strings_plucked = 0x07 & read_strings();
    Serial.println(strings_plucked);
    switch(strings_plucked)
    {
        case 0b000: OCR0B = OCR1A;
                    OCR1A = ICR1;
                    OCR2B = OCR2A;
                    break;

        case 0b001: D();
                    break;

        case 0b010: G();
                    break;

        case 0b100: B();
                    break;

        case 0b011: DG();
                    break;

        case 0b101: DB();
                    break;

        case 0b110: GB();
                    break;

        case 0b111: DGB();
                    break; 

        default:    break;  
    }
}

char read_strings()
{
    char D_string = (analogRead(Dstring) < 695) ? 0b001 : 0b000;
    char G_string = (analogRead(Gstring) < 770) ? 0b010 : 0b000;
    char B_string = (analogRead(Bstring) < 750) ? 0b100 : 0b000;
    return (D_string | G_string | B_string);
}

unsigned short int Dfret_read() //returned value is subtracted from base frequency to adjust to a higher frequency 
{
    if((analogRead(Dfret) > 50) && (analogRead(Dfret) <= 250)) // third fret
    {
        return 5;
    }
    else if(analogRead(Dfret) > 400 && analogRead(Dfret) <= 600) // second fret
    {
        return 3;
    }
    else if(analogRead(Dfret) > 700 && analogRead(Dfret) <= 1023) // first fret
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

unsigned short int Gfret_read() //returned value is subtracted from base frequency to adjust to a higher frequency
{
    if(analogRead(Gfret) > 50 && analogRead(Gfret) <= 250) // third fret
    {
        return 322;
    }
    else if(analogRead(Gfret) > 400 && analogRead(Gfret) <= 600) // second fret
    {
        return 221;
    }
    else if(analogRead(Gfret) > 700 && analogRead(Gfret) <= 1023) // first fret
    {
        return 114;
    }
    else
    {
        return 0;
    }
}

unsigned short int Bfret_read() //returned value is subtracted from base frequency to adjust to a higher frequency 
{
    if((analogRead(Bfret) > 50) && (analogRead(Bfret) <= 250)) // third fret
    {
        return 406;
    }
    else if(analogRead(Bfret) > 400 && analogRead(Bfret) <= 600) // second fret
    {
        return 278;
    }
    else if(analogRead(Bfret) > 700 && analogRead(Bfret) <= 1023) // first fret
    {
        return 143;
    }
    else
    {
        return 0;
    }
}

void D()
{
    OCR0A = Dfreq - Dfret_read();
    OCR0B = OCR0A/2;
}

void G()
{
    ICR1 = Gfreq - Gfret_read();
    OCR1A = ICR1/2;
}

void B()
{
    OCR2A = Bfreq - Bfret_read();
    OCR2B = OCR2A/2;
}

void DG()
{
    //D
    OCR0A = Dfreq - Dfret_read();
    OCR0B = OCR0A/2;

    //G
    ICR1 = Gfreq - Gfret_read();
    OCR1A = ICR1/2;
}

void DB()
{	
    //D
    OCR0A = Dfreq - Dfret_read();
    OCR0B = OCR0A/2;

    //B
    OCR2A = Bfreq - Bfret_read();
    OCR2B = OCR2A/2;
}

void GB()
{
    //G
    ICR1 = Gfreq - Gfret_read();
    OCR1A = ICR1/2;

    //B
    OCR2A = Bfreq - Bfret_read();
    OCR2B = OCR2A/2;
}
void DGB()
{
    //D
    OCR0A = Dfreq - Dfret_read();
    OCR0B = OCR0A/2;

    //G
    ICR1 = Gfreq - Gfret_read();
    OCR1A = ICR1/2;

    //B
    OCR2A = Bfreq - Bfret_read();
    OCR2B = OCR2A/2;
}
