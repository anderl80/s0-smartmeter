# s0-smartmeter

This repo shows how to using 
- a Raspberry Zero W
- a ESP8266
both used with (the same) IR sensor to log the output of a $S_0$ interface of a power meter.

# Known issues

- Notebook is in German but will be edited.
- ESP8266 can only used in always-on mode (need to count IR pulses all the time), that drowns my battery pack in less than 3 days. Solution to this could be to deploy the counting to an attached device, push that to the ESP8266 from time to time, while it's activated, receives values, pushed to MQTT and goes back to sleep.
