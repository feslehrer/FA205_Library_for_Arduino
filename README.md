# FA205 Bibliothek für Arduino Uno und Nano mit ATmega328P-Controller
### Sonstige FA205-Impementierungen
Implementierungen sind auch für folgende Plattformen verfügbar: 
+ ArduinoUno/Nano mit ATmega328P und ArduinoIDE: https://github.com/feslehrer/FA205_Library_for_Arduino.git
+ ESP32 mit ESP32-Carrier-Board: https://github.com/feslehrer/FA205-ESP32.git

### Implementierung der Technischen Richtlinie FA205 für Arduino.
Bibliotheken für die C-Programmierung für Unterricht und Prüfung im Beruf **Elektroniker für Geräte und Systeme in Baden-Württemberg**.

Diese Bibliotheken sind fast identische Umsetzungen zu meinen FA205-Bibliotheken für MicrochipStudio und können mit der Arduino-IDE verwendet werden.
Vorsicht: Da die Funktionen nativ programmiert sind und nicht auf Arduino-Bibliotheken aufbauen, kann es unter bestimmten Bedingungen zu Konflikten kommen. Die Bibliotheken sind nur für Arduino Uno und Nano-Boards mit ATmega328P-Controller geeignet. Mit wenig Aufwand können sie für ATmega8 und ATmega328PB geändert werden.
Die Bibliotheken können direkt mit dem Arduino-Carrier-Board (AS-Elektronik) zusammen verwendet werden. Das Carrier-Board kann mit Arduino-Nano, Arduino-Uno, Atmega328Pxplained-mini und kompatiblen Controller-Modulen bestückt werden:

![image](https://github.com/feslehrer/FA205_Library_for_Arduino/assets/24614659/97ba22d6-7ddc-4444-a178-783176a95a53)
www.ase-schlierbach.de

<br>Zur Installation der Bibliotheken gehen Sie wie folgt vor:

<br>1. Download der Zip-Bibliothek von GitHub: https://github.com/feslehrer/FA205_Library_for_Arduino
![image](https://github.com/feslehrer/FA205_Library_for_Arduino/assets/24614659/2e762978-edeb-4d41-bab4-f704d60bec41)

<br>2. Anschließend wird in der ArduinoIDE die Lib als Zip-Bibliothek in den „sketchbook\libraries“-Ordner eingebunden:

![image](https://github.com/feslehrer/FA205_Library_for_Arduino/assets/24614659/37570d5f-6274-4827-9876-a8dd5b0b5df8)

<br>3. Im libraries-Prdner erscheint die Lib nun als „FA205_Library_for_Arduino-main“:  

![image](https://github.com/feslehrer/FA205_Library_for_Arduino/assets/24614659/8453072d-3d2e-408e-948c-e26ca7c508c7)

<br>4. Zur Verwendung der Bibliotheken in der Arduino-IDE wird einfach die Header-Datei **controller.h** inkludiert:

![image](https://github.com/feslehrer/FA205_Library_for_Arduino/assets/24614659/c5dee2c5-6652-4448-b0e2-9c47c36852ea)
<br>Warnung: Die Bibliotheken sind nur für ArduinoUno und Nano mit ATmega328P-Controller geeignet, da sie nicht auf die Arduino Funktionen
aufsetzen, sondern hardwarenahe programmiert sind. Das Mischen von Richtlinien- und Arduino-Funktionen wird meistens trotzdem funktionieren, kann aber nicht in jedem Fall garantiert werden.

2022.02.24@Rah
