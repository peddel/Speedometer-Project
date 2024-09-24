#include <QDebug>
#include <chrono>
#include <QSerialPort>
#include "serialportservice.h"

void SerialPortService::run()
{
    QSerialPort serial;
    serial.setPortName(Setting::SerialPort::CLIENT_PORT);
    serial.setBaudRate(Setting::SerialPort::BAUD_RATE);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);

    while (!end)
    {
        if (connected && serial.isReadable())
        {
            serial.clear();
            uint8_t temp_buffer[sizeof(buffer)]{0};
            if (serial.waitForReadyRead(Setting::INTERVAL_MS * 2)) // just something larger than INTERVAL_MS
            {
                if (sizeof(buffer) == serial.read(reinterpret_cast<char *>(temp_buffer), sizeof(buffer)))
                {
                    connected = true;
                    // Read success
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    std::memcpy(buffer, temp_buffer, sizeof(buffer));
                }
                else
                {
                    // Read fail
                    qDebug() << "Failed to read ";
                    connected = false;
                    serial.close();
                }
            }
            else
            {
                qDebug() << "Time out error";
                connected = false;
                serial.close();
            }
        }
        else
        {
            if (serial.open(QIODevice::ReadOnly))
            {
                qDebug() << "Serial port successfully opened";
                connected = true;
            }
            else
            {
                qDebug() << serial.errorString();
                connected = false;
            }
        }
        msleep(Setting::INTERVAL_MS);
    }

    if (serial.isOpen())
    {
        serial.close();
    }
}

bool SerialPortService::get_connection_state()
{
    return connected;
}
