#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

struct seqPattern {
    std::vector<char> prefix;
    std::vector<seqPattern>* suffixes;
};

void prefixSpan(std::map<int, std::vector<char>>* s, long double minSup) {

}

int main() {
    //generate tree
    //output tree

    std::map<std::string, int> dictionary;
    //parse output with dictionary
    std::ifstream myFile("dictionary.txt");
    if (myFile.is_open()) {

        myFile.close();
    }
}