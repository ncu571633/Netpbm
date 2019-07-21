# Netpbm
The program used to Read/Write portable pixmap Netpbm file(format .pbm, .pgm, .ppm, .pnm)

See Netpbm format definition at:
https://en.wikipedia.org/wiki/Netpbm_format

I/O operation:
P1, P2: Standard I/O
P4: Memory Map

Data storage:
P1, P4: since the color of P1/P4 is 0–1 (white & black), bit array is used to store image matrix in order to save memory.
P2: Use a 1-D array to store image matrix
