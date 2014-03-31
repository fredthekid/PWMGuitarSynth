//Note base frequency
#define Efreq 47 // timer 0, prescaled to 15.6Hz
#define Afreq 4545 // timer 1, prescaled to 2MHz
#define Dfreq 27 //timer 2, prescaled to 15.6Hz

//ANALOG PINS (INPUTS)
const int Estring = 0;
const int Astring = 1;
const int Dstring = 2;
const int Efret = 3;
const int Afret = 4;
const int Dfret = 5;

//DIGITAL PINS (OUTPUTS)
const int Espeaker = 5; // w/timer 0, OC0B output (pin 5)
const int Aspeaker = 9; // w/timer 1, OC1A output (pin 9)
const int Dspeaker = 3; // w/timer 2, OC2B output (pin 3)

//loop() variable
char strings_plucked = 0;

void setup()
{
    pinMode(Espeaker, OUTPUT);
    pinMode(Aspeaker, OUTPUT);
    pinMode(Dspeaker, OUTPUT);

    //setup timers
    TCCR0A = 0b00100011;
    TCCR0B = 0b00001101;

    TCCR1A = 0b10100010; 
    TCCR1B = 0b00011010;
    OCR1A  = 0;

    TCCR2A = 0b00100011;
    TCCR2B = 0b00001111;
    
    //for testing
    Serial.begin(9600);
}

void loop()
{
    strings_plucked = 0x07 & read_strings();
    Serial.println(analogRead(Dfret));
    switch(strings_plucked)
    {
        case 0b000: OCR0B = OCR1A;
                    OCR1A = ICR1;
                    OCR2B = OCR2A;
                    break;

        case 0b001: E();
                    break;

        case 0b010: A();
                    break;

        case 0b100: D();
                    break;

        case 0b011: EA();
                    break;

        case 0b101: ED();
                    break;

        case 0b110: AD();
                    break;

        case 0b111: EAD();
                    break; 

        default:    break;  
    }
}

char read_strings()
{
    char E_string = (analogRead(Estring) < 740) ? 0b001 : 0b000;
    char A_string = (analogRead(Astring) < 750) ? 0b010 : 0b000;
    char D_string = (analogRead(Dstring) < 750) ? 0b100 : 0b000;
    return (E_string | A_string | D_string);
}

unsigned short int Efret_read() //returned value is subtracted from base frequency to adjust to a higher frequency 
{
    if((analogRead(Efret) > 50) && (analogRead(Efret) <= 250)) // third fret
    {
        return 7;
    }
    else if(analogRead(Efret) > 400 && analogRead(Efret) <= 600) // second fret
    {
        return 5;
    }
    else if(analogRead(Efret) > 700 && analogRead(Efret) <= 1023) // first fret
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

unsigned short int Afret_read() //returned value is subtracted from base frequency to adjust to a higher frequency
{
    if(analogRead(Afret) > 50 && analogRead(Afret) <= 250) // third fret
    {
        return 723;
    }
    else if(analogRead(Afret) > 400 && analogRead(Afret) <= 600) // second fret
    {
        return 496;
    }
    else if(analogRead(Afret) > 700 && analogRead(Afret) <= 1023) // first fret
    {
        return 255;
    }
    else
    {
        return 0;
    }
}

unsigned short int Dfret_read() //returned value is subtracted from base frequency to adjust to a higher frequency
{
    if(analogRead(Dfret) > 50 && analogRead(Dfret) <= 250) // third fret
    {
        return 5;
    }
    else if(analogRead(Dfret) > 400 && analogRead(Dfret) <= 600) // second fret
    {
        return 4;
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

void E()
{
    OCR0A = Efreq - Efret_read();
    OCR0B = OCR0A/2;
}

void A()
{
    ICR1 = Afreq - Afret_read();
    OCR1A = ICR1/2;
}

void D()
{
    OCR2A = Dfreq - Dfret_read();
    OCR2B = OCR2A/2;
}

void EA()
{
    //E
    OCR0A = Efreq - Efret_read();
    OCR0B = OCR0A/2;

    //A
    ICR1 = Afreq - Afret_read();
    OCR1A = ICR1/2;
}

void ED()
{	
    //E
    OCR0A = Efreq - Efret_read();
    OCR0B = OCR0A/2;

    //D
    OCR2A = Dfreq - Dfret_read();
    OCR2B = OCR2A/2;
}

void AD()
{
    //A
    ICR1 = Afreq - Afret_read();
    OCR1A = ICR1/2;

    //D
    OCR2A = Dfreq - Dfret_read();
    OCR2B = OCR2A/2;
}
void EAD()
{
    //E
    OCR0A = Efreq - Efret_read();
    OCR0B = OCR0A/2;

    //A
    ICR1 = Afreq - Afret_read();
    OCR1A = ICR1/2;

    //D
    OCR2A = Dfreq - Dfret_read();
    OCR2B = OCR2A/2;
}
