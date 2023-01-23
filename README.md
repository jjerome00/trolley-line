# trolley-line
arduino program to control a G-Scale model trolley with a motor controller and ir break beam sensors

## Expected behavior
GIVEN a straight track with ir sensors on either end   

WHEN user presses button   
THEN trolley will travel to other side of track   

WHEN trolley breaks the ir beam on that side of track   
THEN the trolley will stop   

WHEN trolley moves for 25 seconds
THEN the trolley will stop
(in case of ir beam failure)

WHEN the user presses the button (again)   
THEN the trolley will travel towards the other side of track    
(repeat)   


## Dependencies

1. Adafruit MotorShield
2. 2 IR Break Beam Sensors
3. 1 physical button with a built-in LED
4. A G-Scale Model trolley, and track

