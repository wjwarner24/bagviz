#pragma once

#include <string>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include <iostream>

struct TopicInfo
{
    std::string name;
    std::string type;
    std::string type_description_hash;
    uint64_t msgCount;

    bool operator==(const TopicInfo &other) const
    {
        return name == other.name;
    }
};

namespace std
{
    template <>
    struct hash<TopicInfo>
    {
        std::size_t operator()(const TopicInfo &t) const
        {
            return hash<std::string>()(t.type_description_hash);
        }
    };
}

struct BagMessage
{
    uint64_t timestamp;
    std::vector<uint8_t> data; // raw serialized data
};

struct TopicMessage
{
    TopicInfo topic_name;
    BagMessage message;
};

class BagLoader
{
public:
    // Constructor and Destructor
    BagLoader(const std::string &metadataPath, const std::string &db3Path);
    ~BagLoader();

    // Load metadata
    void init();
    void reset();

    // Loads data into m_topicMessages from the selected topics from the selected times
    void open(std::vector<TopicInfo> &selectedTopics, uint64_t startTime, uint64_t endTime);

    // Gets a message from each topic at a specific time
    std::vector<TopicMessage> getMessagesAt(uint64_t timestamp, const std::vector<TopicInfo> &visualize_topics) const;

    // Gets a message for a specific topic at or before a specific timestamp
    std::optional<BagMessage> getMessageAtTime(const std::vector<BagMessage> &messages, uint64_t timestamp) const;

    // Populates topics and start/end time of the bag from metadata
    void getTopicsInfo();
    void getStartTime();
    void getEndTime();

    // Getters for metadata
    std::vector<TopicInfo> getAllTopics() const { return m_topics; }
    uint64_t getStartTimeNs() const { return m_start_time; }
    uint64_t getEndTimeNs() const { return m_end_time; }
    bool isInitialized() const { return m_inited; }
    uint64_t getSelectedStartTimeNs() const { return m_selected_start_time; }
    uint64_t getSelectedEndTimeNs() const { return m_selected_end_time; }

private:
    std::string m_metadataPath;
    std::string m_db3Path;
    sqlite3 *m_db;
    std::vector<TopicInfo> m_topics;
    uint64_t m_start_time;
    uint64_t m_end_time;

    // Variables that need to be reset on each open
    std::unordered_map<TopicInfo, std::vector<BagMessage>> m_topicMessages;
    uint64_t m_selected_start_time;
    uint64_t m_selected_end_time;
    bool m_inited = false;
};
