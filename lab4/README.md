# LCD display
# Procedure
## Task 1
Task 1 was fairly straight forward since it pseudocode was already laid out for the function in SSD2119.c.
The only thing we needed to do was look up the values for the indicated registers in the data sheet. This
got our LCD screen successfully running and displaying images with LCD_ColorFill and the LCD_Draw
functions.
## Task 2
Task 2 was also simple because we could reuse a lot of our code from Lab3. All the ADC initialization
and reading was the same as Lab 3 Task 1. The only difference we made was modifying the print
functions in SSD2119.c to print the resulting temperature readings.
## Task 3
To begin Task 3, we first set up red, yellow, and green circles with rectangular screen buttons below
them for the traffic light and traffic light controls. This was fairly simple using the LCD_Draw and LCD
printf functions. The main challenge came from reading user input to respond to the buttons. We used
a similar format as the Lab3 traffic light, with a 5 second timer interrupt switching between the states
and a 2 second timer interrupt triggering if the user was still holding down the button for 2 seconds. The
main difference is that instead of starting the 2 second timer on a button interrupt, we had to continuously
check if the screen was being pressed and, upon being pressed, detect whether the user was pressing a
button.

# Results
## Task 1
Our LCD_GPIOInit function successfully allowed us to set up the LCD and print to the screen, fulfilling
Task 1 requirements. Since this was just an initialization function, it did not have any state transitions.

## Task 2
Our Task 2 implementation successfully prints out continuous temperature readings from the board’s
internal thermometer and fulfills the requirements for Task 2. Since it is always in one continuous state
of printing temperature readings, I did not include a state diagram.

## Task 3
Task 3 successfully displays traffic light lights based on its state and presents 2 user buttons on the screen
that the user can change the state with. The main difficulty comes from reading the position of user
clicks on the screen. The LCD driver code provided for reading the position of screen touches is fairly
convoluted and provides inconsistent readings so sometimes several attempts are needed to get a button
to activate. Other than that difficulty, the program successfully switches between STOP and GO states
every 5 seconds, will set the lights to WARN upon pressing the pedestrian button, and will only activate
buttons upon holding them for 2 seconds.

# Acknowledgement
This is Lab is cooperated with CSE student: Daniel Sullivan.