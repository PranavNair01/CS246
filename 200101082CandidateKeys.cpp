#include<bits/stdc++.h>

using namespace std;

int binpow(int a, int b){
    if(b==0){
        return 1;
    }
    int res = binpow(a, (b/2));
    if(b%2){
        return res*res*a;
    }
    else{
        return res*res;
    }
}

map<int, set<int>> keyToSet;
map<set<int>, int> setToKey;
set<pair<int, int>> functionalDependencies;
vector<bool> marked;
vector<int> result;

bool compareSet(int a, int b){
    if(keyToSet[a].size() != keyToSet[b].size()) return (keyToSet[a].size() < keyToSet[b].size());
    else return (keyToSet[a] < keyToSet[b]);
}

bool isSubset(int a, int b){
    if((a & b) == a) return true;
    else return false;
}

int main(){
    int noOfAttributes = 0;
    cin>>noOfAttributes;

    vector<int> attributes(noOfAttributes);
    for(int i=1; i<=noOfAttributes; i++){
        attributes[i-1] = i;
    }

    int noOfSubsets = binpow(2, noOfAttributes);

    for(int i=0; i<noOfSubsets; i++){
        set<int> currSubset;
        for(int j=0; j<noOfAttributes; j++){
            int binaryPosition = 1 << j;
            if((i & binaryPosition) != 0){
                currSubset.insert(attributes[j]);
            }
        }
        keyToSet.insert({i, currSubset});
        setToKey.insert({currSubset, i});
        marked.push_back(false);
    }

    int noOfFunctionalDependencies = 0;
    cin>>noOfFunctionalDependencies;
    
    string dummy;
    getline(cin, dummy);

    for(int i=0; i<noOfFunctionalDependencies; i++){
        set<int> leftAttributes;
        set<int> rightAttributes;
        int tempNumber = 0;
        string tempLine1 = "";
        getline(cin, tempLine1);
        stringstream tempStream1(tempLine1);
        string tempWord1;
        while(!tempStream1.eof()){
            tempStream1 >> tempWord1;
            if(stringstream(tempWord1) >> tempNumber){
                leftAttributes.insert(tempNumber);
            }
        }
        string tempLine2 = "";
        getline(cin, tempLine2);
        stringstream tempStream2(tempLine2);
        string tempWord2;
        while(!tempStream2.eof()){
            tempStream2 >> tempWord2;
            if(stringstream(tempWord2) >> tempNumber){
                rightAttributes.insert(tempNumber);
            }
        }
        functionalDependencies.insert(make_pair(setToKey[leftAttributes], setToKey[rightAttributes]));
        // functionalDependencies[setToKey[leftAttributes]].insert(setToKey[rightAttributes]);
    }

    for(int i=0; i<noOfSubsets; i++){
        if(!marked[i]){
            marked[i] = true;
            set<int> currSubset = keyToSet[i];
            int prevStatus = -1;
            int currStatus = i;
            while(prevStatus != currStatus){
                prevStatus = currStatus;
                for(auto it = functionalDependencies.begin(); it != functionalDependencies.end(); it++){
                    if(isSubset(it->first, currStatus)){
                        currStatus = (currStatus | it->second);
                    }
                }
            }
            if(currStatus == (noOfSubsets - 1)){
                result.push_back(i);
                for(int j=i+1; j<noOfSubsets; j++){
                    if(isSubset(i, j)){
                        marked[j] = true;
                    }
                }
            }
        }
    }

    sort(result.begin(), result.end(), compareSet);
    cout<<result.size()<<"\n";
    for(auto it = result.begin(); it != result.end(); it++){
        set<int> currCandidateKey = keyToSet[*it];
        for(auto candidateKeyIt = currCandidateKey.begin(); candidateKeyIt != currCandidateKey.end(); candidateKeyIt++){
            cout<<*candidateKeyIt<<" ";
        }
        cout<<"\n";
    }

    return 0;

}



// 1nf - all atomic values
// 2nf - no functional dependency from subset of  candidate key to non prime attribute
// attribute that is not present in any candidate key is non-prime
// 3nf - no transitive dependency, that is no candidate key indirectly determines non-prime attribute
// bcnf - all functional dependencies of form x -> y , where x is super key 