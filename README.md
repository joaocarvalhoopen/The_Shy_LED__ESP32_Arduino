# The Shy LED – O LED envergonhado

Yesterday late in the night, I finally finished The shy LED, or in Portuguese “O LED envergonhado!”<br>
This is a system of “high technology!”  hehehehe :-D<br>  
Just kidding it’s a simple little project.<br>

## License:
MIT open source

## Description

This Arduino ESP32 project has a LED that is always on, even when the light of a chandelier is over it. But it turns off when you shine a flash light in the direction of the LED, he becomes shy and turns off. After that, if you turn the flash light in other direction it turns on immediately. The LED flash light that I used while developing this little project was a 2 euro flash light, nothing fancy.<br> 
<br>
This little project was inspired by a story that in some part of the world there is a super hero, a men with a super power, he can become invisible! But only when nobody is looking at him!<br> 
Here is almost the same thing. This is a LED that gives light, but that when you shine a flash light in is direction it turns off the LED light! You turn the flash light in other direction it turns on again. So he is a shy LED :-D   I do little electronics boxes with my little daughter.<br>

![PNG project schematic](/The_Shy_LED_schematic.png)


## Now you probably are asking how all this works, right?<br>

Very well, for a long time it is known that an LED can be used has a photo detector, a light detector. It appears in the Mint books, and he uses in his rockets for its stability along the years. What I have made was a twist on this concept with a microcontroller.<br>
<br>
Yesterday night, I was at my microelectronics lab at home,  in the Workbench looking at the landscape 20 cm distance, when I looked at the bench DMM and thought that would be fun to test which proprieties of the LED (voltage and current) changed when the LED was connected to a power supply and I shined a flash light with white light in his direction.<br>
<br>
I only had a LED, a resistor and a power supply.<br>
In reality in the DMM there was no perceptive change, although it had 6 1/2  digits!<br>
<br>
I then decided to disconnect the wires, that is, to put the circuit in high impedance (pins in the air), then connect the DMM to the LED and shine the flashlight. I saw a voltage potential that rose from 0V to 60mV. If I turn the flash light in other direction it doesn’t came to zero volts because there is ambient light but it reduced to almost zero.<br>
<br>
I thought, 60 mV, I can measure with the ADC of ESP32 (from my experience if I do some averaging I can have a 1mV resolution), more info below.<br>
<br>
And after that I thought that I could do something funny with this, I could change between two modes very rapidly at over 25Hz to have persistence of vision (POV). I could change between having the LED on and measuring the flash light shining on the LED. In this way the LED looked always on. I Could have the LED on for 20 milliseconds, after that turn the LED off and change the circuit to high impedance (Hi Z), waited for 10 milliseconds, so that the light from the flashlight charged the LED capacitor with a potential that I could measure with the ESP32 ADC. 
When I tried to pass the theory to the practice I encountered two problems. One of them was the fact that the ADC of the ESP32 is “crap”, because it not only isn’t linear and it’s range doesn’t start at 0V and ends at 3.3V. It works only on an intermediate range.<br>
<br>
But because every now and then we all have to put our makers hat and come up with a hack, finding a solution that is a little uncommon or out of our comfort zone. I thought a little about it and did exactly that. That is it would be a simples thing if it was other microcontroller with a linear ADC that starts on 0V but with ESP32 I would need to spray a little bit of extra magic powder over it! Hehehhehe<br>
<br>
But I don’t mind, this kind of things is what makes this project fun!<br>
<br>
The circuit has two states of program and of electric circuit. The circuit mutates itself electrically, one mode when the LED is turned on, in which the pin 12 LED power is giving the current (OUTPUT 3.3 Volts) and in that pin 25 Virtual Ground is working like a real ground (OUTPUT 0 Volts).<br>
<br>
The other electric mode is when you are measuring the potential in the LED, caused by the fact that he is turned off and we are shining a LED flash light in his direction. In that case pin 12 LED power is high impedance, pin 25 Virtual Ground  is high impedance and pin 27 Half Power offset is turned on, this makes a voltage divider that makes possible that the LED charges with 60 millivolts but with an offset 610 millivolts. Well inside the ADC measuring range.<br>
<br>
Vout = R2  /  (R1 + R2)  * Vin       [Voltage divider]<br>
<br>
This state commutation occurs more than 25 times a second (> 25Hz) and because of this we have the effect of persistence of vision (POV) and the LED looks like it is always on, when in reality it isn’t.<br>
<br>
In the fairy tale world, this should be enough to make it work, but because we leave in the real world and there are exogenous factors that influence a system has delicate has this one. Don’t forget that we are talking of a normal LED having a capacitance in the order of 1 a 3 pF (Pico Farads 10E-12 ), so I had to make 3 things in order it to work, and now it works flawlessly (The devil is in the details):<br>

* I had do add a delay of 10 milliseconds after the pins modes were correct to measure the drop of voltage across the LED so that the capacitor is charged by the light of the LED flashlight and I could measure it in the ADC.

* Did I already sad that the ADC of the ESP32 is “crap”? Such a good chip, the ESP32, and with an Aquila's like that in the ADC, what a shame.  In order to be more precise, because it oscillated a lot I had to make an average of 40 samples of the ADC for each measure. With this the high frequency oscillations were dumped.

* Because there was a low frequency high oscillation between cycles, that appear perhaps to be correlated with the timing variations of the Arduino delay (not using timers) or with the serial communications, that was making it impossible to apply a simple threshold to determine when the flash light was shining on it or not and with the chandelier or day light was over the LED.
Because of this I decided to make a running average of the last 3 values, it didn’t work, then 5 values, then 10, then 15 and finally I fine-tuned it to the running average of the last 20 values as the ideal value, so that it makes the signal smooth.  It increased the lag, but it wasn’t significant, and become imperceptible.  

In all the process of fine-tuning the serial port monitor and the plotter monitor were the key components, to make the fine tunings and at 2 in the morning I had the circuit and the firmware stable and really fine-tuned!<br> 
I recommend that you turn on the plotter monitor in the Arduino IDE to see the graph of the running mean changing of the light detector of the LED, while changing the light sources intensities. It makes nice curves and not only on and off values.<br>
<br>
From what the little that I researched, I didn’t see any circuit like this one, feel free to use it as you desire the license it MIT Open Source license. I used a 3mm green LED but it should work also without tuning with a redo r a yellow LED.<br>
<br>
Have fun!<br>
Joao Nuno Carvalho<br>



