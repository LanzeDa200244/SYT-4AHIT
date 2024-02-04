
#include <avr/io.h>
#include "USART.h"
#include <avr/interrupt.h>
#include "SystemClock.h"

int status;
volatile uint32_t startTime;
volatile uint32_t endTime;

int main(){

  USARTInit(0,9600,1,0, 1,0);
  printf("Program starting...\n");

  initTimer0AsSystemClock(0);

  startTime = 0;
  endTime = 0;

  DDRE |= (0x01<<5); // PE5/INT5 wird ein Ausgang
  PORTE |= (0x01<<5); // Ausgabe HighPegel

  //configure Interrupts (Any Edge)
  EICRB |= (0x01 << ISC50);
  EICRB &= ~(0x01 << ISC51);

  //Interrupt-Enable-Flag für Externen Interrupt Nr.5 auf 1 setzen
  EIMSK |= (0x01 << 5);
  //erlauben von globalen Interrupt
  sei();


  
  while(1)
  {
    //Vor der Messung ist die Variable für den Status 
    status = 0;
   
    // Messungs wird gestartet

    //PE5 zum Ausgang machen
    DDRE |= (0x01 << 5);
    delayMicroseconds(20);

    //High-Pegel am PE5 ausgeben
    PORTE |= (0x01 << 5);
    delayMicroseconds(20);

    //Low-Pegel am PE5 ausgeben
    PORTE &= ~(0x01 << 5);
    //delayMicroseconds(20);

    //PE5 zu Eingang umschreiben
    DDRE &= ~(0x01 << 5);
    //delayMicroseconds(20);

    
    status = 1;

    uint32_t diff = endTime - startTime;

    // Zeit Differenz von Microsekunden zu Sekunden konvertieren
    // 1 Sekunde = 1000000 Mikrosekunden
    float timeDiffSeconds = (float)diff / 1000000.0;

    // Distanz in CM berechnen
    //Zeitdifferenz * Schallgeschwindigkeit, * 100 für m in cm, /2 weil Weg hin und zurück und brauchen nur Hinweg
    int distance = (int)(timeDiffSeconds * 340.0 * 100.0) / 2.0;
    
    printf("gemessene Distanz: %d cm\n", distance);
    delayMilliseconds(100);
      
  }
}


ISR(INT5_vect) {
  if (status == 1) {
    startTime = micros();
    status = 2;
  } else if (status == 2) {
    endTime = micros();
    status = 3;
  }
}
