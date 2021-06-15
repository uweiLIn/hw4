# hw4
In Q1 the car parking I divided the whole process into three parts. First, move the car to the extended front edge of blank, turn the car and move into the blank. Most of the code modification are in Bbcar_rpc.cpp. By adding a new rpc function call park with parameters two distance and direction.

In Q2 Line Detection, most of processing is done in openMV. Throught out the regression, threshold value setting of color and redefination of coordination. Passing the rho and theta to pid.py to calculate the output, which passed by uart to mbed then change the direction of car.

In Q3 apirltag, I simply copy openMV code from lab14 last sub-experiment. Through uart, read char by char, discard the value after decimal point. In certain angle range, the car will turn. After that, thecar go straight for certain distance. Using ping to check the distance pass the value to PC.

Result :

1. the car will park in blank
2. the car will remain go straight before line end. However, since it close to end of line camera may not scan some part of line. Therefore, it wont "100%" go through the end.
3. the car will scan apurltag and change the direction, then go straight
