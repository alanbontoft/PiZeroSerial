/*
 * Program to control up to 24 relays from Pi Zero
 * Uses mosquitto and wiringPi libraries
 * MQTT message payload is 2 bytes, channel (1 - 24) and relay state (0 = off, any other value = on)
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

#include <wiringPi.h>

#include <wiringSerial.h>

int main(int argc, char *argv[])
{
	int ret = 0, fd, count = 0, ready;
	char ch;

	ret = wiringPiSetup();

	// put Tx & Rx pins into Alt5 mode
	// normally works without this unless pins have been
	// configured for general GPIO by another process
	pinModeAlt(15, 0b0010);
	pinModeAlt(16, 0b0010);

	fd = serialOpen("/dev/ttyS0", 9600);

	if (fd == -1)
	{
		ret = -1;
	}

	while(true)
	{
		serialPutchar(fd, 'a' + count++);
		sleep(1);
		if (count == 10) count = 0;

		ready = serialDataAvail(fd);

		if (ready > 0)
		{
			for (int i=0; i < ready; i++)
			{
				ch = serialGetchar(fd);
				ch = toupper(ch);
				serialPutchar(fd, ch);
			}
		}
	}

	return ret;
}

