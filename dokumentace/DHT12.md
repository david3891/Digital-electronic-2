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

![obrázek](https://github.com/tomas-fryza/Digital-electronics-2/blob/master/Labs/08-i2c/Images/i2c_protocol.jpg)

   | **Adresa registru** | **Popis adresy** |
   |         :-:         |        :-:       |
   | 0x00                | vlhkosť          |
   | 0x01                | vlhkosť (zlomková čásť ) |
   | 0x02                | teplota          |
   | 0x03                | teplota (zlomková část) |
   | 0x04                | kontrola (súčet všetkých informácií) |
   
   ## Zapojenie:
   
   ![zapojení](https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.instructables.com%2FDHT12-i2c-Cheap-Humidity-and-Temperature-Sensor-Fa%2F&psig=AOvVaw3YkZt3d1kQbtiiSi_RFIaW&ust=1639668434795000&source=images&cd=vfe&ved=0CAsQjRxqFwoTCKja7YqP5vQCFQAAAAAdAAAAABAD)
   
