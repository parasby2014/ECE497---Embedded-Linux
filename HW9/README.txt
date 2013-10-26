

matrixLED:

1.Whevever an action is taken on the browser, information is sent using socket.emit. After connect() is called, 'matrix' is sent to the bone and then the browser reads the current values on the LED matrix, converts, and then displays them. From there the browser waits for clicks and depening on where the use clicks, information is transmitted to the bone and to the browser. 

2. When an 'LED' is clicked, the LEDclick function in matrixLED.js is called and updates the LED by adding  or removing a class called 'on' which sets the color of the LED. 

3. the 'on' entry calss is used to color the LED's green. 

4. I will control both LED's by adding two more classes for green+red and red. These are going to be define in the matrixLED.css where each class will have a specific 'background color' set. From there I will modify the LEDclick function to check the current class. If the current class is green, remove that class and add the red+green class. If the current class is red+green, remove it and add the red class. If the current class is red, remove it to turn off all LEDs. Otherwise add the green class to turn on the green LED. It's a little blunt, but it works. 

5. Code is implemented and checked. Look at matrixLED.js and matrixLED.css to see changes. For some reason I need two clicks for the colors to switch and turn on. I'm not quite sure why, but either way, it works. 
