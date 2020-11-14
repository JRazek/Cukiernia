#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


struct Container{
    const int id;
    long bestCandidate = 0; //0 for a, 1 for b, 2 for c. we assume its a unless proving its not.
    long productsPerCategory[3];
    Container(int id, long a, long b, long c): id(id){
        this->productsPerCategory[0] = a;
        this->productsPerCategory[1] = b;
        this->productsPerCategory[2] = c;
    }
    long long getEffortForCategory(int catNum){
        switch (catNum) {
            case 0: return productsPerCategory[1] + productsPerCategory[2];
            case 1: return productsPerCategory[0] + productsPerCategory[2];
            case 2: return productsPerCategory[0] + productsPerCategory[1];
            default: cout<<"ERROR"; return -1;
        }
    }

    struct Comparator {
        int category;
        bool negative;
        Comparator(int category, bool negative = false){
            this->category = category;
            this->negative = negative;
        }
        bool operator()(Container * c1, Container * c2){
            return compareQuery(c1, c2, category, negative);
        }
    private :
        static bool compareQuery(Container * c1, Container * c2, int catNum, bool negative = false) {
            long e1 = c1->getEffortForCategory(catNum);
            long e2 = c2->getEffortForCategory(catNum);
            if(negative){
                return e1 > e2;
            }
            return e1 < e2;
        }
    };
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

    int catContainersCount[3] = {0};
    catContainersCount[0] = containersCount;//all the containers are default to a

    for(int cat = 1; cat < 3; cat++) {
        for (int i = 0; i < containersCount; i++) {
            Container * c = containers[i];
            if(c->getEffortForCategory(c->bestCandidate) > c->getEffortForCategory(cat)){
                catContainersCount[c->bestCandidate] -- ;
                c->bestCandidate = cat;
                catContainersCount[cat] ++;
                cout<<"";
            }
        }
    }

    if((catContainersCount[0] == 0 && catPresent[0]) || (catContainersCount[1] == 0 && catPresent[1]) || (catContainersCount[2] == 0&& catPresent[2])){
        vector<Container *> categorySortedContainers[3];
        categorySortedContainers[0] = vector<Container*>();
        categorySortedContainers[1] = vector<Container*>();
        categorySortedContainers[2] = vector<Container*>();

        for(auto c : containers){
            categorySortedContainers[0].push_back(c);
            categorySortedContainers[1].push_back(c);
            categorySortedContainers[2].push_back(c);
        }
        for(int i = 0; i < 3; i ++)
            sort(categorySortedContainers[i].begin(), categorySortedContainers[i].end(), Container::Comparator(i));

        Container * usedContainerPreviously = nullptr;

        bool resolveConflict = false;
        for(int  i = 0; i < 3 ; i++){
            if(catContainersCount[i] == 0 && catPresent[i]){
                Container * firstC = categorySortedContainers[i][0];
                if(usedContainerPreviously == firstC){
                    resolveConflict = true;
                }else{
                    //resolve
                    if(catContainersCount[firstC->bestCandidate] == 1) {
                        resolveConflict = true;
                    }else{
                        catContainersCount[firstC->bestCandidate]--;
                        firstC->bestCandidate = i;
                        catContainersCount[firstC->bestCandidate]++;
                    }
                }
                usedContainerPreviously = firstC;
            }
        }
        if(resolveConflict)
            cout<<"REWRITE!";
    }




    long long sum = 0;
    for(int i = 0; i < containersCount; i ++){
        Container * c = containers[i];
        sum += c->getEffortForCategory(c->bestCandidate);
    }
    cout<<sum;
    long long t[3] = {0};
   for(int i = 0; i < 3; i ++){
       Container * c1 = containers[0];
       Container * c2 = containers[1];
       Container * c3 = containers[2];
       int in1 = (i + 1) % 3;
       int in2 = (i + 2) % 3;
       t[i] = c1->getEffortForCategory(i) + c2->getEffortForCategory(in1) + c3->getEffortForCategory(in2);
       cout<<"";
   }
    for(int i = 0; i < 5000; i ++){
       // cout<<i<<" ";
    }
    for(auto c : containers){
        delete c;
    }
    return 0;
}
