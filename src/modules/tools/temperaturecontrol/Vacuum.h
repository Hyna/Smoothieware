/*
    This file was created by Hynek Stetina and brings to the smoothie firmware support for Vacuum sensor for pick and place mechine.

The motion control part is heavily based on grbl (https://github.com/simen/grbl).
      smoothie is free software: you can redistribute it and/or modify it under the terms of the gnu general public license as published by the free software foundation, either version 3 of the license, or (at your option) any later version.
      smoothie is distributed in the hope that it will be useful, but without any warranty; without even the implied warranty of merchantability or fitness for a particular purpose. see the gnu general public license for more details.
      you should have received a copy of the gnu general public license along with smoothie. if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VACUUM_H
#define VACUUM_H

#include "TempSensor.h"
#include "RingBuffer.h"
#include "Pin.h"

#include <tuple>

#define QUEUE_LEN 32


class Vacuum : public TempSensor
{
    public:
        Vacuum();
        ~Vacuum();

        // TempSensor interface.
        void UpdateConfig(uint16_t module_checksum, uint16_t name_checksum);
        void get_raw();


    private:
        int new_vacuum_reading();

        Pin  thermistor_pin;

        RingBuffer<uint16_t,QUEUE_LEN> queue;  // Queue of readings

};

#endif
