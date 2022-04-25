#include<bits/stdc++.h>

using namespace std;

int main(){
    int bufferCount = 0;
    int recordsPerPage = 0;
    int recordCount = 0;

    cin>>bufferCount>>recordsPerPage>>recordCount;

    vector<int> data(recordCount);

    for(int i=0; i<recordCount; i++){
        cin>>data[i];
    }

    sort(data.begin(), data.end());

    for(int i=0; i<recordCount; i++){
        cout<<data[i]<<"\n";
    }

    return 0;
}