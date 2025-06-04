#include "BagLoader.hpp"

BagLoader::BagLoader(const std::string &metadataPath, const std::string &db3Path)
    : m_metadataPath(metadataPath), m_db3Path(db3Path), m_db(nullptr)
{
    init();
}

void BagLoader::init()
{
    // Initialize the database pointer to nullptr
    m_db = nullptr;

    // Load topics and times from metadata
    getTopicsInfo();
    getStartTime();
    getEndTime();
    m_inited = true;
}

void BagLoader::reset()
{
    m_topicMessages.clear();
    //m_selected_topics.clear();
    m_selected_start_time = m_start_time;
    m_selected_end_time = m_end_time;
    m_inited = false;
}

BagLoader::~BagLoader()
{
    if (m_db)
    {
        sqlite3_close(m_db);
    }
}

void BagLoader::getTopicsInfo()
{
    std::vector<TopicInfo> topics;

    try
    {
        YAML::Node root = YAML::LoadFile(m_metadataPath);
        auto topicList = root["rosbag2_bagfile_information"]["topics_with_message_count"];

        for (const auto &topic : topicList)
        {
            TopicInfo info;
            info.name = topic["topic_metadata"]["name"].as<std::string>();
            info.type = topic["topic_metadata"]["type"].as<std::string>();
            info.type_description_hash = topic["topic_metadata"]["type_description_hash"].as<std::string>();
            info.msgCount = topic["message_count"].as<int>();
            topics.push_back(info);
        }
    }
    catch (const YAML::Exception &e)
    {
        std::cerr << "Failed to parse metadata.yaml: " << e.what() << std::endl;
    }
    std::cout << "Found " << topics.size() << " topics in the bag." << std::endl;

    m_topics = topics;
}

void BagLoader::getStartTime()
{
    try
    {
        YAML::Node root = YAML::LoadFile(m_metadataPath);
        m_start_time = root["rosbag2_bagfile_information"]["starting_time"]["nanoseconds_since_epoch"].as<uint64_t>();
    }
    catch (const YAML::Exception &e)
    {
        std::cerr << "Error reading start time from metadata: " << e.what() << std::endl;
    }
}

void BagLoader::getEndTime()
{
    try
    {
        YAML::Node root = YAML::LoadFile(m_metadataPath);
        uint64_t start = root["rosbag2_bagfile_information"]["starting_time"]["nanoseconds_since_epoch"].as<uint64_t>();
        uint64_t duration = root["rosbag2_bagfile_information"]["duration"]["nanoseconds"].as<uint64_t>();
        m_end_time = start + duration;
    }
    catch (const YAML::Exception &e)
    {
        std::cerr << "Error reading end time from metadata: " << e.what() << std::endl;
    }
}

std::optional<BagMessage> BagLoader::getMessageAtTime(const std::vector<BagMessage> &messages, uint64_t timestamp) const
{
    if (messages.empty())
        return std::nullopt;

    // Binary search: find first element > timestamp
    auto it = std::upper_bound(messages.begin(), messages.end(), timestamp,
                               [](uint64_t value, const BagMessage &msg)
                               {
                                   return value < msg.timestamp;
                               });

    if (it == messages.begin())
    {
        return std::nullopt; // No message at or before this time
    }

    --it; // Move to message <= timestamp
    return *it;
}

std::vector<TopicMessage> BagLoader::getMessagesAt(uint64_t timestamp, const std::vector<TopicInfo> &visualize_topics) const
{
    //std::cout << "start of function" << std::endl;
    std::vector<TopicMessage> result;

    for (const auto &topicInfo : visualize_topics)
    {
        auto it = m_topicMessages.find(topicInfo);
        if (it != m_topicMessages.end())
        {
            const auto &messages = it->second;
            auto maybeMsg = getMessageAtTime(messages, timestamp);
            if (maybeMsg.has_value())
            {
                result.push_back(TopicMessage{
                    .topic_name = topicInfo.name,
                    .message = maybeMsg.value()});
            }
        }
    }

    return result;
}

void BagLoader::open(std::vector<TopicInfo> &selectedTopics, uint64_t startTime, uint64_t endTime)
{
    //m_selected_topics = selectedTopics;
    m_selected_start_time = m_start_time;
    m_selected_end_time = m_end_time;

    if (!m_inited)
    {
        std::cerr << "BagLoader not initialized. Call init() first." << std::endl;
        return;
    }

    if (sqlite3_open(m_db3Path.c_str(), &m_db) != SQLITE_OK)
    {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    std::unordered_map<std::string, int> topicIdMap;
    const char *topicSql = "SELECT id, name FROM topics;";
    sqlite3_stmt *topicStmt;
    if (sqlite3_prepare_v2(m_db, topicSql, -1, &topicStmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(topicStmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(topicStmt, 0);
            const unsigned char *name = sqlite3_column_text(topicStmt, 1);
            if (name)
            {
                topicIdMap[reinterpret_cast<const char *>(name)] = id;
            }
        }
        sqlite3_finalize(topicStmt);
    }

    const char *msgSql = "SELECT timestamp, data FROM messages WHERE topic_id = ? AND timestamp BETWEEN ? AND ?;";
    sqlite3_stmt *msgStmt;
    if (sqlite3_prepare_v2(m_db, msgSql, -1, &msgStmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare message query: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    for (const auto &topic : selectedTopics)
    {
        auto it = topicIdMap.find(topic.name);
        if (it == topicIdMap.end())
        {
            std::cerr << "Topic not found in DB: " << topic.name << std::endl;
            continue;
        }

        int topicId = it->second;

        sqlite3_reset(msgStmt);
        sqlite3_bind_int(msgStmt, 1, topicId);
        sqlite3_bind_int64(msgStmt, 2, startTime);
        sqlite3_bind_int64(msgStmt, 3, endTime);

        std::vector<BagMessage> messages;
        while (sqlite3_step(msgStmt) == SQLITE_ROW)
        {
            uint64_t timestamp = sqlite3_column_int64(msgStmt, 0);
            const void *blob = sqlite3_column_blob(msgStmt, 1);
            int size = sqlite3_column_bytes(msgStmt, 1);

            std::vector<uint8_t> data(reinterpret_cast<const uint8_t *>(blob),
                                      reinterpret_cast<const uint8_t *>(blob) + size);

            messages.push_back({timestamp, std::move(data)});
        }

        m_topicMessages[topic] = std::move(messages);
    }

    sqlite3_finalize(msgStmt);
}
