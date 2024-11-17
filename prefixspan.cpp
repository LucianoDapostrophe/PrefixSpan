#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

struct seqPattern {
    std::string prefix;
    std::vector<seqPattern>* suffixes;
};

void prefixSpan(std::map<int, std::vector<char>>* s, long double minSup) {

}

int main() {
    //generate tree
    std::vector<std::string> sequences;
    std::string tmp = "";
    while (std::getline(std::cin >> std::ws, tmp)) {
        sequences.push_back(tmp);
    }
    std::vector<std::string> buildActions;
    std::ifstream myFile("dictionary.txt");
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
    for (std::string n : buildActions) {
        std::cout << n << std::endl;
    }
    //output tree

}