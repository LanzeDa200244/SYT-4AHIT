#include <avr/io.h>
#include <util/delay.h>

#include "USART.h"

int main(){

  USARTInit(0,9600,1,0, 1,0);

  printf("Program starting...\n");

  int i = 0;

  while(1)
  {
    printf("Zählerstand=%d\n",i);
    i++;
    _delay_ms(1000);
  }
}


