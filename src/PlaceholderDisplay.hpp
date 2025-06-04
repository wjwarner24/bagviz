#pragma once

#include <QWidget>
#include <QTextEdit>
#include <vector>
#include "BagLoader.hpp"

class PlaceholderDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit PlaceholderDisplay(QWidget *parent = nullptr);

    void displayMessages(const std::vector<TopicMessage> &messages);

private:
    QTextEdit *m_textEdit;
};
