#include <mbed.h>

enum States{DOWN, UP, FALL, MIN, RISE, MAX};

InterruptIn button(p10);
DigitalOut blue_led(p5);
DigitalOut orange_led(p6);
PwmOut intensidade(p7);
Timeout press_time;
Timeout led_timeout;


States current = UP;
bool isPress;
float frequencia;
float LED_intensidade;



void btn_press();
void btn_release();
void change_states();
void blink();
void Clear();
string name_state();


int main()
{
    Clear();
    button.rise(&btn_press);
    button.fall(&btn_release);
  while(1)
  {
    intensidade = LED_intensidade;
    printf("%s \n", name_state());
    printf("Intensidade: %.2f \n", LED_intensidade);
    wait_ms(100);
  }

  return 0;
}


void Clear()
{
    blue_led = 1;
    orange_led = 0;
    LED_intensidade = 0.5;
}

void btn_press()
{
    isPress = true;
    press_time.attach(&change_states, 1.0);
}

void btn_release()
{
    isPress = false;
        switch (current) {
        case DOWN:
        case RISE:
        case MIN:
            if (LED_intensidade < 1) {
                blue_led = 1;
                red_led = 0;
                current = UP;
            }
            break;
        case UP:
        case FALL:
        case MAX:
            if (LED_intensidade > 0) {
                blue_led = 0;
                orange_Led = 1;
                current = DOWN;
            }
            break;
    }
}

void change_states()
{
    press_time.attach(&change_states, 1.0);
    led_timeout.attach(&blink, 2.0);

    if(isPress)
    {
        switch (current)
        {
        case RISE:
            if (LED_intensidade == 1)
            { 
                current = MAX;
            }
            else
            {
                LED_intensidade += 0.05;
            }
            break;
        case FALL:
            if (LED_intensidade == 0)
            { 
                current = MIN;
            }
            else
            {
                LED_intensidade -= 0.05;
            }
            break;
        case UP:
            current = RISE;
            LED_intensidade += 0.05;
            break;
        case DOWN:
            current = FALL;
            LED_intensidade -= 0.5;
            break;
        default:
            break;
        }

        if(LED_intensidade >= 1)
        {
            current = MAX;
            intensidade = 1;
        }else if(intensidade <= 0)
        {
            current = MIN;
            intensidade = 0;
        }
    }
}

void blink()
{
    if(current == MIN || current == FALL)
    {
        orange_led = !orange_led;
    }else if(current == MAX || current == RISE)
    {
        blue_led = !blue_led;
    }

      if(current == MIN || current == MAX)
    {
        frequencia = 0.1;

    }else if(current == RISE)
    {
        frequencia = 0.2 *  (1 + LED_intensidade / 0.05);

    }else if(current == FALL)
    {
        frequencia = (0.2 * (LED_intensidade / 0.05 - 1));
    }

    led_timeout(&blink, frequencia);
}

string name_state()
{
    switch (current)
    {
    case UP:
        return "ESTADO UP";
    case DOWN:
        return "ESTADO DOW";      
    case MAX:
        return "ESTADO MAX";
    case MIN:
        return "ESTADO MIN";
    case RISE:
        return "ESTADO RISE";
    case FALL:
        return "ESTADO FALL";
    }
}
