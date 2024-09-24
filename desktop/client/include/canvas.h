#ifndef CANVAS_H
#define CANVAS_H

// Include the QWidget class, which is the base class for all GUI objects in Qt
#include <QWidget>

// Include the QPainter class, which provides functionality for painting on widgets
#include <QPainter>
#include <QColor>

// Include the header file for the audio, which contains various constants used for media
#include <QMediaPlayer>
#include <QAudioOutput>


// Include the header file for COMService, which is presumably used for communication and provides data to be displayed
#include "comservice.h"

// Window dimensions in pixels
constexpr int WINDOW_WIDTH = 800;  // Width of the window
constexpr int WINDOW_HEIGHT = 560; // Height of the window

// Arc rectangle dimensions and angles used for drawing an arc
constexpr QRectF ARC_RECTANGLE(70.0, 50.0, 600.0, 600.0); // Rectangle defining the boundary for the arc
constexpr int START_ANGLE = 330 * 16;                     // Starting angle of the arc in 1/16th of a degree
constexpr int SPAN_ANGLE = 240 * 16;                      // Span angle of the arc in 1/16th of a degree

// Tick marks on the arc
constexpr int NUM_LARGE_TICKS = 13;                     // Number of large tick marks (major divisions) on the arc
constexpr int TOTAL_TICKS = (NUM_LARGE_TICKS - 1) * 4;  // Total number of ticks, including smaller ones
constexpr double ANGLE_INCREMENT = 240.0 / TOTAL_TICKS; // Angle between each tick mark

// Initial angle and tick mark lengths
constexpr double INITIAL_ANGLE = 330;      // Starting angle for the first tick mark in degrees
constexpr double LARGE_TICK_LENGTH = 280;  // Length of large (major) tick marks in pixels
constexpr double MEDIUM_TICK_LENGTH = 275; // Length of medium tick marks in pixels
constexpr double SMALL_TICK_LENGTH = 270;  // Length of small tick marks in pixels

// Text positioning relative to tick marks
constexpr double TEXT_DISTANCE_OFFSET = 21; // Offset distance for the text from the tick marks in pixels
constexpr double NEEDLE_LENGTH = 220;       // Length of the needle (pointer) in pixels

// Needle value and offset
constexpr double NEEDLE_VALUE = 240 - 50; // Value that determines where the needle points (subtracting 50 to adjust)
constexpr int NEEDLE_OFFSET = 210;        // Offset value used in needle positioning

// Temperature thresholds for color changes
constexpr double TEMPERATURE_THRESHOLD_LOW = 5;   // Lower threshold for temperature color change
constexpr double TEMPERATURE_THRESHOLD_HIGH = 39; // Upper threshold for temperature color change

// Battery level thresholds for color changes
constexpr double BATTERY_LEVEL_LOW = 25.0;    // Low battery level threshold for color change
constexpr double BATTERY_LEVEL_MEDIUM = 50.0; // Medium battery level threshold for color change

// Magic numbers turned into constants for clarity
constexpr int ELLIPSE_RADIUS = 18;        // Radius of the central ellipse in pixels
constexpr int TEXT_OFFSET_X = -15;        // X-axis offset for text positioning relative to tick marks
constexpr int TEXT_OFFSET_Y = 9;          // Y-axis offset for text positioning relative to tick marks
constexpr int ICON_CONNECTION_X = 345;    // X-axis position for the connection status icon
constexpr int ICON_CONNECTION_Y = 430;    // Y-axis position for the connection status icon
constexpr int SPEED_TEXT_X = 345;         // X-axis position for the speed text
constexpr int SPEED_TEXT_Y = 450;         // Y-axis position for the speed text
constexpr int ERROR_TEXT_X = 300;         // X-axis position for the error text
constexpr int ERROR_TEXT_Y = 450;         // Y-axis position for the error text
constexpr int BATTERY_X = 675;            // X-axis position for the battery icon
constexpr int BATTERY_Y = 380;            // Y-axis position for the battery icon
constexpr int BATTERY_RECT_X = 751 - 25;  // X-axis position for the battery rectangle
constexpr int BATTERY_RECT_Y = 410 - 50;  // Y-axis position for the battery rectangle
constexpr int BATTERY_RECT_WIDTH = 30;    // Width of the battery rectangle in pixels
constexpr int BATTERY_RECT_HEIGHT = 50;   // Height of the battery rectangle in pixels (negative for direction)
constexpr int BATTERY_TEXT_OFFSET_X = 7;  // X-axis offset for the battery text inside the rectangle
constexpr int BATTERY_TEXT_OFFSET_Y = 25; // Y-axis offset for the battery text inside the rectangle
constexpr int TEMP_TEXT_X = 730;          // X-axis position for the temperature text
constexpr int TEMP_TEXT_Y = 490;          // Y-axis position for the temperature text
constexpr int TEMP_ICON_X = 710;          // X-axis position for the temperature icon
constexpr int TEMP_ICON_Y = 480;          // Y-axis position for the temperature icon

// Font icons represented as hexadecimal values
constexpr unsigned int CONNECTION_OK_ICON = 0xe9e4;    // Unicode value for connection OK icon
constexpr unsigned int CONNECTION_ERROR_ICON = 0xe628; // Unicode value for connection error icon
constexpr unsigned int BATTERY_ICON = 0xebdc;          // Unicode value for battery icon
constexpr unsigned int TEMP_ICON = 0xe0c8;             // Unicode value for temperature icon

const QFont HELVETICA_FONT_14("Helvetica", 14);  // Helvetica font, size 14
const QFont HELVETICA_FONT_22("Helvetica", 22);  // Helvetica font, size 22
const QFont HELVETICA_FONT_10("Helvetica", 10);  // Helvetica font, size 10
const QFont ICON_FONT_40("MaterialIcons", 40);   // Material Icons font, size 40
const QFont ICON_FONT_50("MaterialIcons", 50);   // Material Icons font, size 50
const QFont ICON_FONT_100("MaterialIcons", 100); // Material Icons font, size 100

// Declaration of the Canvas class, which inherits from QWidget
class Canvas : public QWidget
{

public:
    // Constructor declaration for Canvas class
    explicit Canvas(COMService *comserv, QWidget *parent = nullptr);
    bool isIconGreen = true;

protected:
    // Override of the paintEvent function to handle custom painting
    void paintEvent(QPaintEvent *event) override;

private:
    // Private member to hold a pointer to the COMService instance used for data
    COMService *comserv;
    QMediaPlayer blinkerSound;
    QAudioOutput blinkerSoundOutput;
    QColor backgroundColor;
};

#endif // CANVAS_H
