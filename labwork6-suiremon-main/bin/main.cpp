#include "lib/Parser.h"
#include "lib/Archiver.h"
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
    Arguments arguments;
    Arguments::Parse(arguments, argc, argv);
    if (arguments.operation == "create") {
        Create(arguments.archName, arguments.filenames);
    } else if (arguments.operation == "append") {
        Append(arguments.archName, arguments.filenames);
    } else if (arguments.operation == "list") {
        List(arguments.archName);
    } else if (arguments.operation == "extract") {
        Extract(arguments.archName, arguments.filenames);
    } else if (arguments.operation == "delete") {
        Delete(arguments.archName, arguments.filenames);
    } else if (arguments.operation == "concatenate") {
        Concat(arguments.archName, arguments.secondArchName);
    }
    return 0;
}