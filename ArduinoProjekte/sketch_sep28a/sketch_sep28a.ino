
//GPIO Pin PF3 zu einem Ausgang machen

/*
Algorithmus

  Bit Nr 3 in SFR PORTF bestimmt, ob man einen High oder Low Pegel ausgibt
  (Bit setzen : 1) -> High Pegel
  (Bit löschen : 0) -> Low Pegel

  In der Initialisierung: PF3 zu einem Ausgang machen
  In der Endlosschleife:
    -High Pegel an PF3 ausgeben
    -wartenc:\Users\dlanz\OneDrive\Desktop\ArduinoProjekte\sketch_sep28a\main.c
    -Low Pegel an PF3 ausgeben
    -warten
*/

#include <util/delay.h>
void setup() {

  //Bit Nr 3 im Special Function Register DDRF setzen (mit 1 beschreiben)
  DDRF |= (0x01<<3);

}


void loop() {

//LED einschalten bei einem High Pegel
  /*PORTF |= (0x01<<3);
  _delay_ms(1000);
  PORTF &= ~(0x01<<3);
  _delay_ms(500);*/

//LED einschalten bei einem Low Pegel
  PORTF &= ~(0x01<<3);
  _delay_ms(1000);
  PORTF |= (0x01<<3);
  _delay_ms(500);

}


