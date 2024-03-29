/*
 Beschreibung:  Countdouwn-Zähler. Startet mit Taster S1
                läuft runter von 9 bis 0, dann ertönt ein
                Signalton.  
   
 Erstellt am: 
 Letzte Änderung:
 
 Autor: Rah
*/

#include "controller.h"

#define	SiebenSeg    _PORTD_
#define Taster_S1    _PORTB_,2
#define Speaker      _PORTB_,3

 uint8_t seg7code[] = { 0xc0,0xf9,0xa4,0xb0,
                        0x99,0x92,0x82,0xf8,
                        0x80,0x90 };

void setup (void)
{  /* Initialisierungen */
  byte_init(SiebenSeg,OUT);
  byte_write(SiebenSeg,0xff);     // Anzeige dunkelschalten
  byte_write(SiebenSeg,seg7code[0]); //Startwert 0
  
  bit_init(Taster_S1,IN);
  bit_init(Speaker,OUT);
}

uint8_t  beep = 0x01;
  
void loop(void)
{
  int8_t   n;
  uint16_t i;

  while(bit_read(Taster_S1)==1);   //Warten bis Taster betätigt
  for(n=9;n>=0;n--)
  {
    delay_ms(1000);
    byte_write(SiebenSeg,seg7code[n]);
  }
  
  for(i=0; i<2000; i++)
  {
    bit_write(Speaker,beep);
    delay_100us(5);
    beep=~beep;
  }
}
