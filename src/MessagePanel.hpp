#pragma once

#include <QWidget>
#include <QMap>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include "BagLoader.hpp"

class MessagePanel : public QWidget
{
    Q_OBJECT

public:
    explicit MessagePanel(const BagLoader &loader, QWidget *parent = nullptr);
    std::vector<TopicInfo> getSelectedTopics() const;

private:
    QVBoxLayout *m_mainLayout;
    QMap<QString, std::pair<QCheckBox *, TopicInfo>> m_checkboxes;
};
