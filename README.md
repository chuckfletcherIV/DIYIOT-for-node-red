# DIYIOT
Open source IOT hardware and software platform based on cheap commodity sensors, ESP8266-12e chips, the MQTT protocol and powered by IBM's Node-Red (pre-installed with most new raspberry Pi 3 installations).

The goal of the project is to put sensors (and actuators like servos or leds) on a local wifi network as inexpensively as possible, ideally for under $7 or 8 per device (including a power supply). 

Using Node-red, these devices can trigger or be triggered by any IOT activity you can think of. Node-red has dozens of supported IOT hooks to services and devices, like Hue or Lifx light bulbs, twitter integration or even the ability to send txt messages via twilio.

No coding is required for basic functionality. All data is sent to Node-red for processing and triggering. Node-red lets you do alot without coding as well, but if you need to code, you can do what you need to with simple javascript. 

DIYIOT is great for home automation projects, DIY escape rooms and halloween displays, and even interactive installations. For more permanent installations, sensors can be soldered directly into the board, vs using a header (which makes the sensor swappable). 

See DIY Board and Sensor Pin Map to figure out what board to use for which sensor (or actuator), also see notes for more advanced usage.

You can use www.PCBGOGO.com to get your own boards made for under $1 each. Gerber files are available for download from our site under "PCB". Keep an eye out for other sources for DIYIOT boards over the coming months. 

If you find a cheap commodity sensor that works great with an exisiting board - let us know so we can add it to the list.

If there is a problem with a sensor firmware code, or pin map, let us know or share a fix.

Future project goals:

Automatic Device Discovery with mDNS (bonjour)

SSL Level Security 

Smaller board size

Long term power and battery opimization

Even more cost reduction per device if we can find engineering and manufacturing partners to work with. 

Thanks, 

the DIYIOT team

Join the DIYIOT for Node Red mailing list at https://groups.google.com/forum/#!forum/diyiot-node-red
