#include <fstream>
#include <iostream>
#include <vector>
#include <string>
//#include <map>

struct seqPattern {
    std::string prefix;
    std::vector<std::string> suffixes;
    int count = suffixes.size();
    std::vector<seqPattern>* successors;
};

void prefixSpan(std::vector<std::string> sequences, std::vector<std::string> buildActions, unsigned long long minSup) {

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

int main(int argSup) {
    //generate tree
    unsigned long long minSupport = argSup;
    std::vector<std::vector<int>> dataset;
    std::vector<std::string> sequences;
    std::string tmp = "";
    std::cin.clear();
    std::cin.ignore(1, '\n');
    while (std::getline(std::cin >> std::ws, tmp)) {
        sequences.push_back(tmp);
    }
    std::cout << "sequence: " << sequences[0] << std::endl;
    //get possible actions, winner in first, loser in second
    std::vector<std::string> buildActions;
    getActionsFromDict(buildActions);
    for(std::string s : buildActions) {
        std::cout << s << std::endl;
    }
    std::cout << "running prefixspan with minsupport = " << minSupport << std::endl;
    prefixSpan(sequences, buildActions, minSupport);
    //output tree

}