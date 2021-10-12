# Lab 3: David Sladkowski

Link to your `Digital-electronics-2` GitHub repository:

   (https://github.com/david3891/Digital-electronic-2)


### Data types in C

1. Complete table.

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0, 1, ..., 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | -128, ..., 127 | Signed 8-bit integer |
| `uint16_t` | 16 | 0, ..., 65 535 | Unsigned 16-bit integer |
| `int16_t`  | 16 | -32 768, ..., 32 767 | Signed 16-bit integer |
| `float`    | 32 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`     | - | - | Function does not return a value |


### GPIO library

1. In your words, describe the difference between the declaration and the definition of the function in C.
   * Function declaration - Deklarace funkce je prototyp, který určuje název funkce, návratové typy a parametry bez těla funkce.
   * Function definition - Definice funkce na druhé straně odkazuje na skutečnou funkci, která určuje název funkce, návratové typy a parametry s tělem funkce.

2. Part of the C code listing with syntax highlighting, which toggles LEDs only if push button is pressed. Otherwise, the value of the LEDs does not change. Use function from your GPIO library. Let the push button is connected to port D:

```c
    // Configure Push button at port D and enable internal pull-up resistor
    GPIO_read(&PORTD, BUTTON);
    GPIO_config_input_pullup(&DDRD, BUTTON);

    // Infinite loop
    while (1)
    {
        if(bit_is_clear(PIND, BUTTON))
		  {
			  _delay_ms(BLINK_DELAY);
			  PORTB = PORTB ^ (1<<LED_GREEN);
			  PORTC = PORTC ^ (1<<LED_RED);
			  _delay_ms(BLINK_DELAY);
			  loop_until_bit_is_set(PIND,BUTTON);
		  }
    }
```


### Traffic light

1. Scheme of traffic light application with one red/yellow/green light for cars and one red/green light for pedestrians. Connect AVR device, LEDs, resistors, one push button (for pedestrians), and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()
