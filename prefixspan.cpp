#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

struct seqPattern {
    std::string prefix;
    std::vector<std::string> suffixes;
    int count = suffixes.size();
    std::vector<seqPattern>* successors;
};

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
    for (std::vector<int> sequence : dataset) {
        auto it = find(sequence.begin(), sequence.end(), prefix);
        if (it != sequence.end() && ++it != sequence.end()) {
            std::vector<int> projectedSequence(it, sequence.end());
            projectedDatabase.push_back(projectedSequence);
        }
    }
    return projectedDatabase;
}

std::map<int, int> findFrequent(std::vector<std::vector<int>>& dataset, int minSupport) {
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

void prefixSpan(std::vector<std::vector<int>>& dataset, const std::vector<int>& prefix, unsigned long long minSup) {
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

void getActionsFromDict(std::vector<std::string>& buildActions) {
    std::ifstream myFile("dictionary.txt");
    std::string tmp;
    if (myFile.is_open()) {
        while (myFile.good()) {
            std::getline(myFile, tmp);
            std::string num = "";
            for (char c : tmp) {
                if (std::isdigit(c)) {
                    num += c;
                }
            }
            buildActions.push_back(num);
        }
        myFile.close();
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

int main(int argSup) {
    //generate tree
    unsigned long long minSupport = argSup;
    std::vector<std::string> data;
    std::string tmp = "";
    while (std::getline(std::cin >> std::ws, tmp)) {
        data.push_back(tmp);
    }
    for (std::string s : data) std::cout << s << std::endl << std::endl;
    std::vector<std::vector<int>> dataset = convertToInts(data);
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