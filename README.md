# Mux
Class for multiplexing Arduino and Mbed digital IO pins  
Written by Dan Oates (WPI Class of 2020)

### Description
This class is designed to interface with multiplexers that map 2^N channels to one IO pin via N digital IO-controlled select lines. The template class IOClass can be one of the following:

- [DigitalOut](https://github.com/doates625/DigitalOut.git)
- [DigitalIn](https://github.com/doates625/DigitalIn.git)
- [PwmOut](https://github.com/doates625/PwmOut.git)
- [AnalogIn](https://github.com/doates625/AnalogIn.git)
- [PulseIn](https://github.com/doates625/PulseIn.git)

The operator[] allows for natural indexing as if the object were truly an
array of IO pins. For example:

```
Mux<DigitalIn> input(D3, 4, <select lines>)	// 16-channel input mux on D3
int reading = input[12].read()			// Read channel 12
```

Channels are indexed from 0 to 2^N-1. Note that device-specific hardware latency may result in a pin operation proceeding before the select lines have settled. To compensate for this, instances of the Mux class take a microsecond delay at construction which automatically occurs after each select line change before allowing for a pin operation.

### Dependencies
- [Platform](https://github.com/doates625/Platform.git)
- [DigitalOut](https://github.com/doates625/DigitalOut.git)

### References
- [Example Mux](https://www.sparkfun.com/products/9056)