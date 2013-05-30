# Requirements

Some of the requirements based on the things we'll need to control.

Keep in mind that I'm using this document as a "mind dump" too in order to remember everything about this project, so you might see too much information in some places where it's not needed. **:)**


## Microcontroller Requirements

Because we are awesome, we'll need a microcontroller with the following specifications:

**Miscellaneous**

  - Being able to be powered from a 3.3v source like a coin cell battery
  - The RX/TX ports should remain free for a possible serial communication expansion
  - "Standby mode" to consume very little current from our battery while the user isn't interacting with the input buttons
  - PWM capability

**I/O**

  - 1x Interrupt pin for the multiplexer check routine
  - 4x Input pins for the multiplexer
  - 7x Output pins for the LCD
  - 1x PWM pin to set the LM317 adjust pin
  - 2x Output pins to control the logic for the output transistors
  - 2x Serial communication pins (RX/TX)

After looking at these specifications, trimming things, and reading datasheets I decided to use the [TI MSP430G2553](http://www.ti.com/product/msp430g2553). At first I was thinking about using a [Microchip PIC18F2550](http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en010280) since I'm a lot more familiar with PIC chips, but I started this project to learn more about electronics, and since I had a [Launchpad](http://processors.wiki.ti.com/index.php/MSP430_LaunchPad_(MSP-EXP430G2)) laying around and waiting for a project I decided to use it to learn more about the MSP430 line too.

Because of the limitations of the chip in terms of amount of I/O pins (which is a good thing to have if you want to learn how to work with constraints) I decided to use a multiplexer for the user input interface and some XOR gates to control the output transistors.


## Input Multiplexer

Since we are going to have a awesome LCD interface we'll need some push buttons to interact with it. Instead of using a bunch of precious inputs from our microcontroller we'll use a multiplexer, in our case a [SN74LS251](http://www.ti.com/product/sn74ls251). It'll receive inputs from the following parts:

  - 1x Standby button
  - 2x Menu selector buttons for the LCD interface
  - 4x Directional buttons for the LCD interface

### Interrupt

In order for the microcontroller to consume as little power as possible, instead of running the multiplexer check routine every single time we'll put it in a "standby mode" and use interrupts to wake it up. To make this possible we'll put a wire in parallel with the input array going to the interrupt pin.


## Output Logic

Since we want to use as little I/O pins as possible I decided to use some XOR gates from a [SN74HC86](http://www.ti.com/lit/ds/symlink/sn74hc86.pdf), to control the output transistors, so instead of using 3 pins, one for each transistor, we only use 2 pins. It's a simple logic circuit and you can read more about it in the [Output Logic Schematic](output-logic-sch-file) file.


## LCD Power

As specified in the general microcontroller requirements list, the microcontroller will be powered from a 3.3v coin cell battery and our LCD requires a 5v supply. To power it we'll need a [voltage doubler](link-to-wikipedia-here), as seen in [this EEVBlog post](link-to-post-here).


---------------------------------------------------

Check the LCD driver chip that Dave used in his uWatch project since the Hitachi one I have can be powered from 2.5v up to 5.5v. Maybe it just works better at 5v?

More information: http://www.calcwatch.com/construction.htm

---------------------------------------------------
