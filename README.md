# Phuc Tran

## January 2022

### Reaction Arduino Game

### Description

The program implements a two-person reaction game where each player is given a push button and a respective LED. A third LED will act as the start signal. At first the LED will blink rapidly until either player press their button. Then, the LED will count down at a random pace. Finally, once the LED is blinking at a steady rate, the first player to press their button wins the game, and their LED will blinky rapidly. 

### State Diagram

![alt text](https://github.com/phuc-tran-hub/Reaction-Game/blob/main/ReactionGameStateMachine.drawio.png?raw=true)

### Evaluation

Since this is the one of the first few project involved with embedded programming, there are many techniques that I can polish. One inherit flaw with the code is that there will be a bias as to which player takes priority within the code. As I learned in the near future, we can use interrupts to our convenience and detect pin change (HIGH to LOW) at any point of the code rather than those if statements. There are many more subjects in embedded programming that I am learning from i2C, USART communication to Accelerometer, Motor and Servos!

### Execution

1. Complete the Arduino/Breadboard setup according to reactionGame.c header comments

2. Power the Arduino to the computer

3. Make flash the file onto the Arduino
