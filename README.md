# Arduino-based-E-Skateboard

Arduino based controller and receiver for DIY electric skateboard

The system consist of 2 arduinos, both communicating using UART over bluetooth. 

For my particular use, an arduino pro mini is used for the nunchunk controller and an arduino pro micro for the sk8 controller

The system works in a very simple manner to make life easier. 

The wii nunchuck controller is modified and its analog stick as well as its 2 buttons are connected to the arduino. The arduino will read the analog input of the analog stick and map it into numbers ranging from 1100 to 1800. Then it will print that number over serial for the other arduino to receive.

The sk8 controller will parse and filter the receiving number and feed it into the ECS. The number represents time in microseconds. This time is used to control the pwm signal of the ESC. The ECS is idling at 1100μs and is at full throttle at 1800μs. 

The code also has external lights support, as well as both skateboard and wii controller battery monitoring capabilities. There is also an accidental trigger protection as well as disconnection protection.

# Small issues with the code #

# sk8 Controller:
The parseInt() function used to parse the int number from the incoming serial port can sometimes slow down the code. This is because of the way parseInt() works, which is to look into the incoming buffer and read any number it find until it reaches a character, in the case of this code, the new line character. For some reason, this process will bug if the small delay in the main code is removed. A better way to read incoming data and then filter just the int number might be a better solution 

The Serial1 used in the code is because I am using the arduino pro micro, which has 2 Serial ports. The default, Serial, is used when working over the USB connection, while Serial1 is the one used for the hardware tx-rx pins. 

# Nunchuck Controller
The code to monitor the voltage of the battery of the arduino in the nunchunk, is using the 3.3 voltage of the arduino as reference. If you are powering the arduino directly with a 3.7v lipo battery using the on-board voltage regulator, the voltage output of the regulator will drop below 3.3v while the battery discharges, making the reading inaccurate. If you are using this approach for power, a better way to monitor the battery would be to use the internal 1.1v reference voltage for the ADC, and use a voltage devider to get the battery level (3.3v - 4.2v) down to the 0v - 1.1v range. BE AWARE that this will also affect the reading of the analog input of the joystick, as you will also be limited to a maximum of 1.1v for the lowest point of the joystick.
