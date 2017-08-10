# BlueHourMaquette
Code and other relevant files for a maquette of New American Public Art's project, Blue Hour

## Fade_With_Motion.ino
This is the code that runs the brain!

It has a few main functions:
* global variables
* setup
* loop
* checkAndFade
* fadeUp
* fadeDown

### Global Variables
You can view all of them in the code itself, but the ones to watch out for are dataPin, clockPin, motionPin, and analogPin. As you an guess by the names, these correspond to input pins on the arduino. It is important to make sure these match up with your circuit, otherwise it won't work (either correctly, or at all).

It is also important to note that the colors array is declared in the global variables. The size of this array is what constrains the maximum amount of totes you can have in a stack-- currently that maximum is five. If you want to add more, simply append additional "0,0,127"s for each tote to the end of the array. It will set them blue to start. You will also need to change the variable "maxTotes" right after it to the correct maximum, as there is a check for that later in the code.

### setup()
This is the setup function for the arduino sketch. In order, it:
* declares motionPin as an input (so that the arduino will read from the photoresistors)
* reads analogPin (so that the arduino reads the resistance from the stack of totes, to determine how many there are)
* calculates the number of totes from the total resistance
* turns all the connected leds blue to begin

### loop()
This is the function that calls all the other code, after the arduino has been "setup." This also does many things. In order, it:
* tracks time
* calculates the number of totes (That way, if the number changes while it's running, the stack will still function as desired)
* reads the input value from the photoresistors
* updates the maximum amount of light, based on whether the photoresistor value is higher than the current max (**Note: This does not ever decrease, so if you were to shine a bright light at the photoresistors only once, it would assume that was the default light state in the room. This can cause problems because it will assume there is motion at a lower light state, though you may have just turned that light off.**)
* checks for motion by seeing if the light state is lower than the max light state, and sets a boolean to true or false accordingly
* calls checkAndFade to update the rgb values for the leds in a for loop
* updates the pixel colors with the new rgb values in a for loop
* "shows" the colors on the strip

### checkAndFade(int timer, int tote)
This function checks whether the motion boolean (global variable bool motion) is true or false, and updates the rgb values of the leds accordingly. It takes timer as an input so that it only updates every 10 milliseconds, which was a fade time we liked. It takes tote as an input because that number corresponds to the tote that is being updated.

It calls fadeUp or fadeDown depending on whether motion is true or false.

### fadeUp(int colors, int shift)
This function checks the rgb values stored in the colors array (which is passed to the function) at a specific point and updates them to be more yellow or red, depending. The specific point is determined by the shift variable (also passed to the function), which corresponds to the tote being updated. This is necessary because each tote has three variables in the colors array. For example, tote 1 has colors[0] as its red value, colors[1] as its green value, and colors[2] as its blue value. Tote 2's rgb values begin at colors[3], tote 3's at colors[6], and so on.

After the colors array is updated with new rgb values, it is returned at the end of the function.

### fadeDown(int colors, int shift)
This works almost exactly the same as fadeUp, but it fades the rgb values back down to blue.
