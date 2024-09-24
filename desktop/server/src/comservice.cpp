#include "comservice.h"
#include "setting.h" // Include the settings header to access Signal

void COMService::insert(uint8_t data, uint32_t start, uint32_t length)
{
    if (start + length <= sizeof(buffer) * CHAR_BIT)
    {
        int pos = start % CHAR_BIT;
        int index = start / CHAR_BIT;
        for (std::size_t i = 0; i < length; i++)
        {
            uint8_t bit = (uint8_t)((data >> i) & 1); // Extract the bit at position i
            if (bit == 0)
            {
                buffer[index] &= ~(1 << pos); // Clear the bit at the position
            }
            else
            {
                buffer[index] |= (1 << pos); // Set the bit at the position
            }
            if (++pos == CHAR_BIT) // Move to the next byte if position reaches CHAR_BIT
            {
                pos = 0; // Reset bit position
                index++; // Move to the next byte
            }
        }
    }
}

void COMService::set_speed(uint8_t speed)
{
    insert(speed, signal["speed"].start, signal["speed"].length);
}

void COMService::set_temperature(int8_t temperature)
{
    insert(static_cast<uint8_t>(temperature), signal["temperature"].start, signal["temperature"].length);
}

void COMService::set_battery_level(uint8_t battery_level)
{
    insert(battery_level, signal["battery_level"].start, signal["battery_level"].length);
}

void COMService::set_light_signal_state(light_signal_t light_signal)
{
    insert(light_signal.left,
           signal["light_signal_left"].start,
           signal["light_signal_left"].length);
    insert(light_signal.right,
           signal["light_signal_right"].start,
           signal["light_signal_right"].length);
    insert(light_signal.warning,
           signal["light_signal_warning"].start,
           signal["light_signal_warning"].length);
}
