# arduino-based-E-Skateboard
arduino based controller and receiver for DIY electric skateboard

E-Skateboard Control code. 

The system consist of 2 arduinos, both communicating using UART over bluetooth. 

The system works in a very simple manner, to minimize risk and bugs. 

The wii nunchuck controller is modified and its analog stick as well as its 2 buttons are connected to the arduino. The arduino will read the analo input of the analog stick and map it into numbers ranging from 1100 to 1800. Then it will print that number over serial for the other arduino to receive.

The sk8 controller will parse and filter the receiving number and feed it into the ECS. The number represents time in microseconds. This time is used to control the pwm signal of the ESC. The ECS is idling at 1100 μs and at full throttle at 1800μs. 
