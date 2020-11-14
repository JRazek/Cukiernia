#include <iostream>
#include <vector>
#include <algorithm>

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

    struct Comparator {
        int category;
        Comparator(int category){
            this->category = category;
        }
        bool operator()(Container * c1, Container * c2){
            return compareQuery(c1, c2, category);
        }
    private :
        static bool compareQuery(Container * c1, Container * c2, int catNum) {
            int e1 = c1->getEffortForCategory(catNum);
            int e2 = c2->getEffortForCategory(catNum);
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

    for(int cat = 1; cat <= 2; cat++) {
        for (int i = 0; i < containersCount; i++) {
            Container * c = containers[i];
            if(c->getEffortForCategory(c->bestCandidate) > c->getEffortForCategory(cat)){
                catContainersCount[c->bestCandidate] -- ;
                c->bestCandidate = cat;
                catContainersCount[cat] ++;
            }
        }
    }

    if(catContainersCount[0] == 0 || catContainersCount[1] == 0 || catContainersCount[2] == 0){
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

        if(catContainersCount[0] == 0 || catContainersCount[1] == 0 || catContainersCount[2] == 0){
            Container * bestContainerForCat[3] = {nullptr};
            for(int i = 0; i < 3 ; i ++){
                int c = catContainersCount[i];
                if(c == 0 && catPresent[i]){
                    bestContainerForCat[i] = categorySortedContainers[i][0];
                }
            }
            {
                Container * lastUsed = nullptr;
                int lastUsedCat = -1;
                for (int i = 0; i < 3; i++) {
                    int c = catContainersCount[i];
                    if (c == 0) {
                        Container * used = bestContainerForCat[i];
                        //yeah this code is shit. sry
                        if (used != nullptr && used == lastUsed) {
                            //conflict between two.
                            //there must have been two already.
                            //conflict!
                            Container * catACon0 = categorySortedContainers[lastUsedCat][0];
                            Container * catACon1 = categorySortedContainers[lastUsedCat][1];

                            Container * catBCon0 = categorySortedContainers[i][0];
                            Container * catBCon1 = categorySortedContainers[i][1];

                            //cat A num - lastUsedCat
                            //cat B num - i
                            //find the best combination

                            int s1 = catACon0->getEffortForCategory(lastUsedCat) + catBCon1->getEffortForCategory(i);
                            int s2 = catACon1->getEffortForCategory(lastUsedCat) + catBCon0->getEffortForCategory(i);
                            if(s1 < s2){
                                //first in A second in B
                                categorySortedContainers[lastUsedCat][0]->bestCandidate = lastUsedCat;
                                categorySortedContainers[i][1]->bestCandidate = i;
                            }else{
                                categorySortedContainers[lastUsedCat][1]->bestCandidate = lastUsedCat;
                                categorySortedContainers[i][0]->bestCandidate = i;
                            }
                            
                        }
                        lastUsed = used;
                        lastUsedCat = i;
                    }
                }
            }
            for(int i = 0; i < 3; i ++){
                int c = catContainersCount[i];
                if(c == 0){
                    Container * used = bestContainerForCat[i];
                    if(used != nullptr)
                        used->bestCandidate = i;
                }
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
