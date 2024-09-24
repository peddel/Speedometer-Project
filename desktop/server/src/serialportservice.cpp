#include "serialportservice.h"
#include <QDebug>

void SerialPortService::run()
{
    QSerialPort serialPort;
    serialPort.setPortName(Setting::SerialPort::SERVER_PORT);
    serialPort.setBaudRate(Setting::SerialPort::BAUD_RATE);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setParity(QSerialPort::NoParity);

    while (!end)
    {
        if (connected && serialPort.isWritable())
        {
            uint8_t temp_buffer[sizeof(buffer)]{0};
            
            // Lock the buffer before accessing it
            {
                std::scoped_lock<std::mutex> lock(buffer_mutex);
                std::memcpy(temp_buffer, buffer, sizeof(buffer));
            }

            QByteArray data(reinterpret_cast<const char *>(temp_buffer), sizeof(buffer));
            // Send the data in the buffer
            int bytesWritten = serialPort.write(data);

            serialPort.waitForBytesWritten(100);

            if (bytesWritten == -1)
            {
                qDebug() << "Failed to write to serial port:" << serialPort.errorString();
                connected = false;
                serialPort.close();
            }
            else if (bytesWritten != sizeof(buffer))
            {
                qDebug() << "Could not write all data to serial port:" << serialPort.errorString();
                connected = false;
                serialPort.close();
            }
            else
            {
                qDebug() << "Successfully wrote" << bytesWritten << "bytes to serial port.";
            }
        }
        else
        {
            if (serialPort.open(QIODevice::WriteOnly))
            {
                qDebug() << "Serial port successfully opened";
                connected = true;
            }
            else
            {
                qDebug() << serialPort.errorString();
                connected = false;
            }
        }

        // Sleep for 20 milliseconds before the next send
        msleep(Setting::INTERVAL_MS);
    }
}

bool SerialPortService::get_connection_state()
{
    return connected;
}
