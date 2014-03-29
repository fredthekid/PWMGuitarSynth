//ANALOG PINS
const int Estring = 0;
const int Astring = 1;
const int Dstring = 2;
const int Efret = 3;
const int Afret = 4;
const int Dfret = 5;

//DIGITAL PINS
const int Espeaker = 3;
const int Aspeaker = 4;
const int Dspeaker = 5;

//USED FOR CONTROLLING FREQUENCY (delays in microseconds)
const int Edelay = 6250;
const int Adelay = 4545;
const int Ddelay = 3400;

//
char strings_plucked = 0;

void setup()
{
    pinMode(Espeaker, OUTPUT);
    pinMode(Aspeaker, OUTPUT);
    pinMode(Dspeaker, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    strings_plucked = 0x07 & read_strings();
    Serial.println(strings_plucked,BIN);
    switch(strings_plucked)
    {
        case 0b000: break;

        case 0b001: E(); 
                    break;

        case 0b010: A();
                    break;

        case 0b100: D();
                    break;

//        case 0b011: EA();
//                    break;
//
//        case 0b101: ED();
//                    break;
//
//        case 0b110: AD();
//                    break;
//
//        case 0b111: EAD();
//                    break; 
        default: break;
       }
}

void E()
{
    int totalEdelay = Edelay;
    int onTime = 0;
    for(int i = 1; i < 1000; i+=5)
    {
        onTime = totalEdelay/(2*i);
        digitalWrite(Espeaker, HIGH);
        delayMicroseconds(onTime);
        digitalWrite(Espeaker, LOW);
        delayMicroseconds(totalEdelay-onTime);
        if(analogRead(Estring) < 750)
        {
            i = 1;
        }
    }

    digitalWrite(Espeaker, LOW);
}

void D()
{
    int totalDdelay = Ddelay;
    int onTime = 0;
    for(int i = 1; i < 1000; i+=5)
    {
        onTime = totalDdelay/(2*i);
        digitalWrite(Dspeaker, HIGH);
        delayMicroseconds(onTime);
        digitalWrite(Dspeaker, LOW);
        delayMicroseconds(totalDdelay-onTime);
        if(analogRead(Dstring) < 750)
        {
	  i = 1;
        }
    }
	
    digitalWrite(Dspeaker, LOW);
}

void A()
{
    int totalAdelay = Adelay;
    int onTime = 0;
    for(int i = 1; i < 1000; i+=5)
    {
        onTime = totalAdelay/(2*i);
        digitalWrite(Aspeaker, HIGH);
        delayMicroseconds(onTime);
        digitalWrite(Aspeaker, LOW);
        delayMicroseconds(totalAdelay-onTime);
        if(analogRead(Astring) < 750)
        {
			i = 1;
        }
    }
	
    digitalWrite(Aspeaker, LOW);
}

char read_strings()
{
    unsigned int E_string = (analogRead(Estring) < 750) ? 0b001 : 0b000;
    unsigned int A_string = (analogRead(Astring) < 750) ? 0b010 : 0b000;
    unsigned int D_string = (analogRead(Dstring) < 750) ? 0b100 : 0b000;

    return (E_string | A_string | D_string);
}
