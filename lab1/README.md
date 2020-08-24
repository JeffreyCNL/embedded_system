Task1.
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
Task2.
For the second task, I turned off all the LEDs first
as well. After that, my program enables me to wait
for the user to push start/ stop button. By pushing
it, my LEDs will shine between red (stop state)
and green (go state) forever. By pushing pedestrian
button, if it’s in stop state it won’t happen anything
but since it shine between two colors quickly we
can’t feel it. However, by pushing pedestrian button
at go state, the green LED will turn off > yellow on
> yellow off > red on. If I keep pushing it, the red
LED will keep on since it is the time for pedestrians
to pass, not the cars