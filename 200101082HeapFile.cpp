#include<bits/stdc++.h>

using namespace std;

class Page{

    public:

        Page(){
            pageSize = 0;
            data = nullptr;
            prevPage = nullptr;
            nextPage = nullptr;
        }

        Page(int size){
            pageSize = size;
            data = new int[pageSize];
            prevPage = nullptr;
            nextPage = nullptr;

            for(int i=0; i<pageSize; i++){
                data[i] = -1;
            }

            for(int i=(pageSize-4); i<pageSize; i++){
                data[i] = 0;
            }

            for(int i=(pageSize-8); i<(pageSize-4); i++){
                data[i] = 0;
            }

        }

        Page* getPreviousPage(){
            return prevPage;
        }

        Page* getNextPage(){
            return nextPage;
        }

        bool canInsert(int size){
            int bytesUsed = data[pageSize-8] + (data[pageSize-4] * 4) + 16;
            int bytesLeft = pageSize - bytesUsed;
            if((size + 4) <= bytesLeft) return true;
            else return false;
        }

        void insert(int val, int size){
            int startIdx = data[pageSize-8];
           // cout<< startIdx<< endl;
            for(int i=startIdx; (i - startIdx) < size; i++){
                data[i] = val;
               // cout<< val<<endl;
            }
            for(int i=(pageSize-8); i<(pageSize-4); i++){
                data[i] += size;
            }
            for(int i=(pageSize-4); i<pageSize; i++){
                data[i]++;
            }
            int addressStartIdx = pageSize - 16 - (4 * data[pageSize-4]);
            for(int i=addressStartIdx; (i - addressStartIdx) < 4; i++){
                data[i] = startIdx;
            }
        }

        int search(int val){
            int noOfRecords = data[pageSize-4];
            for(int i=0; i<noOfRecords; i++){
                int startIdx = pageSize - 16 - (4 * (i+1));
               // cout<<data[data[startIdx]] << " ";
                if(data[data[startIdx]] == val) return i;
            }
            return -1;
        }

        int getRecordCount(){
            return data[pageSize-4];
        }

        void setPreviousPage(Page* page){
            prevPage = page;
        }

        void setNextPage(Page* page){
            nextPage = page;
        }

        virtual ~Page(){

        }

    private:

        int pageSize;
        int* data;
        Page* prevPage;
        Page* nextPage;

};

class HeapFile{

    public:

        HeapFile(){
            startPage = nullptr;
            pageCount = 0;
            pageSize = 0;
        }

        HeapFile(int size){
            startPage = nullptr;
            pageCount = 0;
            pageSize = size;
        }

        void insert(int data, int size){
            if(startPage == nullptr){
                Page* firstPage = new Page(pageSize);
                pageCount++;
                startPage = firstPage;
                firstPage->insert(data, size);
            }
            else{
                Page* currPage = startPage;
                while(currPage != nullptr){
                    if(currPage->canInsert(size)){
                        currPage->insert(data, size);
                        return ;
                    }
                    else{
                        if(currPage->getNextPage() == nullptr){
                            Page* newPage = new Page(pageSize);
                            pageCount++;
                            newPage->insert(data, size);
                            currPage->setNextPage(newPage);
                            newPage->setPreviousPage(currPage);
                            return ;
                        }
                    }
                    currPage = currPage->getNextPage();
                }
            }
        }

        void search(int data){
            int pageNo;
            Page* currPage;
            for(pageNo = 0, currPage = startPage; currPage != nullptr; currPage = currPage->getNextPage(), pageNo++){
                int searchResult = currPage->search(data);
                if(searchResult != -1){
                    cout<<pageNo<<" "<<searchResult<<"\n";
                    return ;
                }
            }
            cout<<-1<<" "<<-1<<"\n";
            return ;
        }

        void printStatus(){
            cout<<pageCount<<" ";
            Page* currPage = startPage;
            while(currPage != nullptr){
                cout<<currPage->getRecordCount()<<" ";
                currPage = currPage->getNextPage();
            }
            cout<<"\n";
        }

        virtual ~HeapFile(){

        }

    private:

        Page* startPage;
        int pageCount;
        int pageSize;

};

int main(){
    int pageSize = 0;
    cin>>pageSize;

    HeapFile heapFile(pageSize);

    int option = 4;
    do{
        cin>>option;
        
        switch(option){
            case 1 : {
                int recordSize = 0;
                int primaryKey = 0;
                cin>>recordSize>>primaryKey;
                heapFile.insert(primaryKey, recordSize);
                break;
            }
            case 2 : {
                heapFile.printStatus();
                break;
            }
            case 3 : {
                int primaryKey = 0;
                cin>>primaryKey;
                heapFile.search(primaryKey);
                break;
            }
            default : {
                option = 4;
                break;
            }
        }

    }while(option != 4);

    return 0;
}