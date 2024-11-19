#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <limits>

using Sequence = std::vector<std::vector<int>>;
using Dataset = std::vector<Sequence>;

// Helper function to display sequences
void printSequences(const Dataset& sequences) {
    for (const auto& seq : sequences) {
        std::cout << "[";
        for (int i = 0; i < seq.size(); i++) {
            std::cout << "{";
            for (int j = 0; j < seq[i].size(); j++)
            {
                std::cout << seq[i][j];
                if (i < seq[i].size() - 1) std::cout << ", ";
            }
            std::cout << "}";
            if (i < seq.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

// project the dataset based on a prefix
Dataset projectDatabase(const Dataset& dataset, const std::vector<int>& prefix) {
    Dataset projectedDatabase;
    for (const auto& sequence : dataset) {
        for (int i = 0; i < sequence.size(); i++) {
            if (sequence[i] == prefix) {
                if (i + 1 < sequence.size()) {
                    Sequence projectedSequence(sequence.begin() + i + 1, sequence.end());
                    projectedDatabase.push_back(projectedSequence);
                }
                break;
            }
        }
    }
    return projectedDatabase;
}

// find patterns occuring >= the support
std::map<std::vector<int>, int> findFrequent(const Dataset& dataset, int minSupport) {
    std::map<std::vector<int>, int> frequency;
    for (const auto& sequence : dataset) {
        std::set<std::vector<int>> uniqueItems(sequence.begin(), sequence.end());
        for (const auto& itemset : uniqueItems) {
            frequency[itemset]++;
        }
    }
    std::map<std::vector<int>, int> frequentItems;
    for (const auto& item : frequency) {
        if (item.second >= minSupport) {
            frequentItems[item.first] = item.second;
        }
    }
    return frequentItems;
}

void prefixSpan(const Dataset& dataset, const Sequence& prefix, int minSup) {
    //find frequent items in dataset
    std::map<std::vector<int>, int> frequentItems = findFrequent(dataset, minSup);
    //iterate over each frequent item
    for (const auto& item : frequentItems) {
        Sequence newPrefix = prefix;
        newPrefix.push_back(item.first);

        //output for debugging
        std::cout << "Frequent Pattern: ";
        printSequences({newPrefix});

        //project database from the current prefix
        Dataset projectedDatabase = projectDatabase(dataset, item.first);

        //recursive call
        if (!projectedDatabase.empty()) {
            prefixSpan(projectedDatabase, newPrefix, minSup);
        }
    }
}

int handleArgs(int argc, char** argv) {
    //the following 'if' is a debug line and should be removed at the end
    if (argc > 1) {
        std::string num = argv[1];
        return std::stoi(num);
    }
    return INT_MAX;
}

int main(int argc, char** argv) {
    std::cout << "Have " << argc << " arguments:\n";
    int minSupport = handleArgs(argc, argv);
    //generate tree
    Dataset dataset;
    dataset.push_back(Sequence());
    dataset[0].push_back(std::vector<int>());
    int i = 0, j = 0;
    int num;
    while (std::cin >> num) {
        if (num == -2) {
            dataset.push_back(Sequence());
            dataset[i].pop_back();
            std::cout << "new sequence" << std::endl;
            i++;
            j = 0;
            dataset[i].push_back(std::vector<int>());
        }
        else if (num == -1) {
            dataset[i].push_back(std::vector<int>());
            std::cout << "new itemset" << std::endl;
            j++;
        }
        else {
            dataset[i][j].push_back(num);
            std::cout << "number entered" << std::endl;
        }
        printSequences(dataset);
    }
    dataset.pop_back();
    //debug
    Sequence prefix;
    std::cout << "running prefixspan with minsupport = " << minSupport << std::endl;
    //output tree
    prefixSpan(dataset, prefix, minSupport);
}