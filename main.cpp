#include <iostream>
#include <boost/program_options.hpp>
#include "Ris.h"
#include "pHashRis.h"
#include "DCTpHash.h"
#include "MHpHash.h"
#include "RVpHash.h"

using namespace std;
namespace po = boost::program_options;

const string DEFAULT_DATABASE = "index";
const string DEFAULT_HASHTYPE = "DCT";
const double DEFAULT_THRESHOLD = 0.125;

int main(int argc, const char *argv[]) {
    try {
        double threshold;
        string database;
        string hash_type;
        // Declare the supported command line options.
        po::options_description options_desc("./pHashRis [options] <command> <path>\nAllowed options:");
        options_desc.add_options()
                ("help,h", "Display a help message")
                ("command,c", po::value<string>()->required(), "Set the command to execute:\n"
                        "index: \tIndex a file or the content of a directory\n"
                        "search: \tSearch a file or the content of a directory into the index\n"
                        "statistics: \tDisplay statistical information about the distances between the indexed images\n"
                        "distances: \tDisplay the pair of image whose distance is less than the threshold\n")
                ("path,p", po::value<string>(), "Set the path to the dir or file to process. This option is required for the index and search command.")
                ("database,d", po::value<string>(&database)->default_value(DEFAULT_DATABASE), "Set database file path")
                ("hashtype,f", po::value<string>(&hash_type)->default_value(DEFAULT_HASHTYPE), "Set the perceptual image hash type:\n"
                        "DCT: \tDiscrete Cosine Transform based hash (default)\n"
                        "MH: \tMarr-Hildreth Operator based hash\n"
                        "RV: \tRadial Variance based hash")
                ("threshold,t", po::value<double>(&threshold)->default_value(DEFAULT_THRESHOLD), "Set distance threshold for search");

        po::positional_options_description pos_options_desc;
        pos_options_desc.add("command", 1);
        pos_options_desc.add("path", 1);

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, options_desc), vm);
        po::store(po::command_line_parser(argc, argv).options(options_desc)
                                                     .positional(pos_options_desc)
                                                     .run(), vm);

        if (argc == 1 or vm.count("help")) {
            cout << options_desc << endl;
            return EXIT_SUCCESS;
        }

        po::notify(vm);

        if (vm.count("command")) {
            const string& command = vm["command"].as<string>();

            Ris* app;

            if (hash_type.compare("MH") == 0) {
                app  = new pHashRis<MHpHash, MHpHashDistance, MHpHashFunction>(database, threshold);
            } else if (hash_type.compare("RV") == 0) {
                app  = new pHashRis<RVpHash, RVpHashDistance, RVpHashFunction>(database, threshold);
            } else {
                app  = new pHashRis<DCTpHash, DCTpHashDistance, DCTpHashFunction>(database, threshold);
            }

            app->LoadIndex();

            if (command.compare("index") == 0) {
                if (vm.count("path")) {
                    app->Index(vm["path"].as<string>());
                    app->SaveIndex();
                } else {
                    cerr << "Error: the --path option is required for the index command." << endl;
                }
            } else if (command.compare("search") == 0) {
                if (vm.count("path")) {
                    app->Search(vm["path"].as<string>());
                } else {
                    cerr << "Error: the --path option is required for the search command." << endl;
                }
            } else if (command.compare("statistics") == 0) {
                app->DisplayIndexStatistics();
            } else if (command.compare("distances") == 0) {
                double max_distance = threshold;
                app->DisplayDistances(max_distance);
            } else {
                cerr << "Error: unknown command. For more information, see the documentation." << endl;
            }

            delete app;
        }
    }
    catch (exception &e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return EXIT_SUCCESS;
}

