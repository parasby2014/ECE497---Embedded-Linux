

matrixLED:

1.Whevever an action is taken on the browser, information is sent using socket.emit. After connect() is called, 'matrix' is sent to the bone and then the browser reads the current values on the LED matrix, converts, and then displays them. From there the browser waits for clicks and depening on where the use clicks, information is transmitted to the bone using socket.emit and i2cset. The LED registers are modified accordingly to where the click occurs. 

2. When an 'LED' is clicked, the LEDclick function in matrixLED.js is called and updates the LED by adding  or removing a class called 'on' which sets the color of the LED. The corresponding LED registers is also toggled. 

3. the 'on' entry calss is used to color the LED's green. 

4. I will control both LED's by adding two more classes for green+red and red. These are going to be define din the matrixLED.css where each class will have a specific 'background color' set. From there I will modify the LEDclick function to check the LED registers and set the colors accordingly in the pattern green, yellow, red, off. The program ignores the red LED registers, so I need to adjust it to read all the registers. boneServer.js also needs to be adjusted to do this as well. 

5. Code is implemented and checked. Look at matrixLED.js and matrixLED.css and boneServer.js to see changes.
