#include <avr/io.h>
#include <util/delay.h>

#include "USART.h"
int main(){

  // Taster mit internen PUllUP Widerstand

  USARTInit(0,9600,1,0, 1,0);

  printf("Program starting...\n");

  DDRF = 0x38; 
  PORTF |= (0x01<<2); //PIN PF3 internen Pullupwiderstand aktivieren

  while(1)
  {
    //shiften, heißt den 3er Pin dann zum 0er und dann mit 0x01 per & verknüpfen -> alle sind 0 außer das 0er Pin 
    // == -> heißt gleich 0 -> bei PULLUP ist bei gedrücktem Taster liegt eine 0 an
    if( ((PINF >> 2) & 0x01 ) == 0x00)
    {
        printf("Taster gedrueckt\n");
        PORTF = 0x38;
        _delay_ms(500);
    }
    else{
        printf("Taster nicht gedrueckt\n");
        _delay_ms(500);
    }
  }
}