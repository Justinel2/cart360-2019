/********************** ETUDE 3 CART 360 2019 *******************************
 * WELCOME! 
 * THE PURPOSE OF THIS EXERCISE IS TO DESIGN A VERY SIMPLE KEYBOARD (5 KEYS)
 * WHICH ALLOWS YOU TO PLAY LIVE, RECORD, AND PLAYBACK SINGLE NOTES (NO CHORDS). 
 * THERE WILL BE A BUTTON WHICH WHEN PRESSED WILL TAKE THE USER TO THE NEXT MODE:
 * THERE ARE 5 POSSIBLE MODES
 * 0 ==  reset    ==   led off == also resets
 * 1 ==  live     ==   led BLUE
 * 2 ==  record   ==   led RED
 * 3 ==  play     ==   led GREEN
 * 4 ==  loopMODE ==   led PURPLE
 * 
 * ADDITIONALLY - WHEN THE USER CHANGES MODE, 
 * THE RGB LED WILL CHANGE COLOR (ACCORDING TO THE SPECIFICATIONS)

 * PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
 * DO NOT ADD ANY MORE FUNCTION DEFINITIONS 
 * ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
 * THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
 * PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
 * GOOD LUCK!
 */
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 9 // R PIN
#define LED_PIN_G 6 // G PIN
#define LED_PIN_B 5 // B PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration = 200;

// constant for pin to output for buzzer 
#define BUZZER_PIN 3 // PWM

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES]; 
int mode =0; // start at off
int prevButtonState = LOW; // start at off

/*************************************************************************/

// ANALYSIS OF THE KEYBOARD
// The keyboard is a button-resistor ladder. The set of resistors are connected in serie so there
// is a voltage drop accross each resistors. This then attributes a different voltage to each button and inputted in the
// analog input. The voltage is then used as a frequency to be ring/outputted by the buzzer. 


/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  pinMode(BUTTON_MODE_PIN, INPUT);
}

/**********************LOOP() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change the loop - it establishes the flow of the program
 * We call here 3 functions repeatedly:
 * 1: chooseMode(): this function will determine the mode that your program is in 
 * based on if the button (linked to the BUTTON_MODE_PIN) was pressed
 * 2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
 * 3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  chooseMode();
  setRGB();
  selectMode();
}
/******************CHOOSEMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Read the value from the Button (linked to the BUTTON_MODE_PIN) and 
 * if is being pressed then change the mode variable.
 * REMEMBER:
 * mode == 0 is reset 
 * mode == 1 is live
 * mode == 2 is record
 * mode == 3 is play
 * mode == 4 is loopMode
 * Every time the user presses the button, the program will go to the next mode,
 * once it reaches 4, it should go back to mode == 0. 
 * (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode(){
  // Get the state of the button (HIGH (pressed) or LOW (not pressed))
  int buttonState = digitalRead(BUTTON_MODE_PIN);

  // If the state of the button changes
  if (buttonState != prevButtonState) {
    // If the button just pushed
    if (buttonState == HIGH) {
      // If the mode is not 4
      if (mode < 4) {
        // next mode
        mode++;
      }
      // if mode is 4
      else {
        // put back the mode to 0
        mode = 0;
      }
    }
  }
  // Put the current button state as the previous state for iteration
  prevButtonState = buttonState;
  
}

/******************SETRGB(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Depending on the value of the mode variable:
 * if the mode is 0 - RGB LED IS OFF
 * if the mode is 1 - RGB LED IS BLUE
 * if mode is 2 - RGB LED IS RED
 * if mode is 3 - RGB LED IS GREEN
 * if mode is 4 - RGB LED iS PURPLE
 * YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{
  // depending on the mode
  switch (mode) {
    // if the mode is 0
    case 0: 
      // put the red, green and blue light off
      analogWrite(LED_PIN_R,0);
      analogWrite(LED_PIN_G,0);
      analogWrite(LED_PIN_B,0);
      // quit the switch statement
      break;
    // if the mode is 1
    case 1:
      // turn on the blue light
      analogWrite(LED_PIN_B,200);
      // quit the switch statement
      break;
      // if the mode is 2
     case 2:
      // turn off the blue light
      // and turn on the red light
      analogWrite(LED_PIN_B,0);
      analogWrite(LED_PIN_R,200);
      // quit the switch statement
      break;
     // if the mode is 3
     case 3:
      // turn off the red light
      // and turn on the green light
      analogWrite(LED_PIN_R,0);
      analogWrite(LED_PIN_G,255);
      // quit the switch statement
      break;
     // if the mode is 4
     case 4:
      // turn off a green light
      // and turn on the red light
      // and turn on the blue light
      analogWrite(LED_PIN_G,0);
      analogWrite(LED_PIN_R,100);
      analogWrite(LED_PIN_B,125);
      // quit the switch statement
      break;
      // if none of the case correspond
      default:
        break;
  }
}
/**********************SELECTMODE() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
 * There are 4 possibilities
 * 1: reset(): this function will reset any arrays etc, and will do nothing else
 * 2: live(): this function will play the corresponding notes 
 * to the user pressing the respective buttons. 
 * NOTE:: the notes played are NOT stored
 * 3: record(): this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * 4: play(): this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * 5: loopMode(): this function will playback any notes stored in the array that were recorded, 
 * BUT unlike the previous mode, you can choose in which sequence the notes are played.
 * REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/
void selectMode()
{
  if(mode ==0) { 
    reset();
  }
  else if(mode ==1) {
    live();
  }
  else if(mode ==2) {
    record();
  }
  
  else if(mode ==3) {
    play();
  }
   
   else if(mode ==4) {
    loopMode();
  }
}
/******************RESET(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
  // for each note 
  for (int i = MAX_NOTES-1; i >= 0; i--){
    // remove the note and change it to 0
    notes[i] = 0;
  }
  // reset the counter
  countNotes = 0;
  
}
/******************LIVE(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * NOTE:: the notes played are NOT stored
 * SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * THEN - output the note to the buzzer using the tone() function
**************************************************************************/
void live()
{
    // ring the buzzer with the frequency of the button-resistor ladder combo
    tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), duration);
}
/******************RECORD(): IMPLEMENT **********************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * SO:you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * AND - output the note to the buzzer using the tone() function
 * THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
**************************************************************************/
void record()
{ 
  // Get the state of the button (HIGH (pressed) or LOW (not pressed)) 
  int button = analogRead(NOTE_IN_PIN);
  // if the counter is smaller than the maximum notes allowed
  if (countNotes < MAX_NOTES) {
    // ring the buzzer with the frequency of the button-resistor ladder combo
    tone(BUZZER_PIN, button, duration);
    // if the frequency is bigger than 0
    if (button > 0) {
      delay(600);
      // plug the frequency in the array
      notes[countNotes] = button;
      // add one to the counter
      countNotes++;
    }
  }
}
/******************PLAY(): IMPLEMENT ************************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ each value IN ORDER
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play()
{
  // for each note in order
  for (int i = 0; i < MAX_NOTES; i++){
    // if the mode button is not pushed
    if (digitalRead(BUTTON_MODE_PIN) != HIGH) {
      // ring the buzzer with the frequency stocked in the array
      tone(BUZZER_PIN, notes[i], duration);
      delay(200);
    }
    // if the mode button is pushed
    else {
      // exit the loop
      return;
    }
  }
}
/******************LOOPMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void loopMode()
{
  // for each note in reverse order
  for (int i = MAX_NOTES-1; i >= 0; i--){
    // if the mode button is not pushed 
    if (digitalRead(BUTTON_MODE_PIN) != HIGH) {
      // ring the buzzer with the frequency stocked in the array
      tone(BUZZER_PIN, notes[i], duration);
      // generate a random delay between each note
      delay(random(200, 300));
    }
    // if the mode button is pushed
    else {
      // exit the loop
      return;
    }
  }
}

/**************************************************************************/
