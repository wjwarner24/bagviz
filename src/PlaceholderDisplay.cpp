#include "PlaceholderDisplay.hpp"
#include <QVBoxLayout>

PlaceholderDisplay::PlaceholderDisplay(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_textEdit = new QTextEdit(this);
    m_textEdit->setReadOnly(true);
    layout->addWidget(m_textEdit);
}

void PlaceholderDisplay::displayMessages(const std::vector<TopicMessage> &messages)
{
    m_textEdit->clear();

    m_textEdit->append(QString("Received %1 messages:\n").arg(messages.size()));

    for (const auto &msg : messages)
    {
        QString line = QString("Topic: %1 | Timestamp: %2 | Data size: %3 bytes")
                           .arg(QString::fromStdString(msg.topic_name.name))
                           .arg(msg.message.timestamp)
                           .arg(msg.message.data.size());

        m_textEdit->append(line);
    }
}
