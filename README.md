First Experiments trying around with data types, file formats etc. 

Currently encodes text via a custom (?) algorithm dividing each char-byte into 4 * 2 bits, then modifies the RGBA values accordingly

- basic scrambling/unscrambling functionality tested -

To-Do:
Add cryptographic noise & pseudorandom distribution of data
Adapt algorithm to lower individual pixel change (max of 3pt too easy to see)
