#include "setting.h"
#include "window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>

Window::Window(COMService *service, QWidget *parent)
    : QDialog(parent), comserv(service),
      speedLabel(this), speedLabel2(this),
      temperatureLabel(this), temperatureLabel2(this),
      batteryLabel(this), batteryLabel2(this),
      speedSlider(Qt::Horizontal, this),
      temperatureSlider(Qt::Horizontal, this),
      batterySlider(Qt::Horizontal, this),
      leftCheckBox(this), rightCheckBox(this), warningCheckBox(this),
      layout(this), checkBoxLayout()
{
    Setting::Signal &signal{Setting::Signal::instance()};

    setWindowTitle("Server");
    setFixedSize(Setting::Server_GUI::Window_Size::width, Setting::Server_GUI::Window_Size::height);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    speedLabel.setText("Speed");
    speedLabel2.setText("0 kph");
    speedSlider.setRange(signal["speed"].min, signal["speed"].max);
    QObject::connect(&speedSlider, &QSlider::valueChanged, [this, service](int value)
                     {
        speedLabel2.setText(QString("%1 kph").arg(value));
        service->set_speed(value); });
    speedLabel2.setFixedWidth(Setting::Server_GUI::Label_Width::width);
    layout.addWidget(&speedLabel, 0, 0);
    layout.addWidget(&speedSlider, 0, 1);
    layout.addWidget(&speedLabel2, 0, 2);

    temperatureLabel.setText("Temperature");
    temperatureLabel2.setText("0°C");
    temperatureSlider.setRange(signal["temperature"].min, signal["temperature"].max);
    QObject::connect(&temperatureSlider, &QSlider::valueChanged, [this, service](int value)
                     {
        temperatureLabel2.setText(QString("%1°C").arg(value));
        service->set_temperature(value); });
    temperatureLabel2.setFixedWidth(Setting::Server_GUI::Label_Width::width);
    layout.addWidget(&temperatureLabel, 1, 0);
    layout.addWidget(&temperatureSlider, 1, 1);
    layout.addWidget(&temperatureLabel2, 1, 2);

    batteryLabel.setText("Battery Level");
    batteryLabel2.setText("0%");
    batterySlider.setRange(signal["battery_level"].min, signal["battery_level"].max);
    QObject::connect(&batterySlider, &QSlider::valueChanged, [this, service](int value)
                     {
        batteryLabel2.setText(QString("%1%").arg(value));
        service->set_battery_level(value); });
    batteryLabel2.setFixedWidth(Setting::Server_GUI::Label_Width::width);
    layout.addWidget(&batteryLabel, 2, 0);
    layout.addWidget(&batterySlider, 2, 1);
    layout.addWidget(&batteryLabel2, 2, 2);

    leftCheckBox.setText("Left Signal");
    rightCheckBox.setText("Right Signal");
    warningCheckBox.setText("Warning Signal");

    // Add the checkboxes to the horizontal layout

    checkBoxLayout.addWidget(&leftCheckBox);
    checkBoxLayout.addStretch();
    checkBoxLayout.addWidget(&warningCheckBox);
    checkBoxLayout.addStretch();
    checkBoxLayout.addWidget(&rightCheckBox);

    // Add the horizontal layout to the grid layout
    layout.addLayout(&checkBoxLayout, 3, 0, 1, 3); // Span across 3 columns

    rightCheckBox.setStyleSheet("QCheckBox:disabled { color: rgba(0, 0, 0, 0.5); }");
    leftCheckBox.setStyleSheet("QCheckBox:disabled { color: rgba(0, 0, 0, 0.5); }");

    QObject::connect(&leftCheckBox, &QCheckBox::stateChanged, [this, service](int state)
                     {
        bool checked = state == Qt::Checked;
        bool warningOn = warningCheckBox.checkState() == Qt::Checked;
        service->set_light_signal_state({checked, 0, warningOn});
        if (state == Qt::Checked) {
            rightCheckBox.setDisabled(true);
        } else {
            rightCheckBox.setEnabled(true);
        } });
    QObject::connect(&rightCheckBox, &QCheckBox::stateChanged, [this, service](int state)
                     {
        bool checked = state == Qt::Checked;
        bool warningOn = warningCheckBox.checkState();
        service->set_light_signal_state({0, checked, warningOn});
        if (state == Qt::Checked) {
            leftCheckBox.setDisabled(true);
        } else {
            leftCheckBox.setEnabled(true);
        } });
    QObject::connect(&warningCheckBox, &QCheckBox::stateChanged, [this, service](int state)
                     {
        bool checked = state == Qt::Checked;
        bool leftOn = leftCheckBox.checkState() == Qt::Checked;
        bool rightOn = rightCheckBox.checkState() == Qt::Checked;
        service->set_light_signal_state({leftOn, rightOn, checked}); });
    setLayout(&layout);
}
