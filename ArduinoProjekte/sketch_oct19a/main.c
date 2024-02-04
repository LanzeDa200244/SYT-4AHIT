#include <avr/io.h>
#include <util/delay.h>

#include "USART.h"
int main(){

  // TAster mit Externen PULLDown Widerstand
  USARTInit(0,9600,1,0, 1,0);

  printf("Program starting...\n");

  DDRF &= ~(0x01<<3); //PIN PF3 zu einem Eingang machen -> Bit löschen 0 heißt Eingang
  PORTF &= ~(0x01<<3); //PIN PF3 internen Pullupwiderstand deaktivieren

  while(1)
  {
    //shiften, heißt den 3er Pin dann zum 0er und dann mit 0x01 per & verknüpfen -> alle sind 0 außer das 0er Pin 
    // != -> heißt ungleich 0 (bei PULLDOWN ließt er bei gedrücktem Taster eine 1)
    if( ((PINF >> 3) & 0x01 ) != 0x00)
    {
        printf("Taster gedrueckt\n");
    }
    else{
        printf("Taster nicht gedrueckt\n");
        _delay_ms(500);
    }
  }
}