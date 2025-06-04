#include "ViewerWindow.hpp"
#include "BagLoader.hpp"
#include <QApplication>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " /path/to/rosbag2" << std::endl;
        return 1;
    }

    QString bagDirPath = argv[1];
    QDir bagDir(bagDirPath);
    QFileInfo metadataFile(bagDir.filePath("metadata.yaml"));

    if (!bagDir.exists() || !metadataFile.exists())
    {
        QMessageBox::critical(nullptr, "Error", "No metadata.yaml file found in the specified bag directory.");
        return 1;
    }

    QStringList db3Files = bagDir.entryList(QStringList() << "*.db3", QDir::Files);
    if (db3Files.isEmpty())
    {
        QMessageBox::critical(nullptr, "Error", "No .db3 file found in the specified bag directory.");
        return 1;
    }

    // TODO: Handle multiple .db3 files if necessary
    QString db3FilePath = bagDir.filePath(db3Files.first());

    // Initialize BagLoader with metadata and db3 path
    BagLoader loader(metadataFile.filePath().toStdString(), db3FilePath.toStdString());
    std::vector<TopicInfo> allTopics = loader.getAllTopics();
    uint64_t startTime = loader.getStartTimeNs();
    uint64_t endTime = loader.getEndTimeNs();
    loader.open(allTopics, startTime, endTime); // Load all topics for now

    if (allTopics.empty())
    {
        QMessageBox::critical(nullptr, "Error", "No topics found in the bag.");
        return 1;
    }

    ViewerWindow *viewer = new ViewerWindow(loader);
    viewer->resize(1024, 768);
    viewer->setWindowTitle("BagViz - Viewer");
    viewer->show();

    //selector.exec(); // Blocks here until OK is pressed
    return app.exec();
}
