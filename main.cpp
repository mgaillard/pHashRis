#include <iostream>

#include "pHashRis.h"

using namespace std;

const string DATABASE = "index";

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Run pHash <command> <path>" << endl
             << "Commands:" << endl
             << "index: Index a single or many images in a directory" << endl
             << "search: Search images into the index" << endl;

        return 1;
    }

    string command(argv[1]);
    string path(argv[2]);

    pHashRis app(DATABASE);
    app.LoadIndex();

    if (command.compare("index") == 0) {
        app.Index(path);
        app.SaveIndex();
    } else if (command.compare("search") == 0) {
        app.Search(path);
    } else {
        cout << "Unknown command." << endl;
    }

    return EXIT_SUCCESS;
}

