This daemon periodically sends the current system-wide CPU and memory usage via the serial board.
The intended receiver is a micro controller displaying this information on an external display.
Currently, Linux is the only supported system.

This program uses the GPL-licensed [libserialport](https://sigrok.org/wiki/Libserialport) maintained by the Sigrok project.
