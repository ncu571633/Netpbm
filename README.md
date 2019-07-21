# Netpbm
The program is used to Read/Write portable pixmap Netpbm files (format .pbm, .pgm, .ppm, .pnm).

See Netpbm format definition at:
https://en.wikipedia.org/wiki/Netpbm_format

I/O operation:
P1, P2 format: Standard I/O
P4 format: Memory Map

Data storage:
P1, P4 format: 
    Since the color of P1/P4 is 0/1(white/black), a bit array with size (row * column) / 32 is used to store image matrix in order to save memory. The element matrix[i][j] is stored at the (i * column + j) % 32 bit of data[(i * column + j) / 32]. The compression rate is 32 (32 is the size of int)
P2: 
    Use a 1-D array to store image matrix. The element matrix[i][j] is stored at data[i * column + j].

