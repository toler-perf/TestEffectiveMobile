#include "List.h"


bool List::DeserializeFromText(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open input file: " << filename << std::endl;
        return false;
    }

    std::vector<std::pair<std::string, int>> nodes_data;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t delimiter_pos = line.find_last_of(';');
        if (delimiter_pos == std::string::npos) {
            std::cerr << "Invalid line format: " << line << std::endl;
            return false;
        }

        std::string data = line.substr(0, delimiter_pos);
        std::string rand_index_str = line.substr(delimiter_pos + 1);

        int rand_index = std::stoi(rand_index_str);
        nodes_data.emplace_back(data, rand_index);
    }

    file.close();

    count = nodes_data.size();
    if (count == 0) return true;

    head = new ListNode();
    head->data = nodes_data[0].first;

    ListNode* current = head;
    std::vector<ListNode*> nodes_ptrs;
    nodes_ptrs.push_back(current);

    for (size_t i = 1; i < count; ++i) {
        ListNode* node = new ListNode();
        node->data = nodes_data[i].first;

        node->prev = current;
        current->next = node;

        current = node;
        nodes_ptrs.push_back(node);
    }

    tail = current;

    for (size_t i = 0; i < count; ++i) {
        int rand_index = nodes_data[i].second;
        if (rand_index >= 0 && rand_index < static_cast<int>(count)) {
            nodes_ptrs[i]->rand = nodes_ptrs[rand_index];
        }
        else {
            nodes_ptrs[i]->rand = nullptr;
        }
    }

    return true;
}

void List::Serialize(const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open output file: " << filename << std::endl;
        return;
    }

    std::vector<ListNode*> nodes_ptrs;
    ListNode* current = head;
    while (current) {
        nodes_ptrs.push_back(current);
        current = current->next;
    }

    uint32_t node_count = nodes_ptrs.size();
    file.write(reinterpret_cast<char*>(&node_count), sizeof(node_count));

    for (size_t i = 0; i < node_count; ++i) {
        ListNode* node = nodes_ptrs[i];

        uint32_t data_len = node->data.size();
        file.write(reinterpret_cast<char*>(&data_len), sizeof(data_len));

        file.write(node->data.c_str(), data_len);

        int32_t rand_index = -1;
        if (node->rand) {
            for (size_t j = 0; j < node_count; ++j) {
                if (nodes_ptrs[j] == node->rand) {
                    rand_index = j;
                    break;
                }
            }
        }
        file.write(reinterpret_cast<char*>(&rand_index), sizeof(rand_index));
    }

    file.close();
}

bool List::Deserialize(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open binary file: " << filename << std::endl;
        return false;
    }

    Clear();

    uint32_t node_count = 0;
    file.read(reinterpret_cast<char*>(&node_count), sizeof(node_count));

    if (node_count == 0) return true;

    std::vector<std::pair<std::string, int>> nodes_data;

    for (uint32_t i = 0; i < node_count; ++i) {
        uint32_t data_len = 0;
        file.read(reinterpret_cast<char*>(&data_len), sizeof(data_len));

        std::string data(data_len, '\0');
        file.read(&data[0], data_len);

        int32_t rand_index = 0;
        file.read(reinterpret_cast<char*>(&rand_index), sizeof(rand_index));

        nodes_data.emplace_back(data, rand_index);
    }

    file.close();

    count = nodes_data.size();

    head = new ListNode();
    head->data = nodes_data[0].first;

    ListNode* current = head;
    std::vector<ListNode*> nodes_ptrs;
    nodes_ptrs.push_back(current);

    for (size_t i = 1; i < count; ++i) {
        ListNode* node = new ListNode();
        node->data = nodes_data[i].first;

        node->prev = current;
        current->next = node;

        current = node;
        nodes_ptrs.push_back(node);
    }

    tail = current;

    for (size_t i = 0; i < count; ++i) {
        int rand_index = nodes_data[i].second;
        if (rand_index >= 0 && rand_index < static_cast<int>(count)) {
            nodes_ptrs[i]->rand = nodes_ptrs[rand_index];
        }
    }

    return true;
}
