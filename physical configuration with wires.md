

Music
Board:Ground -> brown-wire > arduino Ground
Board:VIN > red-wire > arduino VIN 
Board:0pin (music) > orange-wire > arudino pin13

Motor Controller
Board:5v > orange-wire > arudino 5v
Board:Gnd > purple-wire > arudino Gnd
Board:SCL > green-wire > arudino SCL
Board:SDA > yellow-wire > arudino SDA

Senors
note: sensors share their 5v + Gnd
The 5v + Gnd + Left (signal) + Right (signal) are all fed into a single 4-block terminal

Right side (or Pin One in code)
Arduino pin4 > yellow-wire > [block terminal] > White-wire for Right side (in bundle)

Left side (or Pin Two in code)
Arduino pin2 > white-wire > [block terminal] > White-wire for Left side (in bundle)

Arduino 5v > red wire > [block terminal] > to the senors + button (you must share!)
Arudino Gnd > black (via marker) wire > [block terminal] > to the sensors + button (you must share!)