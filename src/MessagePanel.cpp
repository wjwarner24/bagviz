#include "MessagePanel.hpp"
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>

MessagePanel::MessagePanel(const BagLoader &loader, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *containerLayout = new QVBoxLayout(this);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    QWidget *scrollContent = new QWidget;
    m_mainLayout = new QVBoxLayout(scrollContent);

    std::vector<TopicInfo> topics = loader.getAllTopics();

    for (const auto &topic : topics)
    {
        QString topicName = QString::fromStdString(topic.name);
        QGroupBox *groupBox = new QGroupBox(topicName);
        groupBox->setCheckable(false);

        QVBoxLayout *groupLayout = new QVBoxLayout;

        QCheckBox *checkbox = new QCheckBox("Visualize");
        checkbox->setChecked(false);
        groupLayout->addWidget(checkbox);

        QFormLayout *infoLayout = new QFormLayout;
        infoLayout->addRow("Type:", new QLabel(QString::fromStdString(topic.type)));
        infoLayout->addRow("Messages:", new QLabel(QString::number(topic.msgCount)));

        groupLayout->addLayout(infoLayout);
        groupBox->setLayout(groupLayout);

        m_checkboxes.insert(topicName, std::make_pair(checkbox, topic));

        m_mainLayout->addWidget(groupBox);
    }

    m_mainLayout->addStretch();
    scrollContent->setLayout(m_mainLayout);
    scrollArea->setWidget(scrollContent);
    containerLayout->addWidget(scrollArea);
    setLayout(containerLayout);
}

std::vector<TopicInfo> MessagePanel::getSelectedTopics() const
{
    std::vector<TopicInfo> selected;
    for (auto it = m_checkboxes.constBegin(); it != m_checkboxes.constEnd(); ++it)
    {
        const QCheckBox *checkbox = it.value().first;
        const TopicInfo &info = it.value().second;
        if (checkbox && checkbox->isChecked())
        {
            selected.push_back(info);
        }
    }
    return selected;
}


