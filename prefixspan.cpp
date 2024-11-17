#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

struct seqPattern {
    std::vector<int> prefix;
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
    //read dictionary
    std::map<int, std::string> dictionary;
    std::ifstream myFile("dictionary.txt");
    if (myFile.is_open()) {
        while (myFile.good()) {
            std::getline(myFile, tmp);
            std::cout << tmp << std::endl;
            //dictionary[tmp[tmp.size() - 1] - '0'] = tmp.substr()
        }
        myFile.close();
    }
    //output tree

}