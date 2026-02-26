#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;

    std::string data;
};

class List {
public:
    List() : head(nullptr), tail(nullptr), count(0) {}

    ~List() {
        Clear();
    }

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    bool DeserializeFromText(const std::string& filename);
    void Serialize(const std::string& filename);
    bool Deserialize(const std::string& filename);

    void Print() {
        std::vector<ListNode*> nodes_ptrs;
        ListNode* current = head;
        while (current) {
            nodes_ptrs.push_back(current);
            current = current->next;
        }

        for (size_t i = 0; i < nodes_ptrs.size(); ++i) {
            std::cout << "Node " << i << ": data=\"" << nodes_ptrs[i]->data << "\"";

            if (nodes_ptrs[i]->rand) {
                // Находим индекс rand узла
                for (size_t j = 0; j < nodes_ptrs.size(); ++j) {
                    if (nodes_ptrs[j] == nodes_ptrs[i]->rand) {
                        std::cout << ", rand->" << j;
                        break;
                    }
                }
            }
            else {
                std::cout << ", rand->nullptr";
            }
            std::cout << std::endl;
        }
    }

private:
    void Clear() {
        ListNode* current = head;
        while (current) {
            ListNode* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        count = 0;
    }

    ListNode* head;
    ListNode* tail;
    size_t count;
};