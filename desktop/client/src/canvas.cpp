#include "canvas.h"
#include <QFileInfo>
#include <QDebug>
// Constructor for the Canvas class
Canvas::Canvas(COMService *comserv, QWidget *parent)
    : QWidget(parent), comserv(comserv), blinkerSound(this), blinkerSoundOutput(this), backgroundColor(Qt::black) // Initialize base class QWidget and member variable comserv
{
    // Set a fixed size for the Canvas widget
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    blinkerSound.setAudioOutput(&blinkerSoundOutput);

    QString filePath = "../desktop/client/res/turn-signals.wav"; // Adjust the path as necessary

    // Check if the file exists
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists())
    {
        qDebug() << "Error: File does not exist at" << filePath;
    }
    else
    {
        qDebug() << "File exists at" << filePath;
        // Set the source for QSoundEffect
        blinkerSound.setSource(QUrl::fromLocalFile(filePath));
        blinkerSound.setLoops(QMediaPlayer::Infinite);
    }
}

// Override of the paintEvent method to perform custom painting
void Canvas::paintEvent(QPaintEvent *event)
{
    // Call the base class implementation of paintEvent
    QWidget::paintEvent(event);

    // Create a QPainter object to handle painting operations
    QPainter painter(this);

    // Fill the entire widget area with black color
    painter.fillRect(rect(), backgroundColor);

    // Create a QPen object to define the style of the lines
    QPen pen;
    pen.setStyle(Qt::SolidLine);     // Set the pen style to solid line
    pen.setWidth(10);                // Set the pen width to 10 pixels
    pen.setBrush(Qt::white);         // Set the pen color to white
    pen.setCapStyle(Qt::FlatCap);    // Set the cap style of the pen to flat
    pen.setJoinStyle(Qt::RoundJoin); // Set the join style of the pen to round

    // Enable antialiasing for smoother lines
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);

    // Draw an arc using the defined pen and constants
    painter.drawArc(ARC_RECTANGLE, START_ANGLE, SPAN_ANGLE);

    // Calculate the center of the arc rectangle
    QPointF center = ARC_RECTANGLE.center();

    // Set brush color to red and draw a circle at the center of the arc
    painter.setBrush(Qt::red);
    painter.drawEllipse(center, ELLIPSE_RADIUS, ELLIPSE_RADIUS);

    // Set the font for text drawing
    painter.setFont(HELVETICA_FONT_14);

    // Iterate through each tick mark
    for (int i = 0; i <= TOTAL_TICKS; ++i)
    {
        // Calculate the angle for the tick mark
        qreal angle = INITIAL_ANGLE + i * ANGLE_INCREMENT;
        qreal radians = qDegreesToRadians(angle); // Convert angle to radians

        // Calculate the start position of the tick mark
        qreal x1 = center.x() + 247 * qCos(radians);
        qreal y1 = center.y() - 247 * qSin(radians);

        qreal x2, y2;

        // Determine the style and length of the tick mark
        if (i % 4 == 0)
        {
            pen.setWidth(6); // Set pen width for large tick marks
            painter.setPen(pen);
            x2 = center.x() + LARGE_TICK_LENGTH * qCos(radians);
            y2 = center.y() - LARGE_TICK_LENGTH * qSin(radians);

            // Calculate and display the tick mark value
            int value = (TOTAL_TICKS - i) / 4 * 20;
            QString valueString = QString::number(value);
            QPointF textPosition = QPointF(x1 - TEXT_DISTANCE_OFFSET * qCos(radians) + TEXT_OFFSET_X,
                                           y1 + TEXT_DISTANCE_OFFSET * qSin(radians) + TEXT_OFFSET_Y);
            painter.drawText(textPosition, valueString); // Draw the tick mark value
        }
        else if (i % 4 == 2)
        {
            pen.setWidth(4); // Set pen width for medium tick marks
            painter.setPen(pen);
            x2 = center.x() + MEDIUM_TICK_LENGTH * qCos(radians);
            y2 = center.y() - MEDIUM_TICK_LENGTH * qSin(radians);
        }
        else
        {
            pen.setWidth(2); // Set pen width for small tick marks
            painter.setPen(pen);
            x2 = center.x() + SMALL_TICK_LENGTH * qCos(radians);
            y2 = center.y() - SMALL_TICK_LENGTH * qSin(radians);
        }

        // Draw the tick mark line
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    // Set the font for icon text
    painter.setFont(ICON_FONT_40);

    // Draw connection status icon and text based on the connection status
    if (comserv->get_connection_state())
    {
        painter.drawText(ICON_CONNECTION_X, ICON_CONNECTION_Y, QChar(0xe9e4));
        painter.setFont(HELVETICA_FONT_14);
        painter.drawText(SPEED_TEXT_X, SPEED_TEXT_Y, QString::number(comserv->get_speed()) + " km/h");
    }
    else
    {
        painter.drawText(ICON_CONNECTION_X, ICON_CONNECTION_Y, QChar(0xe628));
        painter.setFont(HELVETICA_FONT_14);
        painter.drawText(ERROR_TEXT_X, ERROR_TEXT_Y, "Connection error!");
    }

    // Set font for battery icon
    painter.setFont(ICON_FONT_100);
    painter.drawText(BATTERY_X, BATTERY_Y, QChar(0xebdc));

    // Retrieve the current temperature from COMService
    int temperature = comserv->get_temperature();

    // Set the pen color based on the temperature
    if (temperature < TEMPERATURE_THRESHOLD_LOW)
    {
        pen.setColor(Qt::white);
    }
    else if (temperature >= TEMPERATURE_THRESHOLD_LOW && temperature < TEMPERATURE_THRESHOLD_HIGH)
    {
        pen.setColor(Qt::blue);
    }
    else
    {
        pen.setColor(Qt::red);
    }

    // Set the font and color for the temperature icon
    painter.setFont(ICON_FONT_50);
    painter.setPen(pen);
    painter.drawText(TEMP_ICON_X, TEMP_ICON_Y, QChar(0xe1ff));

    // Retrieve the current battery level from COMService
    double batteryLevel = comserv->get_battery_level();

    // Determine the color for the battery level indicator
    QColor batteryColor;
    if (batteryLevel < BATTERY_LEVEL_LOW)
    {
        batteryColor = Qt::red;
    }
    else if (batteryLevel >= BATTERY_LEVEL_LOW && batteryLevel < BATTERY_LEVEL_MEDIUM)
    {
        batteryColor = Qt::yellow;
    }
    else
    {
        batteryColor = Qt::green;
    }

    // Set the font and color for the battery icon
    painter.setFont(ICON_FONT_100);
    painter.setPen(batteryColor);
    painter.drawText(BATTERY_X, BATTERY_Y, QChar(0xebdc));

    // Define the rectangle for the battery level indicator
    QRectF batteryRect(BATTERY_RECT_X, BATTERY_RECT_Y,
                       BATTERY_RECT_WIDTH, -batteryLevel);

    // Define the rectangle representing the current battery level
    QRectF levelRect(batteryRect.left(),
                     batteryRect.bottom() - (batteryRect.height() * (batteryLevel / 100.0)),
                     batteryRect.width(),
                     batteryRect.height() * (batteryLevel / 100.0));

    // Set the brush color and draw the battery level rectangle
    painter.setBrush(batteryColor);
    painter.drawRect(levelRect);

    // Draw the outline of the battery rectangle
    pen.setColor(batteryColor);
    pen.setWidth(5);
    painter.setPen(pen);
    painter.drawRect(batteryRect);

    // Set the pen color to white for drawing battery text
    pen.setColor(Qt::white);
    painter.setPen(pen);

    // Set the font for battery text and draw the battery percentage
    painter.setFont(HELVETICA_FONT_10);
    QString batteryText = QString::number(batteryLevel) + "%";
    QPointF textPosition(batteryRect.x() + BATTERY_TEXT_OFFSET_X,
                         batteryRect.y() + BATTERY_TEXT_OFFSET_Y);
    painter.drawText(textPosition, batteryText);

    // Set the font and draw the temperature text
    painter.setFont(HELVETICA_FONT_10);
    painter.drawText(TEMP_TEXT_X, TEMP_TEXT_Y, QString::number(temperature) + " °C");

    // Set the font for the needle
    painter.setFont(HELVETICA_FONT_22);

    // Calculate the angle for the needle based on the speed
    qreal needleRadians = qDegreesToRadians(NEEDLE_OFFSET - comserv->get_speed());

    // Set the pen color for the needle
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(10);
    pen.setColor(Qt::red);
    painter.setPen(pen);

    // Calculate the end position of the needle
    qreal needleX = center.x() + NEEDLE_LENGTH * qCos(needleRadians);
    qreal needleY = center.y() - NEEDLE_LENGTH * qSin(needleRadians);

    // Draw the needle
    painter.drawLine(center, QPointF(needleX, needleY));

    if (comserv->get_connection_state() == true)
    {
        COMService::light_signal_t signal = comserv->get_light_signal_state();

        QColor leftColor = backgroundColor;  // Default to off
        QColor rightColor = backgroundColor; // Default to off

        if (signal.left)
        {
            leftColor = isIconGreen ? Qt::green : backgroundColor;
        }

        if (signal.right)
        {
            rightColor = isIconGreen ? Qt::green : backgroundColor;
        }

        if (signal.warning)
        {
            leftColor = isIconGreen ? Qt::green : backgroundColor;
            rightColor = isIconGreen ? Qt::green : backgroundColor;
        }

        // Set the pen color and draw the left signal
        pen.setColor(leftColor);
        painter.setPen(pen);
        painter.setFont(ICON_FONT_50);
        painter.drawText(50, 75, QChar(0xe5c4)); // Left signal

        // Set the pen color and draw the right signal
        pen.setColor(rightColor);
        painter.setPen(pen);
        painter.drawText(600, 75, QChar(0xe5c8)); // Right signal

        bool shouldPlaySound = (signal.left || signal.right || signal.warning);

        // Manage the sound playback based on the signal state
        if (shouldPlaySound && !blinkerSound.isPlaying())
        {
            blinkerSound.play(); // Start playing the sound if it's not already playing
        }
        else if (!shouldPlaySound && blinkerSound.isPlaying())
        {
            blinkerSound.stop(); // Stop playing the sound if no signal is active
        }
    }
    else
    {
        blinkerSound.stop(); // Stop sound if the connection is lost
    }
}
