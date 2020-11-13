#include <iostream>
#include <vector>

using namespace std;


struct Container{
    const int id;
    int bestCandidate = 0; //0 for a, 1 for b, 2 for c. we assume its a unless proving its not.
    int a;
    int b;
    int c;
    Container(int id, int a, int b, int c): id(id){
        this->a = a;
        this->b = b;
        this->c = c;
    }
    static bool compare(Container * c1, Container * c2, int category){
        return true;
    }
};
vector<string> split(string str, char divider){
    vector<string> result;

    string currWord = "";
    for(int i = 0; i < str.size(); i ++){
        currWord+=str[i];
        if(str[i] == divider || str.size()-1 == i){
            result.push_back(currWord);
            currWord = "";
        }
    }
    return result;
}

int main() {
    string line;
    getline(cin, line);

    int containersCount = stoi(line);

    vector<Container *> containers;
    for(int i = 0; i < containersCount; i ++){
        getline(cin, line);
        vector<string> args = split(line, ' ');
        int a = stoi(args[0]);
        int b = stoi(args[1]);
        int c = stoi(args[2]);
        containers.push_back(new Container(a, b , c));
    }

    for(auto c : containers){
        delete c;
    }
    return 0;
}
