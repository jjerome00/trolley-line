

---
## Music
Music requires power, a "button" pin, and ground (pin + ground activate the button)
Power is supplied via usb connection - easiest way since it can get pure 5v

The Music board connects its 0pin to arduino pin 12 
The Music board ground connects to arduino ground

note: I mistakenly used pin 13 on arduino in the past - big mistake 
That's the pin arduino uses for its own internal led mounted on the board for testing

Music Board:Ground -> brown-wire > arduino Ground
Music Board:0pin (music) > orange-wire > arudino pin12
Music Board usb-connection: 5V power supply (old phone charger)

Speakers use separate USB power and plug into music board via 3.5mm jack

---
## Motor Controller
Motor Board:5v > orange-wire > arudino 5v
Motor Board:Gnd > purple-wire > arudino Gnd
Motor Board:SCL > green-wire > arudino SCL
Motor Board:SDA > yellow-wire > arudino SDA

---
## Senors
9/2024: I switched the break-beam sensors for a limit switch. 
The same logic works except the limit switch does not need power. 
I only use the ground + pin

Right side (or Pin One in code)
Arduino pin4 > yellow-wire > White-wire for Right side (in bundle)

Left side (or Pin Two in code)
Arduino pin2 > white-wire > White-wire for Left side (in bundle)

Arduino 5v > red wire > [block terminal] > to the senors + button (you must share!)
Arudino Gnd > black (via marker) wire > [block terminal] > to the sensors + button (you must share!)

---
## Button
Arduino 7: button pin (starts trolley)
Arduino 8: button LED 
Arduino Ground

