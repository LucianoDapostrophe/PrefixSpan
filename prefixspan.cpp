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
};

// Helper function to display sequences
void printSequencesToConsole(const Dataset& sequences, int count) {
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
        std::cout << "]" << " Count is " << count << std::endl;
    }
}

//store data for processing
void printSequencesToCsv(const Dataset& sequences, int count) {
    std::ofstream file("patterns.txt", std::ios::app);
    for (const auto& seq : sequences) {
        file << "[";
        for (int i = 0; i < seq.size(); i++) {
            file << "{";
            for (int j = 0; j < seq[i].size(); j++) {
                file << seq[i][j];
                if (j < seq[i].size() - 1) file << ", ";
            }
            file << "}";
            if (i < seq.size() - 1) file << ", ";
        }
        file << "]" << count << "\n";
    }
    file.close();
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

        //output
        std::cout << "Frequent Pattern: ";
        printSequencesToConsole({newPrefix}, item.second);
        printSequencesToCsv({newPrefix}, item.second);


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

void readDictionary(std::map<int, std::string>& w, std::map<int, std::string>& l) {
    std::string tmp = "";
    std::ifstream openFile("dictionary.txt");
        if (!openFile.is_open()) {
        std::cerr << "file dictionary failed to open" << std::endl;
    }
    while (std::getline(openFile, tmp)) {
        size_t it = tmp.find(":");
        std::string second = tmp.substr(0, it);
        int first = std::stoi(tmp.substr(it + 1));
        if (second[0] == '+') {
            w[first] = second;
        } else {
            l[first] = second;
        }
    }
    openFile.close();
}

Dataset processCSV() {
    Dataset dataset;
    std::string strSequence;
    std::ifstream openFile("patterns.txt");
    if (!openFile.is_open()) {
        std::cerr << "file failed to open" << std::endl;
    }
    while (std::getline(openFile, strSequence)) {
        int i = -1;
        std::string tmp = "";
        Sequence s;
        for (char c : strSequence) {
            if (std::isdigit(c)) {
                tmp += c;
            }
            else if (c == '{') {
                s.push_back(std::vector<int>());
                i++;
            }
            else if (tmp != "") {
                s[i].push_back(std::stoi(tmp));
                tmp = "";
            }
        }
        dataset.push_back(s);
    }
    openFile.close();
    dataset.pop_back();
    return dataset;
}

void prefixSpanNaive(const Dataset& dataset, const std::map<int, std::string>& wActions, const std::map<int, std::string>& lActions) {
    std::set<std::vector<std::vector<int>>> patterns(dataset.begin(), dataset.end());
    for (const auto& sequence : dataset) {
        Sequence dualSequence;
        for (int i = 0; i < sequence.size(); i++) {
            dualSequence.push_back(std::vector<int>());
            for (int j = 0; j < sequence[i].size(); j++) {
                auto it = wActions.find(sequence[i][j]);
                if (it != wActions.end()) {
                    std::string action = it->second;
                    action[0] = '-';
                    bool match = false;
                    for (auto& value : lActions) {
                        if (value.second == action) {
                            dualSequence[i].push_back(value.first);
                            match = true;
                            break;
                        }
                    }

                }
            }
        }
    }
}

int main(int argc, char** argv) {
    int minSupport = handleArgs(argc, argv);
    //generate tree
    Dataset dataset = processInput();
    Sequence prefix;
    std::cout << "running prefixspan with minsupport = " << minSupport << std::endl;
    //output tree
    unsigned long long count = prefixSpan(dataset, prefix, minSupport);
    std::cout << count << " patterns found." << std::endl;
    //compute frequent balanced patterns
    std::map<int, std::string> wActions;
    std::map<int, std::string> lActions;
    readDictionary(wActions, lActions);
    std::cout << "computing prefixspannaive to find balanced patterns." << std::endl;
    dataset = processCSV();
    //count = prefixSpanNaive(dataset, wActions, lActions);
}