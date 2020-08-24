# Final Project: Intelligent Parking Garage
# Problem
For our Lab 5, the problem we wanted to address was implementing a control system for a smart parking
garage. Smart parking garages increase convenience and decrease staffing costs for parking. Most simply, they automatically detect vehicles and open the gate. At a more complex level, they can keep track
of which vehicles are coming in to the garage, how often they are parking, and determine which vehicles
are allowed to park in the garage. For the purposes of this lab, we decided to focus on the embedded
systems portion of this control system, namely detecting vehicles and opening the garage gate.

# Procedure
We decided to implement this using a HX-3CC servo motor and OV7670 camera [4] on our TM4C Microcontroller [2]. We planned on capturing an image with the camera and processing the image with a
machine learning library to detect if there is a vehicle in the image, then using the TM4C Pulse Width
Modulation (PWM) module to control the motor to open the parking garage gate. Xnor.ai provides numerous machine learning libraries for embedded systems applications with pre-trained machine learning
models to detect vehicles in images, which suited our purposes perfectly [5]. This could then be expanded upon with other machine learning libraries to detect things like licence plate numbers to allow
even more specific management of the parking garage. For the purposes of demonstration, however, we
3D printed a model parking garage and attempted to detect the shape of model cars in the garage.
The OV7670 camera uses Serial Camera Control Bus (SCCB) protocol [3] to communicate, which
is compatible with the Inter-Integrated Circuit (I2C) protocol on the TM4C board. We then needed to
provide the camera with a clock input to drive the camera system clock (XCLK). We used another PWM
module to create a clock input for the camera module. However, the camera needs a 10Mhz - 48Mhz
clock input [4] and the TM4C board can only modulate the PWM module at the rate of its internal system
clock. This means that the TM4C can only produce a clock with PWM with a maximum frequency of
one half its system clock frequency. We configured the TM4C Phase Locked Loop to run the system
clock at a frequency of 66.67Mhz so we could modulate the PWM every 2 clock cycles and produce an
output clock of 16.67Mhz for the camera to use as input.
The Xnor.ai image processing library requires a minimum of 300x300 resolution to be able to process
an image and detect if it contains a vehicle [5]. So, we used the TM4C I2C module to interface with
the OV7670 camera SCCB protocol and set the camera’s registers and configure the camera to capture
images at a VGA resolution of 640x480 [4]. We could then connect the camera’s vertical synchronization
(VSYNC), horizontal reference (HREF), and pixel clock (PCLK) outputs to the TM4C board and use
those to detect when to capture data from the camera’s 8 data output pins. A VSYNC falling edge
indicated the camera is outputting a new image frame, and a HREF rising edge indicates the camera
is outputting bytes from a new row of pixels. While VSYNC is outputting low and HREF is outputting
high, the camera outputs a new byte of image data over its 8 data pins with every tick of its PCLK output.
By setting interrupts for the VSYNC and HREF edges, we could then use an interrupt on the PCLK tick
to capture the 8 data pin outputs and store that as a byte of image data.
We could use a TM4C General Purpose Timer (GPTM) to sample the camera every 1 seconds and
process the image data with the Xnor.ai library to detect if there was a vehicle in the image. If the machine
learning library outputted that there was a car in the image, we could modulate the PWM connected to
the servo motor to 80% of its previous period which would raise the gate to the parking garage and start a
one-shot GPTM timer. After 5 seconds, the GPTM timer would set the PWM back to its original period
which would lower the gate again.

# Results
Although our final system involved a lot of complex interactive components in terms of Timers, PWM
modules, and interrupts, it remains fairly simply in terms of its FSM states. The entire system has only
two tracked states: gate open or gate closed. The majority of other components involved detecting or
implementing the state transitions.
    We made a lot of modifications to our design (both large and small) throughout the development process
and iterated through many different implementations through the process of research and experimentation. Although we were not able to implement our original design, we demonstrated that a variety of
approaches can be taken to obtain the same result (to a varying degree of specification). We implemented
the an embedded systems control portion of a smart parking garage at a model scale and demonstrated
that through this modular engineering approach, various features can be added or swapped to obtain
a final product. We demonstrated this by developing our servo motor controller and vehicle detection
components in parallel and then combining them at the end. Even though the vehicle detection component needed to be completely redesigned, the servo motor controller was still able to be used exactly as
originally designed.
We could use a similar approach to expand upon our product and add components for web interfacing,
databases, or coordination between multiple gates to implement a smart parking garage in a modular
fashion to improve ease of use.

# Acknowledgement
This final project is cooperate with CSE student Daniel Sullivan,
with me handling the PWM module and he handled the Ultrasonic sensor module.

# Reference
[1] Elec Freaks. Ultrasonic ranging module hc-sr04. https://www.electroschematics.com/wp-content/uploads/2013/07/HCSR04-datasheet-version-1.pdf.
[2] Texas Instruments. Tiva tm4c123gh6pm microcontroller data sheet. http://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf, June 2014.
[3] OmniVision. Serial camera control bus functional specification. http://www4.cs.umanitoba.ca/~jacky/Teaching/Courses/74.795-LocalVision/ReadingList/ov-sccb.pdff, March 2002.
[4] OmniVision. Ov7670/ov7171 cmos vga (640x480) camerachip sensor with omnipixel technology.http://web.mit.edu/6.111/www/f2016/tools/OV7670_2006.pdf, August 2006.
[5] Xnor.ai. Xnor model bundles. https://docs.ai2go.xnor.ai/model_bundles.html.