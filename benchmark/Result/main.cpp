#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include "QueryResult.h"

using namespace std;
namespace po = boost::program_options;

const unsigned long DEFAULT_RELEVANT_ITEM = 7;
const double DEFAULT_START_THRESHOLD = 0;
const double DEFAULT_END_THRESHOLD =  0.5;
const double DEFAULT_STEP_THRESHOLD =  0.03125;

struct RisStatistic {
    double threshold;
    double total_precision = 0;
    double total_recall = 0;
    double total_f1measure = 0;

    RisStatistic(double t) : threshold(t) {}
};

bool ExtractQuery(const string &line, string &query);

bool ExtractNumberResults(const string &line, int &number_files);

void ExtractResult(const string &line, double &dist, string &result_file);

void ProcessQueries(unsigned long relevant_item, double start_threshold, double end_threshold, double step_threshold);

/**
 * Compute the precision, recall and f-measure of a query for a given threshold.
 * The threshold is given through the RisStatistic parameter.
 * The statistics are directly updated in the RisStatistic parameter.
 * @param query A query
 * @param relevant_item The number of relevant items in the query
 * @param statistics A statistic object
 */
void ComputeQueryStatistics(const QueryResult& query, unsigned long relevant_item, RisStatistic& statistics);

int main(int argc, const char *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    try {
        double start_threshold, end_threshold, step_threshold;
        unsigned long relevant_item;
        // Declare the supported command line options.
        po::options_description options_desc("./RisBenchmarkResult [options]\nAllowed options:");
        options_desc.add_options()
                ("help,h", "Display a help message")
                ("relevant_items,r", po::value<unsigned long>(&relevant_item)->default_value(DEFAULT_RELEVANT_ITEM), "Relevant items in a normal query")
                ("start_threshold,s", po::value<double>(&start_threshold)->default_value(DEFAULT_START_THRESHOLD), "Set start threshold")
                ("end_threshold,e", po::value<double>(&end_threshold)->default_value(DEFAULT_END_THRESHOLD), "Set end threshold")
                ("step_threshold,t", po::value<double>(&step_threshold)->default_value(DEFAULT_STEP_THRESHOLD), "Set step threshold");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(options_desc).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << options_desc << endl;
            return EXIT_SUCCESS;
        }

        ProcessQueries(relevant_item, start_threshold, end_threshold, step_threshold);
    }
    catch (exception &e) {
        cerr << "error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void ProcessQueries(unsigned long relevant_item, double start_threshold, double end_threshold, double step_threshold) {
    unsigned long total_queries = 0;
    vector<RisStatistic> statistics;

    // Initialise a RisStatistic entry for each threshold
    for (double t = start_threshold; t <= end_threshold; t += step_threshold) {
        statistics.push_back(RisStatistic(t));
    }

    // Read each line of std::cin
    string line_query, line_file, line_result;
    while (getline(cin, line_query)) {
        string query_file, result_file;
        int number_results;
        double dist;

        if (ExtractQuery(line_query, query_file)) {
            QueryResult query(query_file);

            getline(cin, line_file);
            if (ExtractNumberResults(line_file, number_results)) {
                for (int i = 0; i < number_results; i++) {
                    getline(cin, line_result);
                    ExtractResult(line_result, dist, result_file);

                    query.AddResult(dist, result_file);
                }

                // For each threshold the statistics are updated with the new query.
                // #pragma omp parallel for shared(query, relevant_item)
                for (unsigned int i = 0; i < statistics.size(); i++) {
                    ComputeQueryStatistics(query, relevant_item, statistics[i]);
                }
                total_queries++;
            }
        }
    }

    // Compute the mean precision, recall and f measure
    for (unsigned int i = 0; i < statistics.size(); i++) {
        double mean_precision, mean_recall, mean_f1measure;

        mean_precision = statistics[i].total_precision / total_queries;
        mean_recall = statistics[i].total_recall / total_queries;
        mean_f1measure = statistics[i].total_f1measure / total_queries;

        cout << statistics[i].threshold << "\t" << mean_precision << "\t" << mean_recall << "\t" << mean_f1measure << endl;
    }
}

void ComputeQueryStatistics(const QueryResult& query, unsigned long relevant_item, RisStatistic& statistics) {
    double precision, recall, f1measure;

    unsigned long relevant = relevant_item;
    unsigned long retrieved_relevant = query.NumberRelevantRetrieved(statistics.threshold);
    unsigned long retrieved = query.NumberRetrieved(statistics.threshold);

    // If the query image is non indexed, there is no relevant image in the database.
    if (query.QueryType().compare("non-indexed") == 0) {
        relevant = 0;
    }

    if (retrieved > 0) {
        precision = static_cast<double>(retrieved_relevant) / retrieved;
    } else {
        precision = 1.0;
    }

    if (relevant > 0) {
        recall = static_cast<double>(retrieved_relevant) / relevant;
    } else {
        recall = 1.0;
    }

    if (precision + recall > 0) {
        f1measure = 2 * precision * recall / (precision + recall);
    } else {
        f1measure = 0;
    }

    statistics.total_precision += precision;
    statistics.total_recall += recall;
    statistics.total_f1measure += f1measure;
}

bool ExtractQuery(const string &line, string &query) {
    // The line begins with "Query :"
    if (line.find("Query : ") == 0) {
        query = line.substr(8);
        return true;
    }
    return false;
}

bool ExtractNumberResults(const string &line, int &number_results) {
    // The line begins with "Files found : "
    if (line.find("Files found : ") == 0) {
        number_results = stoi(line.substr(14));
        return true;
    }
    return false;
}

void ExtractResult(const string &line, double &dist, string &result_file) {
    unsigned long pos = line.find(':');
    dist = stod(line.substr(0, pos));
    result_file = line.substr(pos + 2);
}