# OpenLapTimer

###Obiettivo
L’obiettivo è quello di creare un cronometro GPS open source & open hardware per misurare i tempi sul giro nell’ambito motorsport.

Il dispositivo dovrà visualizzare in ogni istante il tempo corrente con la precisione dei decimi di secondo.

![Mockup in funzione](http://tangodev.url.ph/openlaptimer/mockup-in-funzione.png)

Al termine di ogni giro dovrà essere presentato il tempo sul giro per totale di 30” con la precisione dei centesimi di secondo.

![Mockup fine giro](http://tangodev.url.ph/openlaptimer/mockup-fine-giro.png)

###Perché GPS?
La tecnologia GPS ci consente di non avere alcun oggetto da mantenere sul muretto in pista e quindi avere tutto concentrato sul veicolo.
Inoltre in questo modo è possibile avere i dati sulla posizione nel tempo e quindi delineare le traiettorie di ogni giro.

###Features supportate
* registrazione del tempo sul giro
* registrazione del numero di giri effettuati in una sessione in pista
* tracking di tutte le posizioni GPS con log su SD

###Features future
* gestione Tracciati con riferiementi del traguardo e dei settori
* STOP / START automatico in base al movimento / pausa del veicolo
* fix compilazione IDE 1.5.8
* registrazione degli intertempi
* interfaccia utente curata
* aumento della precisione della posizione
* aumento della precisione dei calcoli per determinare il raggiungimento del traguardo / intertempo
* aumento performance generali
* sincronizzazione wireless (BT) dei dati di tracciato / sessioni in pista
* riconoscimento automatico del tracciato
* modalità di funzionamento in TOURING mode: registrazione traccia GPS e tempo totale, velocità max, min, media, soste ecc...
* creazione documentazione connessioni HW
* creazione HW dedicato
* creazione database condiviso dei tracciati internazionali da usare con il laptimer
* contamarce
* creaazione automatica tracciato se non impostato

###Dispositivi utilizzati
* Arduino DUE (DUE R3-E) (http://arduino.cc/en/Main/ArduinoBoardDue)
* Adafruit Ultimate GPS breakout (https://www.adafruit.com/product/746)
* Adafruit 2.8" TFT LCD with Touchscreen Breakout Board w/MicroSD Socket - ILI9341 (http://www.adafruit.com/product/1770)

###Librerie utilizate
* GPS https://github.com/adafruit/Adafruit-GPS-Library
* TFT https://github.com/adafruit/Adafruit-GFX-Library https://github.com/adafruit/Adafruit_ILI9341
* Custom Font (in previsione di utilizzo) https://github.com/marekburiak/ILI9341_due

###Note
Usare la versione 1.5.6 dell’IDE di arduino per compilare il progetto. Le versioni successive non compilano correttamente.
