#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <limits>

// Helper function to display sequences
void printSequences(const std::vector<std::vector<int>>& sequences) {
    for (const auto& seq : sequences) {
        std::cout << "[";
        for (size_t i = 0; i < seq.size(); ++i) {
            std::cout << seq[i];
            if (i < seq.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

// project the dataset based on a prefix
std::vector<std::vector<int>> projectDatabase(const std::vector<std::vector<int>>& dataset, int prefix) {
    std::vector<std::vector<int>> projectedDatabase;
    for (const std::vector<int>& sequence : dataset) {
        auto it = find(sequence.begin(), sequence.end(), prefix);
        if (it != sequence.end() && ++it != sequence.end()) {
            std::vector<int> projectedSequence(it, sequence.end());
            projectedDatabase.push_back(projectedSequence);
        }
    }
    return projectedDatabase;
}

// find patterns occuring >= the support
std::map<int, int> findFrequent(const std::vector<std::vector<int>>& dataset, int minSupport) {
    std::map<int, int> frequency;
    for (const std::vector<int>& sequence : dataset) {
        std::set<int> uniqueItems(sequence.begin(), sequence.end());
        for (int item : uniqueItems) {
            frequency[item]++;
        }
    }
    std::map<int, int> frequentItems;
    for (const auto& item : frequency) {
        if (item.second >= minSupport) {
            frequentItems[item.first] = item.second;
        }
    }
    return frequentItems;
}

void prefixSpan(const std::vector<std::vector<int>>& dataset, const std::vector<int>& prefix, int minSup) {
    //find frequent items in dataset
    std::map<int, int> frequentItems = findFrequent(dataset, minSup);
    //iterate over each frequent item
    for (const auto& item : frequentItems) {
        std::vector<int> newPrefix = prefix;
        newPrefix.push_back(item.first);

        //output for debugging
        std::cout << "Frequent Pattern: ";
        printSequences({newPrefix});

        //project database from the current prefix
        std::vector<std::vector<int>> projectedDatabase = projectDatabase(dataset, item.first);

        //recursive call
        if (!projectedDatabase.empty()) {
            prefixSpan(projectedDatabase, newPrefix, minSup);
        }
    }
}

std::vector<std::vector<int>> convertToInts(const std::vector<std::string>& data) {
    std::string tmp = "";
    std::vector<std::vector<int>> dataset;
    for (int i = 0; i < data.size(); i++) {
        dataset.push_back(std::vector<int>());
        int j = 0;
        while (j < data[i].length())
        {
            if (data[i][j] == '-') {
                j += 3;
                continue;
            }
            if (data[i][j] == ' ' && tmp != "") {
                dataset[i].push_back(std::stoi(tmp));
                tmp = "";
            }
            else {
                tmp += data[i][j];
            }
            j++;
        }
        
    }
    return dataset;
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
    std::vector<std::string> data;
    std::string tmp = "";
    while (std::getline(std::cin >> std::ws, tmp)) {
        data.push_back(tmp);
    }
    for (std::string s : data) std::cout << s << std::endl << std::endl;
    std::vector<std::vector<int>> dataset = convertToInts(data);
    //debug
    for (int i = 0; i < dataset.size(); i++)
    {
        for (int j = 0; j < dataset[i].size(); j++)
        {
            std::cout << dataset[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::vector<int> prefix;
    std::cout << "running prefixspan with minsupport = " << minSupport << std::endl;
    //output tree
    prefixSpan(dataset, prefix, minSupport);
}