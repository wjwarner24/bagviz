// #include "TopicSelector.hpp"
// #include <QScrollArea>
// #include <QGroupBox>
// #include <QVBoxLayout>
// #include <QPushButton>
// #include <QCheckBox>
// #include <QLabel>

// TopicSelector::TopicSelector(const std::vector<TopicInfo> &allTopics, uint64_t startTime, uint64_t endTime, QWidget *parent)
//     : QDialog(parent), m_startTime(startTime), m_endTime(endTime)
// {
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);

//     // Scrollable topic checkboxes
//     QGroupBox *topicBox = new QGroupBox("Topics");
//     QVBoxLayout *topicLayout = new QVBoxLayout(topicBox);
//     for (const auto &topic : allTopics)
//     {
//         QString label = QString("%1 (%2) - %3 msgs")
//                             .arg(QString::fromStdString(topic.name))
//                             .arg(QString::fromStdString(topic.type))
//                             .arg(QString::number(topic.msgCount));

//         QCheckBox *checkbox = new QCheckBox(label);
//         m_topicCheckboxes[QString::fromStdString(topic.name)] = std::pair(checkbox, topic);
//         topicLayout->addWidget(checkbox);
//     }
//     topicBox->setLayout(topicLayout);

//     QScrollArea *scroll = new QScrollArea;
//     scroll->setWidgetResizable(true);
//     scroll->setWidget(topicBox);
//     mainLayout->addWidget(scroll);

//     // OK button
//     QPushButton *okButton = new QPushButton("OK");
//     connect(okButton, &QPushButton::clicked, this, &TopicSelector::confirmSelection);
//     mainLayout->addWidget(okButton);
// }

// void TopicSelector::confirmSelection()
// {
//     emit selectionConfirmed(getSelectedTopics(), getSelectedStartTime(), getSelectedEndTime());
//     accept(); // closes the dialog
// }

// std::vector<TopicInfo> TopicSelector::getSelectedTopics() const
// {
//     std::vector<TopicInfo> selected;
//     for (auto it = m_topicCheckboxes.begin(); it != m_topicCheckboxes.end(); ++it)
//     {
//         const QCheckBox *checkbox = it.value().first;
//         const TopicInfo &topic = it.value().second;

//         if (checkbox->isChecked())
//         {
//             selected.push_back(topic);
//         }
//     }
//     return selected;
// }

// uint64_t TopicSelector::getSelectedStartTime() const
// {
//     return m_startTime;
// }

// uint64_t TopicSelector::getSelectedEndTime() const
// {
//     return m_endTime;
// }
