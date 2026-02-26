#include "List.h"

int main() {
    List myList;

    if (myList.DeserializeFromText("inlet.in")) {
        std::cout << "List built from text file:" << std::endl;
        myList.Print();

        myList.Serialize("outlet.out");
        std::cout << "\nList serialized to binary file." << std::endl;

        List newList;
        if (newList.Deserialize("outlet.out")) {
            std::cout << "\nList deserialized from binary file:" << std::endl;
            newList.Print();
        }
    }

    return 0;
}