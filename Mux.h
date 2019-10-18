/**
 * @file Mux.h
 * @brief Class for multiplexing Arduino and Mbed digital IO pins
 * @author Dan Oates (WPI Class of 2020)
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
		Platform::pin_t pin,
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
	Platform::pin_t pin,
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
	Platform::wait_us(del_us);
	return *this;
}