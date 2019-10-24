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
		uint8_t num_selectors,
		DigitalOut** select_outs,
		uint32_t del_us = 0);
	Mux(
		Platform::pin_t pin,
		uint8_t num_selectors,
		Platform::pin_t* select_pins,
		uint32_t del_us = 0);
	~Mux();
	IOClass& operator[](uint8_t channel);
protected:
	uint8_t num_selectors;
	DigitalOut** select_outs;
	uint32_t del_us;
	bool dynamic_io;
};

/**
 * @brief Constructs multiplexed IO pin
 * @param pin Platform-specific pin ID
 * @param num_selectors Number of select pins
 * @param select_outs Array of select digital output pointers
 * @param del_us Post-select delay [us]
 */
template<class IOClass>
Mux<IOClass>::Mux(
	Platform::pin_t pin,
	uint8_t num_selectors,
	DigitalOut** select_outs,
	uint32_t del_us) :
IOClass(pin)
{
	this->num_selectors = num_selectors;
	this->select_outs = select_outs;
	this->del_us = del_us;
	this->dynamic_io = false;
}

/**
 * @brief Constructs multiplexed IO pin
 * @param pin Platform-specific pin ID
 * @param num_selectors Number of select pins
 * @param select_pins Array of select pin IDs
 * @param del_us Post-select delay [us]
 * 
 * This constructor dynamically allocates the IO interfaces and deletes them
 * on destruction.
 */
template <class IOClass>
Mux<IOClass>::Mux(
	Platform::pin_t pin,
	uint8_t num_selectors,
	Platform::pin_t* select_pins,
	uint32_t del_us) :
Mux(pin, num_selectors, new DigitalOut*[num_selectors], del_us)
{
	for (uint8_t i = 0; i < num_selectors; i++)
	{
		this->select_outs[i] = new DigitalOut(select_pins[i]);
	}
	this->dynamic_io = true;
}

/**
 * @brief Destructs multiplexer interface
 * 
 * Deletes IO interfaces if they were created with dynamic memory.
 */
template <class IOClass>
Mux<IOClass>::~Mux()
{
	if (dynamic_io)
	{
		for (uint8_t i = 0; i < num_selectors; i++)
		{
			delete select_outs[i];
		}
		delete[] select_outs;
	}
}

/**
 * @brief Performs channel selection
 * @param channel Channel to multiplex
 * @return Reference to this object
 */
template<class IOClass>
IOClass& Mux<IOClass>::operator[](uint8_t channel)
{
	for (uint8_t s = 0; s < num_selectors; s++)
	{
		select_outs[s]->write((channel >> s) & 1u);
	}
	Platform::wait_us(del_us);
	return *this;
}