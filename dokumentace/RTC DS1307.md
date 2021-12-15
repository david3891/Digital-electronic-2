# Real time clock DS 1307

Mikrokontrolér udržiava sekundy, minúty, hodiny, dni, mesiace a roky ako informáciu. Automaticky započítava počty dni v mesiacoch a takisto prechodné roky. Hodiny operujú v 24 aj v 12 hodinovom formáte s AM/PM indikátorom.

## Parametre a špecifikácia:

- Dvoj-zbernicové I2C rozhranie
- Riadiace napätie : 3,3V – 5V
- Obsahuje batériovú zálohu
- 1Hz výstupný pin
- 56 bytová of neprelietavá (non-volatile) pamäť dostupná pre použivateľa
- Pracovný prúd: 1,5 mA
- Pracovná teplota: 0°C do +70°C

## Piny:

**VCC:** 3,3V až 5,5V  
**GND:** uzemnenie  
**BAT:** náhradné napájanie pre 3V lítiovú batériu, využíva sa na udržiavanie presného času, pre prípád keď hlavný zdroj energie je prerušený; **keď sa nevyužíva tento pin, musí byť uzemnený**  
**SQW pin:** výstup sú 4 obdĺžnikové signály: 1Hz, 4kHz, 8kHz alebo 32kHz; **ktoré sú programovateľné**  
**SLC:** hodinový vstup pre I2C rozhranie  
**SDA:** data vstup/výstup pre I2C rozhranie, vyžaduje vonkajší pull-up rezistor  
**DS:** pin má výstup teploty, pokiaľ je teplotný senzor pripojený  

![obrázek](https://lastminuteengineers.b-cdn.net/wp-content/uploads/arduino/DS1307-RTC-Module-Pinout.png)
