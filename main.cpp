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
            long e1 = c1->countDelta(catNum);
            long e2 = c2->countDelta(catNum);
            if(negative){
                return e1 > e2;
            }
            return e1 < e2;
        }
    };
    long countDelta(int cat){
        return this->getEffortForCategory(cat) - this->getEffortForCategory(this->bestCandidate);
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
    vector<Container *> sortedContainers[3];
    for(int i = 0; i < containersCount; i ++){
        Container * c = containers[i];
        sortedContainers[0].push_back(c);
        sortedContainers[1].push_back(c);
        sortedContainers[2].push_back(c);
    }
    cout<<"";
    for(int i = 0; i < 3 ; i++){
        sort(sortedContainers[i].begin(), sortedContainers[i].end(), Container::Comparator(i));
    }
    bool checkAll = false;
    for(int i = 0; i < 3 ; i++){
        if(catContainersCount[i] == 0 && catPresent[i]){
            Container * asksFor = sortedContainers[i][0];
            if(catContainersCount[asksFor->bestCandidate] != 1){
                catContainersCount[asksFor->bestCandidate] --;
                asksFor->bestCandidate = i;
                catContainersCount[asksFor->bestCandidate] ++;
            }
            else{
                checkAll = true;
            }
        }
    }
    if(checkAll){
        int bestScore = 2147483647;
        int bestI;
        int bestJ;
        int bestK;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    Container * c0 = sortedContainers[0][i];
                    Container * c1 = sortedContainers[1][j];
                    Container * c2 = sortedContainers[2][k];
                    if(c0 != c1 && c1 != c2 && c0 != c2){
                        int score = c0->countDelta(0) + c1->countDelta(1) + c2->countDelta(2);
                        if(score < bestScore){
                            bestScore = score;
                            bestI = i;
                            bestJ = j;
                            bestK = k;
                        }
                    }
                }
            }
        }
        Container * c0 = sortedContainers[0][bestI];
        catContainersCount[c0->bestCandidate] -- ;
        c0->bestCandidate = 0;
        catContainersCount[c0->bestCandidate] ++ ;

        Container * c1 = sortedContainers[1][bestJ];
        catContainersCount[c1->bestCandidate] -- ;
        c1->bestCandidate = 1;
        catContainersCount[c1->bestCandidate] ++ ;

        Container * c2 = sortedContainers[2][bestK];
        catContainersCount[c2->bestCandidate] -- ;
        c2->bestCandidate = 2;
        catContainersCount[c2->bestCandidate] ++ ;
    }
    long long sum = 0;
    for(int i = 0; i < containersCount; i ++){
        Container * c = containers[i];
        long tmp = c->getEffortForCategory(c->bestCandidate);
        sum += tmp;
    }
    cout<<sum;
    for(auto c : containers){
        delete c;
    }

    return 0;
}

