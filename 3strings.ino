//Note base frequency
#define Gfreq 20 // timer 0, prescaled to 15.6Hz
#define Bfreq 2025 // timer 1, prescaled to 2MHz
#define Efreq 12 //timer 2, prescaled to 15.6Hz

//ANALOG PINS (INPUTS)
const int Gstring = 0;
const int Bstring = 1;
const int Estring = 2;
const int Gfret = 3;
const int Bfret = 4;
const int Efret = 5;

//DIGITAL PINS (OUTPUTS)
const int Gspeaker = 5; // w/timer 0, OC0B output (pin 5)
const int Bspeaker = 9; // w/timer 1, OC1A output (pin 9)
const int Espeaker = 3; // w/timer 2, OC2B output (pin 3)

//loop() variable
char strings_plucked = 0;

void setup()
{
    pinMode(Gspeaker, OUTPUT);
    pinMode(Bspeaker, OUTPUT);
    pinMode(Espeaker, OUTPUT);

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
    Serial.println(strings_plucked,BIN);
    switch(strings_plucked)
    {
        case 0b000: OCR0B = OCR1A;
                    OCR1A = ICR1;
                    OCR2B = OCR2A;
                    break;

        case 0b001: G();
                    break;

        case 0b010: B();
                    break;

        case 0b100: E();
                    break;

        case 0b011: GB();
                    break;

        case 0b101: GE();
                    break;

        case 0b110: BE();
                    break;

        case 0b111: GBE();
                    break; 

        default:    break;  
    }
}

char read_strings()
{
    char G_string = (analogRead(Gstring) < 695) ? 0b001 : 0b000;
    char B_string = (analogRead(Bstring) < 770) ? 0b010 : 0b000;
    char E_string = (analogRead(Estring) < 750) ? 0b100 : 0b000;
    return (G_string | B_string | E_string);
}

unsigned short int Gfret_read() //returned value is subtracted from base frequency to adjust to a higher frequency 
{
    if((analogRead(Gfret) > 50) && (analogRead(Gfret) <= 250)) // third fret
    {
        return 3;
    }
    else if(analogRead(Gfret) > 400 && analogRead(Gfret) <= 600) // second fret
    {
        return 2;
    }
    else if(analogRead(Gfret) > 700 && analogRead(Gfret) <= 1023) // first fret
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned short int Bfret_read() //returned value is subtracted from base frequency to adjust to a higher frequency
{
    if(analogRead(Bfret) > 50 && analogRead(Bfret) <= 250) // third fret
    {
        return 322;
    }
    else if(analogRead(Bfret) > 400 && analogRead(Bfret) <= 600) // second fret
    {
        return 221;
    }
    else if(analogRead(Bfret) > 700 && analogRead(Bfret) <= 1023) // first fret
    {
        return 114;
    }
    else
    {
        return 0;
    }
}

unsigned short int Efret_read() //returned value is subtracted from base frequency to adjust to a higher frequency 
{
    if((analogRead(Efret) > 50) && (analogRead(Efret) <= 250)) // third fret
    {
        return 3;
    }
    else if(analogRead(Efret) > 400 && analogRead(Efret) <= 600) // second fret
    {
        return 2;
    }
    else if(analogRead(Efret) > 700 && analogRead(Efret) <= 1023) // first fret
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void G()
{
    OCR0A = Gfreq - Gfret_read();
    OCR0B = OCR0A/2;
}

void B()
{
    ICR1 = Bfreq - Bfret_read();
    OCR1A = ICR1/2;
}

void E()
{
    OCR2A = Efreq - Efret_read();
    OCR2B = OCR2A/2;
}

void GB()
{
    //G
    OCR0A = Gfreq - Gfret_read();
    OCR0B = OCR0A/2;

    //B
    ICR1 = Bfreq - Bfret_read();
    OCR1A = ICR1/2;
}

void GE()
{	
    //G
    OCR0A = Gfreq - Gfret_read();
    OCR0B = OCR0A/2;

    //E
    OCR2A = Efreq - Efret_read();
    OCR2B = OCR2A/2;
}

void BE()
{
    //B
    ICR1 = Bfreq - Bfret_read();
    OCR1A = ICR1/2;

    //E
    OCR2A = Efreq - Efret_read();
    OCR2B = OCR2A/2;
}
void GBE()
{
    //G
    OCR0A = Gfreq - Gfret_read();
    OCR0B = OCR0A/2;

    //B
    ICR1 = Bfreq - Bfret_read();
    OCR1A = ICR1/2;

    //E
    OCR2A = Efreq - Efret_read();
    OCR2B = OCR2A/2;
}
