#define Efreq 1517 //timer 2, prescaled to 15.6Hz

//ANALOG PINS (INPUTS)
const int Estring = 2;
const int Efret = 5;

//DIGITAL PINS (OUTPUTS)
const int Espeaker = 9; // w/timer 1, OC1A output (pin 9)


void setup()
{
    pinMode(Espeaker, OUTPUT);

    TCCR1A = 0b10100010; 
    TCCR1B = 0b00011010;
    OCR1A  = 0;
    Serial.begin(9600);
}

void loop()
{
  Serial.println(analogRead(Estring));
    if((analogRead(Estring) < 680))
    {
          ICR1 = Efreq - Efret_read();
          OCR1A = ICR1/2;
    }
    
    else
    {
		OCR1A = ICR1;
	}
}
unsigned short int Efret_read() //returned value is subtracted from base frequency to adjust to a higher frequency 
{
    if((analogRead(Efret) > 50) && (analogRead(Efret) <= 250)) // third fret
    {
        return 241;
    }
    else if(analogRead(Efret) > 400 && analogRead(Efret) <= 600) // second fret
    {
        return 165;
    }
    else if(analogRead(Efret) > 700 && analogRead(Efret) <= 1023) // first fret
    {
        return 85;
    }
    else
    {
        return 0;
    }
}
