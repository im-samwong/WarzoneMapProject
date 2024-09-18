#include <iostream>

namespace Orders {
    void testOrdersLists();
}
namespace Cards {
    void testCards();
}

int main() {
    Orders::testOrdersLists();
    std::cout << std::endl;
    Cards::testCards();
    std::cout << std::endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}