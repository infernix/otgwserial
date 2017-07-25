# otgwserial
Talk to an OpenTherm OTGW gateway using non-standard GPIOs using ESP8266

The OTGW is sensitive to garbage on its serial interface and the ESP8266 spits out quite a bit at bootup. TX/RX pins on the ESP were therefore unusable, and alternative GPIOs did not work for me either since the level converter keeps GPIO15 high which engages the ESPs sdcard(?) boot mode.

The only working solution that did not need an OTGW reset when powering both up at the same time was software serial on arbitrary GPIOs.

Sketch includes a config.h.example that can be adjusted for your situation.
