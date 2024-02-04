#include <avr/io.h>
#include "USART.h"
#include <util/delay.h>

int main(){
  
  //PIN 47
  //PL2 (T5)

  //PIN Zu einem Eingang
  DDRL &= ~(0x01 << 2);
  //internen Pullup aktivieren
  PORTL |= (0x01 << 2);

  //setzen der 3 Bits CSn[2..0] -> alles auf 1 heißt steigende Flanke
  TCCR5B |= (0x01 << 2);
  TCCR5B |= (0x01 << 1);
  TCCR5B |= (0x01 << 0);


  USARTInit(0,9600,1,0, 1,0);
  printf("Program starting...\n");
 
  while(1)
  {
    //counter variable
    uint16_t counter;
    //Aktueller Zählerstand ist im Register TCNTn gespeichert
    counter = TCNT5;

    printf("TCNT5=%u\r\n", counter);
   _delay_ms(100); 

  }
}
