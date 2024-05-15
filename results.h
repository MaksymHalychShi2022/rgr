#include <vector>
#include <fstream>
#include <iostream>
#include <string>


class Results {
private:
    struct ResultRecord {
        std::string size, ntreads, time;
    };

    std::vector<ResultRecord> results;
public:
    Results() {
        results = std::vector<ResultRecord>();
    }

    void addRecord(size_t rows, size_t cols, size_t ntreads, size_t time, bool logging = true) {
        results.push_back({std::to_string(rows) + "x" + std::to_string(cols),
                           std::to_string(ntreads),
                           std::to_string(time)
                          });
        if (logging) {
            std::cout << results[results.size()-1].size << ",";
            std::cout << results[results.size()-1].ntreads << ",";
            std::cout << results[results.size()-1].time << "\n";
        }
    }

    void addRecord(size_t rows, size_t cols, const std::string &ntreads, size_t time, bool logging = true) {
        results.push_back({std::to_string(rows) + "x" + std::to_string(cols),
                           ntreads,
                           std::to_string(time)
                          });
        if (logging) {
            std::cout << results[results.size() - 1].size << ",";
            std::cout << results[results.size() - 1].ntreads << ",";
            std::cout << results[results.size() - 1].time << "\n";
        }
    }

    void writeToCSV(const char *fileName) {
        std::ofstream outFile(fileName);
        if (outFile.is_open()) {
            outFile << "size,ntreads,time(microseconds)\n";

            for (auto &it: results) {
                outFile << it.size << ',' << it.ntreads << "," << it.time << "\n";
            }

            outFile.close();
            std::cout << "Results was written to " << fileName << "\n";
        } else {
            std::cout << "Error while writing to file " << fileName << "\n";
        }
    }
};