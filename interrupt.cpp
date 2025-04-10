// Technische Richtlinie FA205
// Bibliothek:       interrupt.c
// Controller:       Atmel ATmega8 / ATmega328p
//     Verwendet externen Interrupt 0 und Timer 0
// Version:          2.0
// erstellt am:      13.07.2015
// letzte Änderung:  11.4.2025
// Autor:            Rahm

#include "interrupt.h"
//#include "math.h"

#define _INT_CONFIG_     EICRA
#define _INT_ENABLE_     EIMSK

// Lokale initialisierungsroutine
uint8_t preload_calc    ( float time);

static uint16_t prescaler;

// Pointer auf Interrupt-Service-Routinen
void (*my_interrupt0) (void);
void (*my_interrupt1) (void);
void (*my_datareceived)  (void);
void (*my_timer)      (void);

//externer Interrupt 0
void ext_interrupt_init( void (*ip) (void) )
{
  my_interrupt0 = ip;
	#ifdef _FALLING_EDGE_TRIGGER_
   _INT_CONFIG_ |=  (1<<ISC01);
   _INT_CONFIG_ &= ~(1<<ISC00);    //auf fallende Flanke triggern
	#else
	 #ifdef _RISING_EDGE_TRIGGER_
	  _INT_CONFIG_ |= (1<<ISC01);
    _INT_CONFIG_ |= (1<<ISC00);   //auf steigende Flanke triggern
   #else 
	  _INT_CONFIG_ &= ~(1<<ISC01);
    _INT_CONFIG_ |=  (1<<ISC00);   //auf beliebige Flanke triggern
	 #endif
	#endif
}

ISR(INT0_vect)                    // Interrupt-Vektor
{
  _INT_ENABLE_ &= ~(1<<INT0);     // ext. Interrupt deaktivieren
  my_interrupt0();                 // Aufruf der Benutzer-ISR
  _INT_ENABLE_ |= (1<<INT0);      // ext. Interrupt aktivieren
}

void ext_interrupt_enable( void )
{
  _INT_ENABLE_ |= (1<<INT0);      // ext. Interrupt aktivieren
  sei();                          // Globale Interruptfreigabe
}

void ext_interrupt_disable( void )
{
  _INT_ENABLE_ &= ~(1<<INT0);     // ext. Interrupt deaktivieren
  cli();                          // Globale Interruptsperre
}

// Externer Interrupt 1
void ext_interrupt1_init( void (*ip) (void) )
{
	my_interrupt1 = ip;
	#ifdef _FALLING_EDGE_TRIGGER_
	 _INT_CONFIG_ |=  (1<<ISC11);
	 _INT_CONFIG_ &= ~(1<<ISC10);    //auf fallende Flanke triggern
	#else
	 #ifdef _RISING_EDGE_TRIGGER_
	  _INT_CONFIG_ |= (1<<ISC11);
	  _INT_CONFIG_ |= (1<<ISC10);   //auf steigende Flanke triggern
	 #else
	  _INT_CONFIG_ &= ~(1<<ISC11);
	  _INT_CONFIG_ |=  (1<<ISC10);   //auf beliebige Flanke triggern
	 #endif
	#endif
}
ISR(INT1_vect)                    // Interrupt-Vektor
{
	_INT_ENABLE_ &= ~(1<<INT1);     // ext. Interrupt deaktivieren
	my_interrupt1();                 // Aufruf der Benutzer-ISR
	_INT_ENABLE_ |= (1<<INT1);      // ext. Interrupt aktivieren
}

void ext_interrupt1_enable( void )
{
	_INT_ENABLE_ |= (1<<INT1);      // ext. Interrupt aktivieren
	sei();                          // Globale Interruptfreigabe
}

void ext_interrupt1_disable( void )
{
	_INT_ENABLE_ &= ~(1<<INT1);     // ext. Interrupt deaktivieren
	//cli();                          // Globale Interruptsperre
}

//*******************************************************************
//** ab hier timer-Funktionen
//*******************************************************************

void timer1ms_init ( void (*ti) (void))
{
    timer_ms_init(ti,1.0);
}

void timer1ms_enable( void )
{  
  timer_ms_enable();
}

void timer1ms_disable( void )
{
  timer_ms_disable();
}

// Die timer-Interrupt-Serviceroutine timer1ms_isr() muss
// selbst geschrieben werden!!

ISR(TIMER1_COMPA_vect)        // Timer1-Interrupt im Autoreload-mode (CTC)
{ 
  TIMSK1 &= ~(1 << OCIE1A);      // Timer Interrupt sperren
  my_timer();                    // aufrufen der Interrupt-Serviceroutine
  TIMSK1 |=  (1 << OCIE1A);      // Timer Interrupt aktivieren
}

void timer_ms_init ( void (*ti) (void), float time)
{
  cli();                    // Interrupts deaktivieren

  my_timer = ti;					  // Pointer auf isr im User-Code (normalerweise: timer_ms_isr) !!
 
  TCCR1A = 0;               // Timer1 Control Register A auf 0 setzen
  TCCR1B = 0;               // Timer1 Control Register B auf 0 setzen
  TCNT1  = 0;               // Timer1 Counter auf 0 setzen

  OCR1A = preload_calc(time);   //  (F_CPU / 1000) - 1;  //1ms
  TCCR1B |= (1 << WGM12);     // CTC-Modus aktivieren
  //TCCR1B |= (1 << CS10);    // Keine Prescaler (16MHz)

  //sei();             // Interrupts aktivieren (!!erst mit timer_ms_enable())
}

uint8_t preload_calc ( float time)
{ // time in ms
  uint8_t preload;
  
  if      (time < 0.015937) prescaler = 1;         // tmax = 15,937µs
  else if (time < 0.1275  ) prescaler = 8;
  else if (time < 1.02    ) prescaler = 64;
  else if (time < 4.08    ) prescaler = 256;
  else                      prescaler = 1024;      // tmax = 16,32ms
  
  preload = (uint8_t) ((float)(F_CPU/1000/prescaler) * time - 1.0);

  return preload;
}

void timer_ms_enable( void )
{
  switch (prescaler)
  {
    case 1:     TCCR1B    |= (1 << CS10); break;               // Timer starten mit Vorteiler 1
    case 8:     TCCR1B    |= (1 << CS11); break;               // Timer starten mit Vorteiler 8
    case 64:    TCCR1B    |= (1 << CS11) | (1 << CS10); break; // Timer starten mit Vorteiler 64
    case 256:   TCCR1B    |= (1 << CS12); break;               // Timer starten mit Vorteiler 256
    case 1024:  TCCR1B    |= (1 << CS12) | (1 << CS10); break; // Timer starten mit Vorteiler 1024    
  }

  TIMSK1 |= (1 << OCIE1A);      // Timer Compare Interrupt aktivieren
  TCCR1B |= (1 << WGM12);       // CTC-Modus aktivieren
  //TCCR1B |= (1 << CS10);        // Keine Prescaler (16MHz)
  sei();                                       // Globale Interruptfreigabe
}

void timer_ms_disable( void )
{
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));  //Timer stoppen
  TIMSK1 &= ~(1 << OCIE1A);       // Compare Match A Interrupt für Timer1 
  //cli();                                     // Globale Interruptsperre
}

//*********** Data Receive Interrupt *******************************

void serial_interrupt_init( void (*sr) (void))
{ 
	rs232_init();  // Serielle Schnittstelle initialisieren
	my_datareceived = sr;
}

ISR(USART_RX_vect)                  // Interrupt-Vektor
{
	//UCSR0B &= ~(1<<RXCIE0);         // Empfangsinterrupt ausschalten
	my_datareceived();                // Aufruf der Benutzer-ISR
  //UCSR0B |= (1<<RXCIE0);          // Empfangsinterrupt einschalten
}

extern void serial_interrupt_enable  ( void )
{
  UCSR0B |= (1<<RXCIE0);            // Empfangsinterrupt einschalten
	sei();
}

extern void serial_interrupt_disable ( void )
{
	UCSR0B &= ~(1<<RXCIE0);           // Empfangsinterrupt ausschalten
}

//  
// Soundausgabe auf Lautsprecher an Port B.3
// 
// Problem: Tonhöhe nicht variabel. In ATmelStudio funktioniert alles. 
//  Evtl.: timer_ms verwendet Timer0. Dieser wird auch von der Arduino-Funktion millis() verwendet.
//         Lösung z.B. timer1 verwenden? ist aber 8 Bit??
//            !!! not yet implemented !!!
#ifndef TON_PIN
  #define TON_PIN _PORTB_,3
#endif


void sound_init(void)
{
    bit_init(TON_PIN,OUT);
}

void note_isr( void )
{  
  bit_write(TON_PIN,~bit_read(TON_PIN));
}

void note_on(float frequenz)
{
  float millisec;
  
  millisec = 500/frequenz;    // Zeit f�r Halbe Periodendauer in ms
  
  timer_ms_init(note_isr, millisec);
  timer_ms_enable();
}

void note_off(void)
{
  timer_ms_disable();
}

void play_note(uint8_t notenwert, uint16_t duration, uint16_t silence)
{
  float exponent = ((float)notenwert - 69.0) / 12.0;
  float frequenz = 440 * pow(2,exponent);

  note_on(frequenz);
  delay_ms(duration);
  note_off();
  delay_ms(silence);
}