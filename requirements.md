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
  - 4x Input pins for the muiltiplexer
  - 6x Output pins for the LCD
  - *REMOVE IF THERE IS PWM LM317 ADJUST* 4x I/O pins for the digital potentiometer SPI (check if it isn't I2C) interface
  - 1x PWM pin to set the LM317 adjust pin
  - 3x Output pins to power the output transistors
  - 2x Serial communication pins (RX/TX)


## Input Multiplexer

Since we are going to have a awesome LCD interface we'll need some push buttons to interact with it. Instead of using a bunch of precious inputs from our microcontroller we'll use a multiplexer, in our case a [SN74LS251](datasheet-link). It'll receive inputs from the following parts:

  - 1x Standby button
  - 2x Menu selector buttons for the LCD interface
  - 4x Directional buttons for the LCD interface

### Interrupt

In order for the microcontroller to consume as little power as possible, instead of running the multiplexer check routine every single time we'll put it in a "standby mode" and use interrupts to wake it up. To make this possible we'll put a wire in parallel with the input array going to the interrupt pin.


## LCD Power

As specified in the general microcontroller requirements list, the microcontroller will be powered from a 3.3v coin cell battery and our LCD requires a 5v supply. To power it we'll need a [voltage doubler](link-to-wikipedia-here), as seen in [this EEVBlog post](link-to-post-here).

---------------------- OBS -----------------------------
Check the LCD driver chip that Dave used in his uWatch project since the Hitachi one I have can be powered from 2.5v up to 5.5v. Maybe it just works better at 5v?
--------------------------------------------------------
