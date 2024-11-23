#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

using Sequence = std::vector<std::vector<int>>;
using Dataset = std::vector<Sequence>;

struct Node {
    std::vector<int> prefix;
    Sequence* successors;
    std::vector<int>* extensions;
};

// Helper function to display sequences
void printSequences(const Dataset& sequences) {
    for (const auto& seq : sequences) {
        std::cout << "[";
        for (int i = 0; i < seq.size(); i++) {
            std::cout << "{";
            for (int j = 0; j < seq[i].size(); j++)
            {
                std::cout << seq[i][j];
                if (j < seq[i].size() - 1) std::cout << ", ";
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

int prefixSpan(const Dataset& dataset, const Sequence& prefix, int minSup) {
    //find frequent items in dataset
    std::map<std::vector<int>, int> frequentItems = findFrequent(dataset, minSup);
    unsigned long long count = 0;
    //iterate over each frequent item
    for (const auto& item : frequentItems) {
        Sequence newPrefix = prefix;
        newPrefix.push_back(item.first);
        ++count;

        //output for debugging
        std::cout << "Frequent Pattern: ";
        printSequences({newPrefix});

        //project database from the current prefix
        Dataset projectedDatabase = projectDatabase(dataset, item.first);

        //recursive call
        if (!projectedDatabase.empty()) {
            count += prefixSpan(projectedDatabase, newPrefix, minSup);
        }
    }
    return count;
}

int handleArgs(int argc, char** argv) {
    //the following 'if' is a debug line and should be removed at the end
    if (argc > 1) {
        std::string num = argv[1];
        return std::stoi(num);
    }
    return INT_MAX;
}

Dataset processInput() {
    Dataset dataset;
    dataset.push_back(Sequence());
    dataset[0].push_back(std::vector<int>());
    int i = 0, j = 0;
    int num;
    while (std::cin >> num) {
        if (num == -2) {
            dataset.push_back(Sequence());
            dataset[i].pop_back();
            i++;
            j = 0;
            dataset[i].push_back(std::vector<int>());
        }
        else if (num == -1) {
            dataset[i].push_back(std::vector<int>());
            j++;
        }
        else {
            dataset[i][j].push_back(num);
        }
    }
    dataset.pop_back();
    return dataset;
}

void readDictionary(std::map<std::string, int>& w, std::map<std::string, int>& l) {
    std::string tmp = "";
    std::ifstream openFile("dictionary.txt");
    while (std::getline(openFile, tmp)) {
        size_t it = tmp.find(":");
        std::string first = tmp.substr(0, it);
        int second = std::stoi(tmp.substr(it + 1));
        if (first[0] == '+') {
            w[first] = second;
        } else {
            l[first] = second;
        }
    }
    openFile.close();
}

int main(int argc, char** argv) {
    int minSupport = handleArgs(argc, argv);
    //generate tree
    Dataset dataset = processInput();
    std::map<std::string, int> wActions;
    std::map<std::string, int> lActions;
    readDictionary(wActions, lActions);
    Sequence prefix(1);
    for (auto& action : wActions) {
        prefix[0].push_back(action.second);
    }
    for (auto& action : lActions) {
        prefix[0].push_back(action.second);
    }
    for (int i = 0; i < prefix[0].size(); i++)
    {
        std::cout << prefix[0][i] << " ";
    }
    std::cout << std::endl;
    std::cout << "running prefixspan with minsupport = " << minSupport << std::endl;
    //output tree
    unsigned long long count = prefixSpan(dataset, prefix, minSupport);
    std::cout << count << " patterns found." << std::endl;
}