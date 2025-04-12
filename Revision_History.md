FA205-Biblitheken für MicrochipStudio und Arduino-Carrier-Board mit ATmega328P Xplained mini<br>
Revision-History
**************************

2025.04.13:<br>
+ Bugfixes in rs232_setbaud(). Funktioniert jetzt für die gängigen Baudraten und lässt ein Ändern während der Programmlaufzeit zu.
+ Support für ATmega8 wurde entfernt.

2025.04.11:<br>
+ timer1ms mit Timer1 anstelle von Timer0<br>
  Vorteil: die Arduino-Funktion millis() kann nun parallel verwendet werden.<br>
+ Timer Bug gefixt<br>
+ lcd_print() und rs232_print() Warnungen wegen unerlaubter Datentypkonvertierung gefixt
