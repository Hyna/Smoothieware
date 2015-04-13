/*
    This file was created by Hynek Stetina and brings to the smoothie firmware support for Vacuum sensor for pick and place mechine.      
 The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Vacuum.h"
#include "libs/Kernel.h"
#include "libs/Pin.h"
#include "Config.h"
#include "checksumm.h"
#include "Adc.h"
#include "ConfigValue.h"
#include "libs/Median.h"
#include "utils.h"
#include "StreamOutputPool.h"

#include <fastmath.h>

#include "MRI_Hooks.h"

#define UNDEFINED -1
#define thermistor_checksum                CHECKSUM("thermistor")
#define thermistor_pin_checksum            CHECKSUM("thermistor_pin")


Vacuum::Vacuum()
{

}

Vacuum::~Vacuum()
{
}

// Get configuration from the config file
void Vacuum::UpdateConfig(uint16_t module_checksum, uint16_t name_checksum)
{

    // ADC pin for vacuum readings
    this->thermistor_pin.from_string(THEKERNEL->config->value(module_checksum, name_checksum, thermistor_pin_checksum )->required()->as_string());
    THEKERNEL->adc->enable_pin(&thermistor_pin);


}


void Vacuum::get_raw()
{

    int adc_value= new_vacuum_reading();
    THEKERNEL->streams->printf("adc= %d\n", adc_value);

}



int Vacuum::new_vacuum_reading()
{
    int last_raw = THEKERNEL->adc->read(&thermistor_pin);
    if (queue.size() >= queue.capacity()) {
        uint16_t l;
        queue.pop_front(l);
    }
    uint16_t r = last_raw;
    queue.push_back(r);
    uint16_t median_buffer[queue.size()];
    for (int i=0; i<queue.size(); i++)
      median_buffer[i] = *queue.get_ref(i);
    uint16_t m = median_buffer[quick_median(median_buffer, queue.size())];
    return m;
};
