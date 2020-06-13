// ****************** Whack-A-Mole***************
// Program written by: Andrew Kurczewski, Joshua Lagger, Zihao Cui
// Date Created: 10/22/2019
// Last Modified: 11/3/2019
// Brief description of the program: 
// This is the game Whack_A_Mole where 9 LEDS are lit one at a time and the objective is
// to have the user "whack" each mole by pressing the corresponding switch with the LED.
// Each mole whacked counts as a point which is tallied and displayed as the total score. 
// When the game is over the tallied score is displayed and the game is able to be restarted 
// indefinently. 
//
// Hardware connections:
// PXx is switch input (1 means switch ……………..; 0 means switch …………………….)
// PXx is switch input (1 means switch ……………….; 0 means switch …………………….)
// PXx is LED output   (0 means LED is …………………, 1 means LED is ……………………)
// PXx is LED output   (0 means LED is ………………,, 1 means LED is ……………………)

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "SysTickInts.h"
#include "PLL.h"

void Init_Ports(void);
void DisplayScoreToLCD(void);
void ShowMole(int mole);
void FillMoleArray(void);
void GetPseudoRand(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // Previous I bit, disable interrupts
void EndCritical(long sr);    // Restore I bit to previous value
void WaitForInterrupt(void);  // Low power mode
volatile uint32_t count = 0;
volatile uint32_t timer = 0;
// Input from switches
unsigned long startGame, Activate, MOLE1, MOLE2, MOLE3, MOLE4, MOLE5,
	MOLE6, MOLE7, MOLE8, MOLE9;
unsigned int score, value, i, arrayCount;
int randNumber;
unsigned int moleArray[9];
// Interger values used for boolean logic.(false = 0, true = 1)
unsigned int isGamePlaying, isGameOver, isLitM1, isLitM2, isLitM3, isLitM4,
	isLitM5, isLitM6, isLitM7, isLitM8, isLitM9;
unsigned int isHitM1, isHitM2, isHitM3, isHitM4, isHitM5, isHitM6, isHitM7, isHitM8, isHitM9;

int main(void){
	// Initialization
	// Call function to set us clock at 80 MHz located in PLL.c
  PLL_Init();
	// Initialize ports
	Init_Ports();
	// Variable init
  SysTick_Init(8000000);
  EnableInterrupts();
  isGamePlaying = 0;
	isGameOver = 0;
  count = 0;
	score = 0;
	value = 0;
	isLitM1 = 0;
	isLitM2 = 0;
	isLitM3 = 0;
	isLitM4 = 0;
	isLitM5 = 0;
	isLitM6 = 0;
	isLitM7 = 0;
	isLitM8 = 0;
	isLitM9 = 0;
	isHitM1 = 0;
	isHitM2 = 0;
	isHitM3 = 0;
	isHitM4 = 0;
	isHitM5 = 0;
	isHitM6 = 0;
	isHitM7 = 0;
	isHitM8 = 0;
	isHitM9 = 0;
	score = 0;
	randNumber = 0;
	arrayCount = 0;
	// Inifinite loop that allows input to be read every tenth of a second
  while(1){  
		 WaitForInterrupt();		
  }
}

// Interrupt service routine
// Executed every 12.5ns*(period) = .1 Sec
void SysTick_Handler(void){
	// Button definitions
	startGame = GPIO_PORTB_DATA_R & 0x01;	// (PB0)
	MOLE1 = GPIO_PORTB_DATA_R & 0x02;	// (PB1)
	MOLE2 = GPIO_PORTB_DATA_R & 0x04;	// (PB2)
	MOLE3 = GPIO_PORTB_DATA_R & 0x08;	// (PB3)
	MOLE4 = GPIO_PORTB_DATA_R & 0x10;	// (PB4)
	MOLE5 = GPIO_PORTB_DATA_R & 0x20;	// (PB5)
	MOLE6 = GPIO_PORTB_DATA_R & 0x40;	// (PB6)
	MOLE7 = GPIO_PORTB_DATA_R & 0x80;	// (PB7)
	MOLE8 = GPIO_PORTD_DATA_R & 0x01;	// (PD0)
	MOLE9 = GPIO_PORTD_DATA_R & 0x02;	// (PD1)
	
	// Add one to the timer value each SysTick cycle
	timer = timer + 1;
	
	// Check if game started button is pressed
	if (startGame !=0 && isGamePlaying != 1){
		isGamePlaying = 1;
		// Fill the mole array
		FillMoleArray();
	}
	else{
		// When the game is not playing
		if(!isGamePlaying){
			// Light the start button LED
			GPIO_PORTE_DATA_R |= 0x01; // (PE0)
		}
	}
	
	// Game is now playing
	if(isGamePlaying == 1){
			// Show the score on the LCD
			DisplayScoreToLCD();
			// MOLE1
			if(count > 0 && count <= 7){
				ShowMole(moleArray[0]);
			}
			// MOLE2
			if(count > 7 && count <= 14){
				ShowMole(moleArray[1]);
			}
			// MOLE3
			if (count > 14 && count <= 21){
				ShowMole(moleArray[2]);
			}
			// MOLE4
			if (count > 21 && count <= 28){
				ShowMole(moleArray[3]);
			}
			// MOLE5
			if (count > 28 && count <= 35){
				ShowMole(moleArray[4]);
			}
			// MOLE6
			if (count > 35 && count <= 42){
				ShowMole(moleArray[5]);
			}
			// MOLE7
			if (count > 42 && count <= 49){
				ShowMole(moleArray[6]);
			}
			// MOLE8
			if (count > 49 && count <= 56){
				ShowMole(moleArray[7]);
			}
			// MOLE9
			if (count > 56 && count <= 63){
				ShowMole(moleArray[8]);
			}
			
			// MOLE sequence is done
			if (count > 63){
				// Turn LEDs off
				GPIO_PORTA_DATA_R &= 0x00;
				GPIO_PORTE_DATA_R &= 0x00;
				isLitM9 = 0;
				count = 0;
				isGamePlaying = 0;
				isGameOver = 1;
			}
			count = count + 1;
	}

	// Checks if mole was hit only while the mole button is lit
	if(isLitM1 == 1){
		// Check for mole 1 button press
		if(MOLE1 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM1 = 1;
		}
	}
	if(isLitM2 == 1){
		// Check for mole 2 button press
		if(MOLE2 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM2 = 1;
		}
	}
	if(isLitM3 == 1){
		// Check for mole 3 button press
		if(MOLE3 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM3 = 1;
		}
	}
	if(isLitM4 == 1){
		// Check for mole 1 button press
		if(MOLE4 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM4 = 1;
		}
	}
	if(isLitM5 == 1){
		// Check for mole 1 button press
		if(MOLE5 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM5 = 1;
		}
	}
	if(isLitM6 == 1){
		// Check for mole 1 button press
		if(MOLE6 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM6 = 1;
		}
	}
	if(isLitM7 == 1){
		// Check for mole 1 button press
		if(MOLE7 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM7 = 1;
		}
	}
	if(isLitM8 == 1){
		// Check for mole 1 button press
		if(MOLE8 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM8 = 1;
		}
	}
	if(isLitM9 == 1){
		// Check for mole 1 button press
		if(MOLE9 !=0){
			// Add to score for mole being hit
			score = score +1;
			// Change is hit to 1 so only one point is counted
			isHitM9 = 1;
		}
	}
	
	// Display score by blinking an LED for each hit for now
	if (isGameOver == 1){
		// Display the score to the LCD
		DisplayScoreToLCD();
		// Clear the score for the next game
		score = 0;
		
		// Once the score has been displayed reset booleans
		isHitM1 = 0;
		isHitM2 = 0;
		isHitM3 = 0;
		isHitM4 = 0;
		isHitM5 = 0;
		isHitM6 = 0;
		isHitM7 = 0;
		isHitM8 = 0;
		isHitM9 = 0;
		isGameOver = 0;
		arrayCount = 0;
	}
}

void Init_Ports(void){
	// Activate ports A, B, D, E
  SYSCTL_RCGCGPIO_R |= 0x1B;
	// Port A init (Output: 2-7)
	GPIO_PORTA_DIR_R |= 0xFC;
  GPIO_PORTA_AFSEL_R &= 0x00;
  GPIO_PORTA_DEN_R |= 0xFC;  
  GPIO_PORTA_PCTL_R = 0x00000000;
  GPIO_PORTA_AMSEL_R = 0;
	// Port B init (Input: 0-7)
	GPIO_PORTB_DIR_R |= 0x00;
  GPIO_PORTB_AFSEL_R &= 0x00;
  GPIO_PORTB_DEN_R |= 0xFF;  
  GPIO_PORTB_PCTL_R = 0x00000000;
  GPIO_PORTB_AMSEL_R = 0;
	// Port D init (Input: 0,1)(Output: 2,3,6)
	GPIO_PORTD_DIR_R |= 0x4C;
  GPIO_PORTD_AFSEL_R &= 0x00;
  GPIO_PORTD_DEN_R |= 0x4F;  
  GPIO_PORTD_PCTL_R = 0x00000000;
  GPIO_PORTD_AMSEL_R = 0;
	// Port E init (Output: 0-5)
	GPIO_PORTE_DIR_R |= 0x3F;
  GPIO_PORTE_AFSEL_R &= 0x00;
  GPIO_PORTE_DEN_R |= 0x3F;  
  GPIO_PORTE_PCTL_R = 0x00000000;
  GPIO_PORTE_AMSEL_R = 0;

}

void DisplayScoreToLCD(void){
	// PD2: binary value 0
	// PD3: binary value 1
	// PD6: binary value 2
	// PE5: binary value 3
	// PE4: digit value (1s or 10s)
	// Convert the score to binary
	if(score < 10){
		switch(score){
			case 0:
				// Output a 0
				GPIO_PORTD_DATA_R = 0x00;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 1:
				// Output a 1
				GPIO_PORTD_DATA_R = 0x04;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 2:
				// Output a 2
				GPIO_PORTD_DATA_R = 0x08;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 3:
				// Output a 3
				GPIO_PORTD_DATA_R = 0x0C;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 4:
				// Output a 4
				GPIO_PORTD_DATA_R = 0x40;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 5:
				// Output a 5
				GPIO_PORTD_DATA_R = 0x44;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 6:
				// Output a 6
				GPIO_PORTD_DATA_R = 0x48;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 7:
				// Output a 7
				GPIO_PORTD_DATA_R = 0x4C;
				GPIO_PORTE_DATA_R = 0x00;
				break;
			case 8:
				// Output a 8
				GPIO_PORTD_DATA_R = 0x00;
				GPIO_PORTE_DATA_R = 0x20;
				break;
			case 9:
				// Output a 9
				GPIO_PORTD_DATA_R = 0x04;
				GPIO_PORTE_DATA_R = 0x20;
				break;
		}
	}
	if(score > 10){
		// Get the 10s digit
		value = score / 10;
		switch(value){
			case 1:
				// Output a 1 (in the tens place)
				GPIO_PORTD_DATA_R = 0x04;
				GPIO_PORTE_DATA_R = 0x10;
				break;
			case 2:
				// Output a 2 (in the tens place)
				GPIO_PORTD_DATA_R = 0x08;
				GPIO_PORTE_DATA_R = 0x10;
				break;
			case 3:
				// Output a 3 (in the tens place)
				GPIO_PORTD_DATA_R = 0x0C;
				GPIO_PORTE_DATA_R = 0x10;
				break;
		}
	}
}

void ShowMole(int mole){
	// Show the mole given in the function arguments
	switch(mole){
		case 1:
			// Turn off other moles lights
			isLitM2 = 0;
			isLitM3 = 0;
			isLitM4 = 0;
			isLitM5 = 0;
			isLitM6 = 0;
			isLitM7 = 0;
			isLitM8 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM2 = 0;
			isHitM3 = 0;
			isHitM4 = 0;
			isHitM5 = 0;
			isHitM6 = 0;
			isHitM7 = 0;
			isHitM8 = 0;
			isHitM9 = 0;
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x03;
			GPIO_PORTE_DATA_R &= 0xF2;
			// Mole is not hit yet
			if(isHitM1 == 0){
				GPIO_PORTE_DATA_R |= 0x02; // Toggle LED (PE1)
				isLitM1 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTE_DATA_R &= 0xFD; // Turn LED off
				isLitM1 = 0;
			}
			break;
		case 2:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM3 = 0;
			isLitM4 = 0;
			isLitM5 = 0;
			isLitM6 = 0;
			isLitM7 = 0;
			isLitM8 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM3 = 0;
			isHitM4 = 0;
			isHitM5 = 0;
			isHitM6 = 0;
			isHitM7 = 0;
			isHitM8 = 0;
			isHitM9 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x03;
			GPIO_PORTE_DATA_R &= 0xF4;
			// Mole is not hit yet
			if(isHitM2 == 0){
				GPIO_PORTE_DATA_R |= 0x04; // Toggle LED (PE2)
				isLitM2 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTE_DATA_R &= 0xFB; // Turn LED off
				isLitM2 = 0;
			}
			break;
		case 3:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM2= 0;
			isLitM4 = 0;
			isLitM5 = 0;
			isLitM6 = 0;
			isLitM7 = 0;
			isLitM8 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM2 = 0;
			isHitM4 = 0;
			isHitM5 = 0;
			isHitM6 = 0;
			isHitM7 = 0;
			isHitM8 = 0;
			isHitM9 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x03;
			GPIO_PORTE_DATA_R &= 0xF8;
			// Mole is not hit yet
			if(isHitM3 == 0){
				GPIO_PORTE_DATA_R |= 0x08; // Toggle LED (PE3)
				isLitM3 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTE_DATA_R &= 0xF7; // Turn LED off
				isLitM3 = 0;
			}
			break;
		case 4:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM2 = 0;
			isLitM3 = 0;
			isLitM5 = 0;
			isLitM6 = 0;
			isLitM7 = 0;
			isLitM8 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM2 = 0;
			isHitM3 = 0;
			isHitM5 = 0;
			isHitM6 = 0;
			isHitM7 = 0;
			isHitM8 = 0;
			isHitM9 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x07;
			GPIO_PORTE_DATA_R &= 0xF0;
			// Mole is not hit yet
			if(isHitM4 == 0){
				GPIO_PORTA_DATA_R = 0x04; // Toggle LED (PA2)
				isLitM4 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTA_DATA_R = 0x00; // Turn LED off
				isLitM4 = 0;
			}
			break;
		case 5:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM2 = 0;
			isLitM3 = 0;
			isLitM4 = 0;
			isLitM6 = 0;
			isLitM7 = 0;
			isLitM8 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM2 = 0;
			isHitM3 = 0;
			isHitM4 = 0;
			isHitM6 = 0;
			isHitM7 = 0;
			isHitM8 = 0;
			isHitM9 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x0B;
			GPIO_PORTE_DATA_R &= 0xF0;
			// Mole is not hit yet
			if(isHitM5 == 0){
				GPIO_PORTA_DATA_R = 0x08; // Toggle LED (PA3)
				isLitM5 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTA_DATA_R = 0x00; // Turn LED off
				isLitM5 = 0;
			}
			break;
		case 6:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM2 = 0;
			isLitM3 = 0;
			isLitM4 = 0;
			isLitM5 = 0;
			isLitM7 = 0;
			isLitM8 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM2 = 0;
			isHitM3 = 0;
			isHitM4 = 0;
			isHitM5 = 0;
			isHitM7 = 0;
			isHitM8 = 0;
			isHitM9 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x13;
			GPIO_PORTE_DATA_R &= 0xF0;
			// Mole is not hit yet
			if(isHitM6 == 0){
				GPIO_PORTA_DATA_R = 0x10; // Toggle LED (PA4)
				isLitM6 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTA_DATA_R = 0x00; // Turn LED off
				isLitM6 = 0;
			}
			break;
		case 7:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM2 = 0;
			isLitM3 = 0;
			isLitM4 = 0;
			isLitM5 = 0;
			isLitM6 = 0;
			isLitM8 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM2 = 0;
			isHitM3 = 0;
			isHitM4 = 0;
			isHitM5 = 0;
			isHitM6 = 0;
			isHitM8 = 0;
			isHitM9 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x23;
			GPIO_PORTE_DATA_R &= 0xF0;
			// Mole is not hit yet
			if(isHitM7 == 0){
				GPIO_PORTA_DATA_R = 0x20; // Toggle LED (PA5)
				isLitM7 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTA_DATA_R = 0x00; // Turn LED off
				isLitM7 = 0;
			}
			break;
		case 8:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM2 = 0;
			isLitM3 = 0;
			isLitM4 = 0;
			isLitM5 = 0;
			isLitM6 = 0;
			isLitM7 = 0;
			isLitM9 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM2 = 0;
			isHitM3 = 0;
			isHitM4 = 0;
			isHitM5 = 0;
			isHitM6 = 0;
			isHitM7 = 0;
			isHitM9 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x43;
			GPIO_PORTE_DATA_R &= 0xF0;
			// Mole is not hit yet
			if(isHitM8 == 0){
				GPIO_PORTA_DATA_R = 0x40; // Toggle LED (PA6)
				isLitM8 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTA_DATA_R = 0x00; // Turn LED off
				isLitM8 = 0;
			}
			break;
		case 9:
			// Turn off other moles lights
			isLitM1 = 0;
			isLitM2 = 0;
			isLitM3 = 0;
			isLitM4 = 0;
			isLitM5 = 0;
			isLitM6 = 0;
			isLitM7 = 0;
			isLitM8 = 0;
			// Allow other moles to be hit
			isHitM1 = 0;
			isHitM2 = 0;
			isHitM3 = 0;
			isHitM4 = 0;
			isHitM5 = 0;
			isHitM6 = 0;
			isHitM7 = 0;
			isHitM8 = 0;		
			// Turn off last mole LED
			GPIO_PORTA_DATA_R &= 0x83;
			GPIO_PORTE_DATA_R &= 0xF0;
			// Mole is not hit yet
			if(isHitM9 == 0){
				GPIO_PORTA_DATA_R = 0x80; // Toggle LED (PA7)
				isLitM9 = 1;
			}
			// Mole has been hit
			else{
				GPIO_PORTA_DATA_R = 0x00; // Turn LED off
				isLitM9 = 0;
			}
			break;
	}
}

void FillMoleArray(void){
	// Get a random number
	GetPseudoRand();
	for( i = 0; i < 9 ; i++ ){
		moleArray[i] = randNumber;
		if(arrayCount == 0){
			randNumber = randNumber + 4;
			if(randNumber > 9){
				randNumber = randNumber - 6;
			}
		}
		if(arrayCount == 1){
			randNumber = randNumber - 3;
			if(randNumber < 0){
				randNumber = randNumber + 6;
			}
		}
		if(arrayCount == 2){
			randNumber = randNumber + 2;
			if(randNumber > 9){
				randNumber = randNumber - 3;
			}
		}
		if(arrayCount == 3){
			randNumber = randNumber + 3;
			if(randNumber > 9){
				randNumber = randNumber - 5;
			}	
		}
		if(arrayCount == 4){
			randNumber = randNumber - 5;
			if(randNumber < 0){
				randNumber = randNumber + 7;
			}
		}
		if(arrayCount == 5){
			randNumber = randNumber + 4;
			if(randNumber > 9){
				randNumber = randNumber - 5;
			}
		}
		if(arrayCount == 6){
			randNumber = randNumber - 2;
			if(randNumber < 0){
				randNumber = randNumber + 5;
			}
		}
		if(arrayCount == 7){
			randNumber = randNumber + 2;
			if(randNumber > 9){
				randNumber = randNumber - 4;
			}
		}
		if(arrayCount == 8){
			randNumber = randNumber + 5;
			if(randNumber > 9){
				randNumber = randNumber - 6;
			}
		}
		if(arrayCount == 9){
			randNumber = randNumber - 3;
			if(randNumber < 0){
				randNumber = randNumber + 5;
			}
		}

		arrayCount = arrayCount +1;
	}
}

void GetPseudoRand(void){
	// Get a digit 0-9 from the timer value
	randNumber = timer % 10;
	if(randNumber == 0){
		randNumber = 1;
	}
}
