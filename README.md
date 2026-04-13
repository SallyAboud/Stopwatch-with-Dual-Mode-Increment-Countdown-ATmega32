# Dual Mode Stopwatch

This project presents the design and implementation of a dual-mode digital stopwatch using the ATmega32 microcontroller and six multiplexed seven-segment displays. It supports both Increment (Count-Up) and Countdown (Count-Down) modes, making it suitable for timing and embedded systems learning applications.

## How It Works

The system starts automatically in Increment mode when powered on and begins counting from 00:00:00.

Using dedicated push buttons, the user can:

* Pause, resume, or reset the timer
* Toggle between Increment and Countdown modes
* Adjust hours, minutes, and seconds in Countdown mode

Timer1 is configured in CTC mode to generate accurate 1-second timing. External interrupts handle Reset (INT0), Pause (INT1), and Resume (INT2).

In Countdown mode, when the timer reaches zero, a buzzer is activated to alert the user. LED indicators show the active operating mode.

## Components

* ATmega32 Microcontroller (16 MHz)
* Six Common-Anode 7-Segment Displays
* 7447 BCD-to-7-Segment Decoder
* NPN Transistors (for multiplexing)
* Push Buttons (control & time adjustment)
* Buzzer
* LEDs

## Development Tools

* IDE: Eclipse
* Compiler: AVR-GCC
* Programmer: eXtreme Burner-AVR (ETA32)

## Features

* Dual operating modes (Increment & Countdown)
* Accurate timing using Timer1 (CTC mode)
* External interrupt-based control
* Multiplexed display for efficient pin usage
* Buzzer alert on countdown completion

## Demo Video & Files

[Proteus Simulation](https://drive.google.com/file/d/1vLMy9-rVtDjLcr0inuOKUk3VrSZu9kjz/view?usp=drive_link)
## License

This project is for educational and non-commercial use.
