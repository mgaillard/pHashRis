#include <iostream>

#include "pHashRis.h"

using namespace std;

const string DATABASE = "index";

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Run pHash <command> <path>" << endl
             << "Commands:" << endl
             << "index: Index a single or many images in a directory" << endl
             << "search: Search images into the index" << endl
             << "distances: Display the distances between the indexed images" << endl;

        return 1;
    }

    string command(argv[1]);

    pHashRis app(DATABASE);
    app.LoadIndex();

    if (command.compare("index") == 0) {
        string path(argv[2]);
        app.Index(path);
        app.SaveIndex();
    } else if (command.compare("search") == 0) {
        string path(argv[2]);
        app.Search(path);
    } else if (command.compare("distances") == 0) {
        int max_distance = static_cast<int>(strtol(argv[2], NULL, 10));
        app.Distances(max_distance);
    } else {
        cout << "Unknown command." << endl;
    }

    return EXIT_SUCCESS;
}

