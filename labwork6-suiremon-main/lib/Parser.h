#include <string>
#include <utility>
#include <vector>

struct Arguments {
    std::string archName;
    std::string secondArchName;
    std::vector<std::string> filenames;
    unsigned char controlBits{};
    std::string operation;
    static void Parse(Arguments&, int, char**);
};
