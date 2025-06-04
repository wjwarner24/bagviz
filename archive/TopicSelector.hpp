// #pragma once

// #include <QDialog>
// #include <QMap>
// #include <QCheckBox>
// #include <vector>
// #include <QString>
// #include "BagLoader.hpp"

// class TopicSelector : public QDialog
// {
//     Q_OBJECT

// public:
//     TopicSelector(const std::vector<TopicInfo> &allTopics, uint64_t startTime, uint64_t endTime, QWidget *parent = nullptr);

//     std::vector<TopicInfo> getSelectedTopics() const;
//     uint64_t getSelectedStartTime() const;
//     uint64_t getSelectedEndTime() const;

// signals:
//     void selectionConfirmed(std::vector<TopicInfo>, uint64_t, uint64_t);

// private slots:
//     void confirmSelection();

// private:
//     QMap<QString, std::pair<QCheckBox *, TopicInfo>> m_topicCheckboxes;
//     uint64_t m_startTime;
//     uint64_t m_endTime;
// };

















// TopicSelector selector(allTopics, startTime, endTime);
// selector.setWindowTitle("Select Topics to Visualize");
// selector.resize(1024, 768);

// QObject::connect(&selector, &TopicSelector::selectionConfirmed,
//                  [&](std::vector<TopicInfo> selectedTopics, uint64_t tStart, uint64_t tEnd)
//                  {
//                      loader.open(selectedTopics, tStart, tEnd);

//                      ViewerWindow *viewer = new ViewerWindow(loader);
//                      viewer->resize(1024, 768);
//                      viewer->setWindowTitle("BagViz - Viewer");
//                      viewer->show();
//                  });
