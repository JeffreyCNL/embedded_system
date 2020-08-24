# Traffic Light System
# Procedure
The first thing I approached to this lab is to understand how the register control the hardware. I found
it hard to control the precise register. However, I
think this lab is pretty nice to us since it basically
tells us what registers do we need to use and after
digging down into the header file, I found every register value is defined in a reasonable way since everything is included in the data sheet. After found
out the big truth about this lab, I think the rest will
be connect the hardware. The circuit diagram for
LED is pretty friendly and is easy for a mechanical student to connect. For the first task,I defined
4 functions. portf init, wait are two functions that
are pretty self-explanatory. I simply created another
function called colorful and it will shine 7 colors. I
also utilized user switches function to check if the
GPIO PORTF DATA to see if the user switches are
being pressed to make LED go red or green. For the
second task, I followed through the external button
and LED code. I used Port A 2 to 6 to control 3 LED
and 2 buttons separately. After LED and switch init
functions, I created a traffic light function that help
me to design the finite state machine. In that function, I have three LEDs on and off functions. The
following table describe how I designed.

|LEDs                  |Buttons Port   |
|----------------------|---------------|
|Start/Stop            |Button PA5     |  
|Pedestrian            |Button PA6     |  
|Red LED(Stop State)   |PA2            |  
|Yellow LED(Warn State)|PA3            |
|Green LED(Go State)   |PA4            |
 

# Result
## Task1.
For the first task, I turned off LED first to avoid
previous work by EEPROM. I was able to shine 7
different colors by combining red, green, and blue as
the assigned task wanted. Also, I used user switch
1 to turn the LED to red and user switch 2 to turn
the LED to green. However, those switch functions
only work after it shine through the 7 colors above,
which means I don’t use interrupt to immediately
turn the LED to red or green. Tao told me that it
is my design and we will be taught about interrupt
function in the near future.
## Task2.
For the second task, I turned off all the LEDs first
as well. After that, my program enables me to wait
for the user to push start/ stop button. By pushing
it, my LEDs will shine between red (stop state)
and green (go state) forever. By pushing pedestrian
button, if it’s in stop state it won’t happen anything
but since it shine between two colors quickly we
can’t feel it. However, by pushing pedestrian button
at go state, the green LED will turn off -> yellow on
-> yellow off -> red on. If I keep pushing it, the red
LED will keep on since it is the time for pedestrians
to pass, not the cars