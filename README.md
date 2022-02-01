# esp8266-sds011-dht22
Some code to connect an SDS011 and DHT22 to an ESP8266

You can configure whatever GPIO pins you want in SerialCom.h, but I used a KeeYees D1 Mini Knockoff and connected the SDS011 TX pin to D3 and the RX pin to D4. The DHT22 data pin is connected to the D1 pin and I've connected a 5K1 pullup resistor between the data pin and 3V3 as described in the DHT22 documentation.
