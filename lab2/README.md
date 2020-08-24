# Timer and Interrupt
# Procedure
For the first task, I configure the timer as the lab
instruction and set GPTMTAILR register as 16M so
that on-board led will shine in 1 Hz. In my polling
function, I check GPTMRIS and XOR with it. This
will simply wait for the assigned time. After it’s done,
it will jump to another color again and again. The
traffic light problem is a little trickier. I set two
timers. One is for 2 sec, and the other one is for
5 sec. In order to generalize the program. I let the
user to type in how many seconds they want to stall.
For the stop and start problem, I set a while loop
within check push time function in front of the while
forever loop. It will check if the button have been
push for more than two seconds. After jump in the
traffic light loop, I checked if the pedestrian button
is pushed. If it is, timer1 will turn on and see if it’s
over 2 seconds and if it’s over 2 seconds, it will go to
the warn state. Clear all the timer and call the traffic light function itself, it will loop forever. For the
second task, I configured the interrupt just like the
lab instruction. I put a Timer Handler in cstart M.c
file. This way we can enable the interrupt. In my
Timer Handler, I simply used a global variable called
flags to change the state and clear the interrupt. The
colorful function is implemented with switch that will
read the flags to choose what color to shine. For task
2.2, I set GPIO PortF Handler in the cstart M.c file.
The interrupt number is 30. The difference in this
lab is that when I initialize the user switches, I have
to enable the interrupt as well. In the interrupt handler, I masked interrupt state register to test if the
switch has been pushed. If switch two is pushed, it
will stop the timer for shining the blue led and turn
into red. Push switch one to resume the shining blue
led. The blue led is shining in 1 Hz based on timer0
which is set as 1 second and just polling. For task 2.3,
I enable GPIO PortA Handler in the dot c file. The
interrupt number is zero. In the interrupt handler
function, I test if the button has been pushed by the
masked interrupt state register. For the timer test,
I only used polling timer functions to poll for 2 and
5 second respectively. The traffic light function also
work as global variables called flag. The interrupt
handler can deal with the flag and therefore, change
the state. The button port and the flag states are
configured as followed.

|LEDs / Buttons        |Port |
|----------------------|-----|
|Start/Stop            |PA5  |  
|Pedestrian Button     |PA6  |  
|Red LED(Stop State)   |PA2  |  
|Yellow LED(Warn State)|PA3  |
|Green LED(Go State)   |PA4  |

State and flag configuration:
|state       |flag|
|------------|----|
|stop(red)   |1   |  
|go(green)   |2   |  
|warn(yellow)|3   |

# Results
## Task1.1
I can shine all the seven colors by polling the timer
in 1 Hz, which is what the task for.

## Task 1.2
I can for the stop/ start button to push more than
2 seconds. Only if it is pushed more than 2 seconds,
we can turn on the traffic light system. The system
will shine between red and green by 5 seconds period
for each color. In this system, we keep checking if the
pedestrian button is being pushed. If we pushed it
more than 2 seconds, it will immediately jump to the
warn state(yellow) and stall for 5 seconds to jump
into stop state.

## Task2.1
I can shine the on-board led in 7 colors through interrupt handler. The output will be the same as task
1.1. But the implementation is different and also I
believe the performance will be better since we don’t
have to poll the timer every time.

## Task 2.2
I can shine the blue led for 1 second and turn it off
for another one second. If I click user switch 2, the
timer for shining led will be turned off and the led
will turn into red. I can click the user switch 1 again
to resume the state.

## Task 2.3
My system will wait for the start/ stop button to be
pushed over 2 seconds. After pass the check point, it
will start shine through red and green by 5 seconds
respectively. If we push the pedestrian more than 2
second. It will turn on the yellow led for 5 seconds
and jump back to the stop state. The output is the
same as Task1.2 but the implementation is different.
