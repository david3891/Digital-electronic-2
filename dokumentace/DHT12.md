# DHT12

Senzor na meranie vlhkosti a teploty prostredia. Komunikácia prebieha cez protokol I2C alebo cez Single bus zapojenie.

## Piny:

**VCC:** 2,7V až 5,5V  
**GND:** uzemnenie  
**SCL:** hodinový vstup pre I2C rozhranie  
**SDA:** data vstup/výstup pre I2C rozhranie  

## I2C komunikácia:

- je zapnutá keď SCL pin je v stave HIGH  
- V prvej časti komunikácie master odosiela adresný frame, ktorý predáva informáciu o typu dát, ktorú mu slave má poskytnúť, tento byte sa skladá z 8 bitov, z toho je posledný R/W bit obsahujúci informáciu či bude informácia od mastera čítaná alebo zapisovaná (v tomto prípade bude zapisovaná, takže hodnota tohto bitu je 0) a následne ACK/NAK bit. Tento adresný frame má mať dekadickú hodnotu 184. Ďalší byte ktorý bude odoslaný obsahuje informáciu o adrese registru, táto informácia hovorí akú informáciu má slave preposlať masterovi v ďalšej časti komunikácie (vlhkosť alebo teplota).  
- V ďalšej časti komunikácie slave odošle ten istý adresný byte, až na R/W bit, ktorý je tentoraz 1, čo znamená že dáta budú čítané. Ďalej slave odošle byte, podľa adresy registru, ktorú master poslal a posiela až dokým nejaký byte nebude mať hodnotu ACK/NAK 1.
