/* Name: Phuc Tran
 *
 * Program name: reactionGame.c
 * Date created: 1/19/22
 * Description: Complete a reaction timing game with two players
 *
 * Dependencies: (what other source files are required)
 * 1. <avr/io.> include input and output registers
 * 2. <util/delay.h> include delay commands
 * 
 * I/O pins:  (what ports are used, and what are they wired to?)
 * Port B:
 * bit 1: left LED
 * bit 2: middle LED
 * bit 3: right LED
 * 
 * Port D: 
 * bit 7: the left button
 * bit 6: the right button
 * 
 * Revisions: (use this to track the evolution of your program)
 * 1. create a initial draft with 4 states and it worked as intended
 * 2. the speed variable seems to not work parameters since _delay_ms() only like constant integers
 * Will find another way change speed with a for loop
 * 3. updated count so it's random by setting the condition to go from preparation to start also modified it so a false start can happen
 */

/* INCLUDE FILES */
#include <avr/io.h>
#include <util/delay.h>

/* FUNCTIONS */

/* CONSTANTS */
#define LED_ON_TIME 200
#define COUNTDOWN_TIME 2000
#define LED_OFF_TIME 200
#define UNIT_TIME 500
#define DECLARE_TIME 100

/* CODE */
int main(void){
    typedef enum {NEUTRAL, PREPARATION, START, DECLARE} state_t;
    state_t state = NEUTRAL;

    // Setup code:
    // Set bit 0, 1, 2 of Port B as output
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << PORTB2);

    // LED1: Player 1's LED, LED2: starter LED, LED3: Player 2's LED
    const uint8_t led_mask = (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
    const uint8_t left_led_mask = 1;
    const uint8_t starter_led_mask = 2;
    const uint8_t right_led_mask = 4;

    // Set bit 7 and 6 of Port D as input, explicitly
    DDRD  &= ~(1 << DDD7) & ~(1 << DDD6);
  	// Enable pull-up resistor on both pins
  	PORTD |= (1 << PORTD7) | (1 << PORTD6);	

    // PIN1: left button, PIN2: right button
    const uint8_t left_button_pin_mask = (1 << PIND7);
    const uint8_t right_button_pin_mask = (1 << PORTD6);

    // Let the starting count be 0
    uint8_t countdown = 0;
    // Let the default speed be 1 * LED_ON_TIME 
    uint8_t speed = 1;
    // Let the default winner be Player 1
    uint8_t winner = 1;
    uint8_t winner_count = 0;

    while (1){
        // Update the counter 
        switch(state){
            case NEUTRAL:
                
                // If both buttons are unpressed, blink the starter LED rapidly
                if ((PIND & left_button_pin_mask) != 0 && (PIND & right_button_pin_mask) != 0){
                    PORTB &= ~led_mask;
                    PORTB |= starter_led_mask;
                    _delay_ms(LED_ON_TIME);   

                    PORTB &= ~led_mask;
                    _delay_ms(LED_OFF_TIME); 

                    // TODO: make the speed dynamic to the 
                    speed++;
                    if (speed == 7){
                        speed = 1;
                    }  
                }

                // If either button is pressed, begin to count down the starter LED
                else if ((PIND & left_button_pin_mask) == 0 || (PIND & right_button_pin_mask) == 0){
                    state = PREPARATION;
                    _delay_ms(UNIT_TIME);
                }
                break;
            
            case PREPARATION:

                // EC: Count to the random count that was created in preparation
                countdown++; 

                // EC: A false start, pressing one's button out of turn awards a win to the other player
                if ((PIND & left_button_pin_mask) == 0){
                    winner = 2;
                    countdown = 0;
                    state = DECLARE;
                    break;
                }

                else if ((PIND & right_button_pin_mask) == 0){
                    winner = 1;
                    countdown = 0;
                    state = DECLARE;
                    break;
                }

                // Continue to countdown if otherwise
                PORTB &= ~led_mask;
                PORTB |= starter_led_mask;
                _delay_ms(COUNTDOWN_TIME);   

                PORTB &= ~led_mask;
                _delay_ms(COUNTDOWN_TIME);
            
                if (countdown == speed){
                    state = START;
                }

                break;
            
            case START:

                // If both buttons are unpressed, blink the starter LED rapidly
                if ((PIND & left_button_pin_mask) != 0 && (PIND & right_button_pin_mask) != 0){
                    PORTB &= ~led_mask;
                    PORTB |= starter_led_mask;
                    _delay_ms(LED_ON_TIME);   

                    PORTB &= ~led_mask;
                    _delay_ms(LED_OFF_TIME); 
                }

                // If the left button is pressed, set the winner as 1 and go to the next state
                else if ((PIND & left_button_pin_mask) == 0){
                    winner = 1;
                    state = DECLARE;
                }

                // If the right button is pressed, set the winner as 2 and go to the next state
                else if ((PIND & right_button_pin_mask) == 0){
                    winner = 2;
                    state = DECLARE;
                }

                break;
            
            case DECLARE:
                
                // Flash rapidly for a second before going back to Neutral
                if (winner_count == 10){
                    winner_count = 0;
                    state = NEUTRAL;
                }

                // declare the winner by blinking the player's LED rapidly
                // winner == 1 (Player 1 or left LED) and winner == 2 (Player 2 or right LED)
                else if (winner == 1){
                    PORTB &= ~led_mask;
                    PORTB |= left_led_mask;    
                    _delay_ms(DECLARE_TIME);   

                    PORTB &= ~led_mask;
                    _delay_ms(DECLARE_TIME);

                    winner_count++;   
                }
                else if(winner == 2){
                    PORTB &= ~led_mask;
                    PORTB |= right_led_mask;    
                    _delay_ms(DECLARE_TIME);   

                    PORTB &= ~led_mask;
                    _delay_ms(DECLARE_TIME);

                    winner_count++;    
                }
                break;
        }
    }
}