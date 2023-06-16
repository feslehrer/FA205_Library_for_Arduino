/* 
  Beschreibung:     Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                    Vorschlag fuer Bit- und Byte-IO
                   
  Autor:            Rolf Rahm
  Datum:            26.05.2015
  Letzte Änderung:  07.05.2023
  Hinweise:					Erstellt mit Arduino-Carrier-Board
*/
#include "controller.h"

#define	LED             _PORTD_        // 
#define LED1            _PORTD_,1
#define LED2            _PORTD_,2
#define	TasterS1        _PORTB_,2      // 
#define TasterS2        _PORTB_,3      //

void setup (void)
{
	/* Initialisierungen */
  bit_init(LED1,OUT);             
  bit_init(LED2,OUT);            // Initialisierung mit 2 Parametern
  bit_init(LED,3,OUT);           //        "        mit 3 Parametern
  
  bit_init(TasterS1,IN);         // PortB.2 = Taster S1
  bit_init(TasterS2,IN);         // PortB.3 = Taster S2
}

// Funktion loop()
void loop(void)
{
  uint8_t temp;

  temp = bit_read(TasterS1);		
	bit_write(LED,3,temp);         // drei Parameter !!!

	temp = bit_read(TasterS2);
  bit_write(LED1,temp);	
	temp = ~temp;
  bit_write(LED2,temp);
}
