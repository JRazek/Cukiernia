#include <iostream>
#include <vector>

using namespace std;


struct Container{
    const int id;
    int bestCandidate = 0; //0 for a, 1 for b, 2 for c. we assume its a unless proving its not.
    int productsPerCategory[3];
    Container(int id, int a, int b, int c): id(id){
        this->productsPerCategory[0] = a;
        this->productsPerCategory[1] = b;
        this->productsPerCategory[2] = c;
    }
    int getEffortForCategory(int catNum){
        switch (catNum) {
            case 0: return productsPerCategory[1] + productsPerCategory[2];
            case 1: return productsPerCategory[0] + productsPerCategory[2];
            case 2: return productsPerCategory[0] + productsPerCategory[1];
            default: cout<<"ERROR"; return -1;
        }
    }
};
vector<string> split(string str, char divider){
    vector<string> result;

    string currWord;
    for(int i = 0; i < str.size(); i ++){
        currWord += str[i];
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

    bool catPresent[3] = {false};

    for(int i = 0; i < containersCount; i ++){
        getline(cin, line);
        vector<string> args = split(line, ' ');
        int a = stoi(args[0]);
        int b = stoi(args[1]);
        int c = stoi(args[2]);

        catPresent[0] += a;
        catPresent[1] += b;
        catPresent[2] += c;

        containers.push_back(new Container(i, a, b , c));
    }

    int catContainers[3] = {0};
    catContainers[0] = containersCount;//all the containers are default to a

    for(int cat = 1; cat <= 2; cat++) {
        for (int i = 0; i < containersCount; i++) {
            Container * c = containers[i];
            if(c->getEffortForCategory(c->bestCandidate) > c->getEffortForCategory(cat)){
                catContainers[c->bestCandidate] -- ;
                c->bestCandidate = cat;
                catContainers[cat] ++;
            }
        }
    }





    int sum = 0;
    for(int i = 0; i < containersCount; i ++){
        Container * c = containers[i];
        sum += c->getEffortForCategory(c->bestCandidate);
    }
    cout<<sum;
    for(auto c : containers){
        delete c;
    }
    return 0;
}
