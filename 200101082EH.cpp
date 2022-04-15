#include<bits/stdc++.h>

using namespace std;

class Bucket{

    public:

        Bucket(){
            capacity = 0;
            localDepth = 0;
        }

        Bucket(int bucketCapacity, int depth){
            capacity = bucketCapacity;
            localDepth = depth;
        }

        int getLocalDepth(){
            return localDepth;
        }

        int getCapacity(){
            return capacity;
        }

        vector<int> getData(){
            return data;
        }

        void setLocalDepth(int depth){
            localDepth = depth;
        }

        void setCapacity(int val){
            capacity = val;
        }

        void setData(vector<int> newData){
            data = newData;
        }

        void insertData(int val){
            data.push_back(val);
        }

    private:
        int capacity;
        int localDepth;
        vector<int> data;

};

class ExtendibleHash{

    public:

        ExtendibleHash(){
            globalDepth = 0;
            maxGlobalDepth = 0;
            bucketCapacity = 0;
            overflowThreshold = 0;
            bucketCount = 0;
        }

        ExtendibleHash(int depth, int maxDepth, int capacity, int m){
            globalDepth = depth;
            maxGlobalDepth = maxDepth;
            bucketCapacity = capacity;
            overflowThreshold = m;
            bucketCount = 0;
            for(int i=0; i < (1 << globalDepth); i++){
                Bucket* newBucket = new Bucket(bucketCapacity, globalDepth);
                directory.push_back(make_pair(newBucket, nullptr));
                bucketCount++;
                isOverflow.push_back(false);
            }
        }

        int hashValue(int i){
            return i;
        }

        bool checkDoubling(){
            int p = 0;
            int q = bucketCount;
            for(int i=0; i < (1 << globalDepth); i++){
                if(isOverflow[i]) p++;
            }
            if((float(p * 100.0)/float(q)) < float(overflowThreshold)) return false;
            else return true;
        }

        void doubleDirectory(){
            if(globalDepth == 22){
                cout<<"Extendible hash has hit maximum global depth of 22\n";
                return ;
            }
            for(int i=0; i < (1 << globalDepth); i++){
                directory.push_back(directory[i]);
                isOverflow.push_back(isOverflow[i]);
            }
            globalDepth++;
            for(int i=0; i < (1 << (globalDepth - 1)); i++){
                if(isOverflow[i]){
                    vector<int> allElements;
                    for(auto element : directory[i].first->getData()) allElements.push_back(element);
                    for(auto element : directory[i].second->getData()) allElements.push_back(element);
                    vector<int> temp;
                    directory[i].first->setData(temp);
                    directory[i].second = nullptr;
                    isOverflow[i] = false;
                    isOverflow[i + (1 << (globalDepth - 1))] = false;
                    for(auto element : allElements) insertData(element);
                }
            }
        }

        void insertData(int data){
            pair<Bucket*, Bucket*> currBuckets = directory[hashValue(data) % (1 << globalDepth)];
            if(currBuckets.first->getData().size() < bucketCapacity){
                currBuckets.first->insertData(data);
                return ;
            }
            else if((currBuckets.first->getData().size() == bucketCapacity) && (currBuckets.first->getLocalDepth() < globalDepth)){
                Bucket* newBucket = new Bucket(bucketCapacity, currBuckets.first->getLocalDepth() + 1);
                directory[hashValue(data) % (1 << globalDepth)] = make_pair(newBucket, nullptr);
                isOverflow[hashValue(data) % (1 << globalDepth)] = false;
                bucketCount++;
                currBuckets.first->setLocalDepth(currBuckets.first->getLocalDepth() + 1);
                vector<int> updatedOldBucket;
                for(auto element : currBuckets.first->getData()){
                    if(hashValue(element % (1 << globalDepth)) != hashValue(data % (1 << globalDepth))){
                        updatedOldBucket.push_back(element);
                    }
                    else{
                        newBucket->insertData(element);
                    }
                }
                newBucket->insertData(data);
                currBuckets.first->setData(updatedOldBucket);
                return ;
            }
            else if((currBuckets.first->getData().size() == bucketCapacity) && (currBuckets.first->getLocalDepth() == globalDepth)){
                Bucket* overflowBucket = new Bucket(bucketCapacity, globalDepth);
                currBuckets.second = overflowBucket;
                directory[hashValue(data) % (1 << globalDepth)].second = overflowBucket;
                isOverflow[hashValue(data % (1 << globalDepth))] = true;
                overflowBucket->insertData(data);
                if(checkDoubling()){
                    doubleDirectory();
                    return ;
                }
                else{
                    return ;
                }
            }
        }

        void showStatus(){
            cout<<globalDepth<<" "<<bucketCount<<"\n";
            return ;
        }

    private:
        int globalDepth;
        int maxGlobalDepth;
        int bucketCapacity;
        int overflowThreshold;
        int bucketCount;
        vector<pair<Bucket*, Bucket*>> directory;
        vector<bool> isOverflow;
};

int main(){
    int globalDepth = 0;
    int bucketCapacity = 0;
    int m = 0;

    cin>>globalDepth>>bucketCapacity>>m;

    ExtendibleHash hash(globalDepth, 22, bucketCapacity, m);

    int option = 6;

    do{
        cin>>option;

        switch(option){
            case 2 : {
                int val = 0;
                cin>>val;
                hash.insertData(val);
                break;
            }
            case 5 : {
                hash.showStatus();
                break;
            }
            default: break;
        }
    }while(option != 6);

    return 0;

}