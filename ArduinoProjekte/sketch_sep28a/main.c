#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

/*
  Bsp 3.c) internen Pullup Widerstand verwendne

  Pin PF3 zu eingang machen --> Bit 3 im SFR DDRF löschen (zu 0 machen)
    DDRF &= ~(0x01<<3)

  Bei einem Eingang:
    Bit 3 im SFR PORTF aktiviert bzw. deaktiviert den internen Pullup Widerstand

    Bit 3 im PortF setzen --> interen pullup Widerstand aktivieren --> PORTF |= (0x01<<3)

  Abfragen, ob mittels Taster ein High oder Low Pegel an den Eingang gelegt worden ist:

    if( PINF & (0x01<<3) ){

      Es Liegt ein High Pegel an
    }
    else{
        Es liegt ein Low Pegel an
    }
*/


int main(){

  USARTInit(0,9600,1,0, 1,0);

  printf("Program starting...\n");

  DDRF &= ~(0x01<<3);

  PORTF |= (0x01<<3);

  while(1)
  {
     if( PINF & (0x01<<3) ){

      printf("Es lieg ein High Pegel an\n");
    }
    else{
        printf("Es lieg ein Low Pegel an\n");
    }
    _delay_ms(1000);
  }
}


