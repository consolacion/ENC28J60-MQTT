# ENC28J60-MQTT
Use of an ENC28J60 shield (or module) with MQTT PubSub


The program uses the old ENC28J60 shield on a UNO or  an ENC28J60 Module on say an Arduino proMini to send MQTT messages.
The ENC28J60 uses a lot of memory so little is left for programming. Therefore this program uses short strings and only publishes MQTT, it does not subscribe to any MQTT

Some memory can be gained by setting the UIP_CONF_UDP value in the UIPEthernet.h library to '1'

There is a DHT11 attached to pin D3.
The state of D4-D12 is published  via MQTT
The value of A0-A3 is also published via MQTT
the value of A2 is  made a float and shows how to send data from a battery that is attached

The deblanking function is there to strip spaces of the numbers being sent, so a receiving program can use them as numbers again.

The IP number as well as the MAC are also published, as is the uptime.

the publish delay is 20 seconds but can easily be adapted by the value of "PUBLISH_DELAY"
The MAC address is made to be 00:01:02:03:04:05 but can easily be changed
