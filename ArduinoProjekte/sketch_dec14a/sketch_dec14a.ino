
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include <avr/interrupt.h>

volatile uint16_t counter = 0;

int main(){

  USARTInit(0,9600,1,0, 1,0);
  printf("Program starting...\n");

  DDRE &= ~(0x01<<5); // PE5/INT5 wird ein Eingang
  PORTE |= (0x01<<5); //internen pullup-Widerstand aktivieren

  // Konfiguration für INT5: steigende Flanke
  EICRB |= (1 << ISC50);
  EICRB |= (1 << ISC51);

  //Interrupt-Enable-Flag für Externen Interrupt Nr.5 auf 1 setzen
  EIMSK |= (0x01<<5); 

  //erlauben von globalen Interrupt
  sei();

  while(1)
  {
    
    _delay_ms(100);
    
  }
}

//Interrupt-Service-Routine for externen Interrupt 5
ISR(INT5_vect)
{
  counter++;
  printf("counter=%d\r\n", counter);
}
