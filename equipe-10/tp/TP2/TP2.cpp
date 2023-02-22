#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#define debounce_time 20
#define DelaiVertAllume 2000
#define Vert 0x01
#define Off 0x00

enum class State
{
    FirstPress,
    FirstRelease,
    SecondPress,
    SecondRelease,
    ThirdPress,
    ThirdRelease,
    On,
};
bool VerifAppuye()
{
    if (PIND & (1 << PD2))
    {
        _delay_ms(debounce_time);
        if (PIND & (1 << PD2))
        {
            return true;
        }
    }
    return false;
}

void init()
{
    DDRA |= (1 << PA0) | (1 << PA1); // force les bits a 1
    DDRD &= ~(1 << PD2);             // force les bits a 0
}
int main()
{
    State state = State::FirstPress;
    init();
    while (true)
    {
        switch (state)
        {

        case State::FirstPress:
            if (VerifAppuye())
            {
                  
                  
                state = State::FirstRelease;
            }
            break;
        case State::FirstRelease:
            if (VerifAppuye() == false)
            {
                state = State::SecondRelease;
            }
            break;
        case State::SecondPress:
            if (VerifAppuye())
            {
                state = State::SecondRelease;
            }
            break;
        case State::SecondRelease:
            if (VerifAppuye() == false)
            {
                state = State::ThirdPress;
            }
            break;
        case State::ThirdPress:
            if (VerifAppuye())
            {
                state = State::ThirdRelease;
            }
            break;
        case State::ThirdRelease:
            if (VerifAppuye()==false)
            {
                state = State::On;
            }
            break;

        case State::On:
            PORTA = Vert;
            _delay_ms(DelaiVertAllume);
            PORTA = Off;
            state = State::FirstPress;
            break;
        }
    }
}

// // #define F_CPU 16000000UL
// // #include <util/delay.h>
// // #include <avr/io.h>
// // #define debounce_time 20
// // #define DelaiVertAllume 2000

// // enum class State
// {
//     off,
//     FirstPress,
//     FirstRelease,
//     SecondPress,
//     SecondRelease,
//     ThirdPress,
//     On,
// };
// bool VerifAppuye()
// {
//     if (PIND & (1 << PD2))
//     {
//         _delay_ms(debounce_time);
//         if (PIND & (1 << PD2))
//         {
//             while (PIND & (1 << PD2))
//             {
//                 // attendre le relâchement du bouton
//             }
//             return true;
//         }
//     }
//     return false;
// }

// void init()
// {
//     DDRA |= (1 << PA0) | (1 << PA1) ;  // force les bits a 1
//     DDRD &= ~(1 << PD2); // force les bits a 0
// }
// int main()
// {
//     int press_counter = 0;
//     State state = State::off;
//     init();
//     while (true)
//     {
//         switch (state)
//         {
//         case State::off:
//             if (VerifAppuye())
//             {
//                 press_counter++;
//                 state = State::off;
//             }
//             break;

//         case State::off1:
//             if (VerifAppuye())
//             {
//                 press_counter++;
//                 if (press_counter == 2)
//                 {

//                     state = State::on;
//                 }
//                 else
//                 {
//                     state = State::off1;
//                 }
//                 break;
//             }
//             else
//             {
//                 press_counter = 0;
//                 break;
//             }

//         case State::on:
//             PORTA = 0x01;
//             _delay_ms(DelaiVertAllume);
//             PORTA = 0x00;
//             state = State::off;
//             press_counter = 0;
//             break;
//         }
//     }
// }
