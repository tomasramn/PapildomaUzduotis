#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
using namespace std;
const char symbols[] = ",.:;\"!?–_`~&^%$#\n\t*()[]@#=+-„“";
vector<string> website;
map<string, int> words;
multimap<string, int> lines;
void countwords(vector<string> list)
{
    map<string, int>::iterator it;
    ofstream ofs("results.txt");
    for (string word : list) {
        it = words.find(word);
        if (it != words.end()) {
            words[word]++;
        }
        else {
            words.insert({ word, 1 });
        }
    }
    for (it = words.begin(); it != words.end(); ++it) {
        if (it->second > 1) {
            ofs << left << setw(3) << to_string(it->second) << it->first << endl;
        }
    }
}
void table(int rowline) //sukuria lentele
{
    multimap<string, int>::iterator it;
    ofstream ofs("lentele.txt");
    ofs << "|";
    for (int i = 1; i <= rowline; i++) {
        ofs << left << setw(5) << to_string(i) << "|";
    }
    ofs << endl;
    for (auto x : words) {
        if (x.second > 1) {
            ofs << "|";
            for (int i = 1; i <= rowline; i++) {
                if (lines.count(x.first) > 0 && x.second == i) {
                    ofs << "  x  |";
                }
                else ofs << "     |";
            }
            ofs << x.first << "\n";
        }
    }
    ofs << "\n";
}
bool iswebsite(string word) //suranda nuoroda ir ja iraso i vektoriu
{
    if (word.rfind("http", 0) == 0 || word.rfind("www", 0) == 0) {
        website.push_back(word);
        return true;
    }
    else
        return false;
}
void printwebsite() //atspausdina tekste rastu puslapiu sarasa
{
    ofstream ofs("websites.txt");
    if (website.size() == 0) {
        ofs << "Puslapiu nerasta!" << endl;
    }
    else {
        for (auto x : website) {
            ofs << x << endl;
        }
    }
}
void removesymbols(string& word)
{
    for (int i = 0; i < sizeof(symbols); i++) {
        word.erase(remove(word.begin(), word.end(), symbols[i]), word.end());
    }
}
void collectwords(string& text)
{
    int row = 1;
    string word, sentence;
    istringstream a(text);
    ofstream ofs("results.txt");
    vector<string> list;
    do {
        cout << row << endl;
        getline(a, sentence);
        istringstream b(sentence);
        while (b) {
            b >> word;
            cout << row << " " << "Eilute" << " | " << " zodis: " << word << endl;
            if (!iswebsite(word)) {
                removesymbols(word);
                if (word.length() > 1) {
                    list.push_back(word);
                    lines.insert({ word, row });
                }
            }
            if (b.eof()) break;
        }
        row++;
    } while (a);
    countwords(list);
}
int main()
{
    ifstream ifs("vilnius.txt");
    string text((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    const int rows = count(text.begin(), text.end(), '\n');
    collectwords(text); table(rows); printwebsite();
}