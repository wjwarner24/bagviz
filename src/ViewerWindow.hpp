#pragma once

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include "BagLoader.hpp"
#include "MessagePanel.hpp"
#include <QLabel>
#include "PlaceholderDisplay.hpp"

class ViewerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerWindow(const BagLoader &loader, QWidget *parent = nullptr);

private slots:
    void handleTimeChanged(int value);

private:
    const BagLoader &m_loader;
    uint64_t m_startTime;
    uint64_t m_endTime;
    QLabel *m_timeLabel; // Display current time
    QSlider *m_timeSlider;
    MessagePanel *m_messagePanel;
    PlaceholderDisplay *m_placeholderDisplay;
};