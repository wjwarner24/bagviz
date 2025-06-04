#include "ViewerWindow.hpp"
#include "PlaceholderDisplay.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QDebug>

ViewerWindow::ViewerWindow(const BagLoader &loader, QWidget *parent)
    : QWidget(parent), m_loader(loader)
{
    m_startTime = m_loader.getSelectedStartTimeNs();
    m_endTime = m_loader.getSelectedEndTimeNs();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Top: Time display label
    m_timeLabel = new QLabel("Time: 0", this);
    m_timeLabel->setStyleSheet("color: black; font-weight: bold;");
    mainLayout->addWidget(m_timeLabel, 0, Qt::AlignLeft);

    QHBoxLayout *centerLayout = new QHBoxLayout();

    // Left: Message Panel
    m_messagePanel = new MessagePanel(loader, this);
    centerLayout->addWidget(m_messagePanel, 1); // stretch factor 1

    // Right: PlaceholderDisplay
    m_placeholderDisplay = new PlaceholderDisplay(this);
    centerLayout->addWidget(m_placeholderDisplay, 3); // stretch factor 3

    mainLayout->addLayout(centerLayout);

    // Bottom: Time slider
    m_timeSlider = new QSlider(Qt::Horizontal, this);
    m_timeSlider->setRange(0, 1000);
    m_timeSlider->setValue(0);
    connect(m_timeSlider, &QSlider::valueChanged, this, &ViewerWindow::handleTimeChanged);
    mainLayout->addWidget(m_timeSlider);
}

void ViewerWindow::handleTimeChanged(int value)
{
    double ratio = value / 1000.0;
    uint64_t currentTime = m_startTime + static_cast<uint64_t>(ratio * (m_endTime - m_startTime));
    auto msgs = m_loader.getMessagesAt(currentTime, m_messagePanel->getSelectedTopics());

    m_timeLabel->setText(QString("Time: %1").arg(currentTime));

    m_placeholderDisplay->displayMessages(msgs);
}
