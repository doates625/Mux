/**
 * @file Mux.h
 * @brief Class for multiplexing Arduino and Mbed digital IO pins
 * @author Dan Oates (WPI Class of 2020)
 * 
 * This class is designed to interface with multiplexers that map 2^N channels
 * to one IO pin via N digital IO-controlled select lines. The template class
 * IOClass can be one of the following:
 * 
 * - DigitalOut (https://github.com/doates625/DigitalOut.git)
 * - DigitalIn (https://github.com/doates625/DigitalIn.git)
 * - PwmOut (https://github.com/doates625/PwmOut.git)
 * - AnalogIn (https://github.com/doates625/AnalogIn.git)
 * 
 * The operator[] allows for natural indexing as if the object were truly an
 * array of IO pins. For example:
 * 
 * Mux<DigitalIn> input(D3, 4, <select lines>)	// 16-channel input mux on D3
 * int reading = input[12].read()				// Read channel 12
 * 
 * Channels are indexed from 0 to 2^N-1. Note that device-specific hardware
 * latency may result in a pin operation proceeding before the select lines
 * have settled. To compensate for this, instances of the Mux class take a
 * microsecond delay at construction which automatically occurs after each
 * select line change before allowing for a pin operation.
 * 
 * Dependencies:
 * - Platform: https://github.com/doates625/Platform.git
 * - DigitalOut: https://github.com/doates625/DigitalOut.git
 * 
 * References:
 * - Multiplexing: https://en.wikipedia.org/wiki/Multiplexing
 * - Example Mux: https://www.sparkfun.com/products/9056
 */
#pragma once
#include <DigitalOut.h>

/**
 * Class Declaration
 */
template<class IOClass>
class Mux : public IOClass
{
public:
	Mux(
		PLATFORM_PIN_TYPE pin,
		uint8_t num_sel_pins,
		DigitalOut* sel_pins,
		uint32_t del_us = 0);
	IOClass& operator[](uint8_t channel);
protected:
	uint8_t num_sel_pins;
	DigitalOut* sel_pins;
	uint32_t del_us;
};

/**
 * @brief Constructs multiplexed IO pin
 * @param pin Platform-specific pin ID
 * @param num_sel_pins Number of select pins
 * @param sel_pins Array of select pins
 * @param del_us Post-select delay [us]
 */
template<class IOClass>
Mux<IOClass>::Mux(
	PLATFORM_PIN_TYPE pin,
	uint8_t num_sel_pins,
	DigitalOut* sel_pins,
	uint32_t del_us
) :
	IOClass(pin)
{
	this->num_sel_pins = num_sel_pins;
	this->sel_pins = sel_pins;
	this->del_us = del_us;
}

/**
 * @brief Performs channel selection
 * @param channel Channel to multiplex
 * @return Reference to this object
 */
template<class IOClass>
IOClass& Mux<IOClass>::operator[](uint8_t channel)
{
	for (uint8_t s = 0; s < num_sel_pins; s++)
	{
		sel_pins[s].write((channel >> s) & 1u);
	}
#if defined(PLATFORM_ARDUINO)
	delayMicroseconds(del_us);
#elif defined(PLATFORM_MBED)
	wait_us(del_us);
#endif
	return *this;
}