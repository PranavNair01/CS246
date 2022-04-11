#include<bits/stdc++.h>

using namespace std;

// class NodeElement{

//     public:

//         NodeElement(){
//             data = 0;
//             leftChild = nullptr;
//             rightChild = nullptr;
//         }

//         NodeElement(int val, Node* lChild, Node* rChild){
//             data = val;
//             leftChild = lChild;
//             rightChild = rChild;
//         }

//         int getData(){
//             return data;
//         }

//         Node* getLeftChild(){
//             return leftChild;
//         }

//         Node* getRightChild(){
//             return rightChild;
//         }

//         void setData(int val){
//             data = val;
//         }

//         void setLeftChild(Node* node){
//             leftChild = node;
//         }

//         void setRightChild(Node* node){
//             rightChild = node;
//         }

//         virtual ~NodeElement(){

//         }

//     private:

//         int data;
//         Node* leftChild;
//         Node* rightChild;

// };

class Node{

    public:

        Node(){
            data = nullptr;
            size = 0;
            pointers = nullptr;
            isDataNode = false;
        }

        Node(int* nodeData, int n, Node** pointersData, bool dataNode){
            data = nodeData;
            size = n;
            pointers = pointersData;
            isDataNode = dataNode;
        }

        int* getData(){
            return data;
        }

        int getSize(){
            return size;
        }

        Node** getPointers(){
            return pointers;
        }

        bool checkDataNode(){
            return isDataNode;
        }

        void setData(int* nodeData){
            data = nodeData;
        }

        void setSize(int val){
            size = val;
        }

        void setPointers(Node** pointersData){
            pointers = pointersData;
        }

        void setDataNode(bool status){
            isDataNode = status;
        }

        void updateData(int val, int idx){
            data[idx] = val;
        }

        void updatePointer(Node* node, int idx){
            pointers[idx] = node;
        }

        virtual ~Node(){

        }

    private:

        // set<NodeElement*>* data;
        int* data;
        int size;
        Node** pointers;
        bool isDataNode;

};

class BPlusTree{

    public:

        BPlusTree(){
            root = nullptr;
            d = 0;
            t = 0;
            dataNodeCount = 0;
            indexNodeCount = 0;
        }

        BPlusTree(Node* rootNode, int dVal, int tVal){
            root = rootNode;
            d = dVal;
            t = tVal;
            dataNodeCount = 0;
            indexNodeCount = 0;
        }

        Node* getRoot(){
            return root;
        }

        void setRoot(Node* node){
            root = node;
        }

        void insertData(int val){
            if(root == nullptr){
                root = new Node();
                root->setDataNode(true);
                int* data = new int[2*d];
                Node** pointers = new Node*[2*d + 1];
                dataNodeCount++;
                root->setData(data);
                root->setPointers(pointers);
                root->updateData(val, 0);
                root->setSize(1);
            }
            else{
                Node* cursor = root;
                Node* parent;

                while(!cursor->checkDataNode()){
                    parent = cursor;
                    for(int i=0; i<cursor->getSize(); i++){
                        if(val < cursor->getData()[i]){
                            cursor = cursor->getPointers()[i];
                            break;
                        }
                        if(i == cursor->getSize()-1){
                            cursor = cursor->getPointers()[i+1];
                        }
                    }
                }
                cout<<"Traverse complete\n";

                if(cursor->getSize() < 2*d){
                    int i=0;
                    while(val > cursor->getData()[i] && i < cursor->getSize()){
                        i++;
                    }
                    for(int j = cursor->getSize(); j > i; j--){
                        cursor->updateData(cursor->getData()[j-1], j);
                    }
                    cursor->updateData(val, i);
                    cursor->setSize(cursor->getSize() + 1);
                    cursor->updatePointer(cursor->getPointers()[cursor->getSize() - 1], cursor->getSize());
                    cursor->updatePointer(nullptr, cursor->getSize() - 1);
                }
                else{
                    int* newLeafData = new int[2*d];
                    Node** newLeafPointers = new Node*[2*d + 1];
                    Node* newLeafNode = new Node(newLeafData, 0, newLeafPointers, true);
                    dataNodeCount++;
                    
                    int virtualNode[2*d + 1];
                    for(int i=0; i < 2*d; i++){
                        virtualNode[i] = cursor->getData()[i];
                    }
                    int i=0, j;

                    while(val > virtualNode[i] && i < 2*d){
                        i++;
                    }

                    for(int j=(2*d + 1); j > i; j--){
                        virtualNode[j] = virtualNode[j-1];
                    }
                    virtualNode[i] = val;

                    cursor->setSize(d);
                    newLeafNode->setSize(d+1);

                    cursor->updatePointer(newLeafNode, cursor->getSize());
                    newLeafNode->updatePointer(cursor->getPointers()[2*d], newLeafNode->getSize());
                    cursor->updatePointer(nullptr, 2*d);

                    for(i=0; i < cursor->getSize(); i++){
                        cursor->updateData(virtualNode[i], i);
                    }

                    for(i=0, j=cursor->getSize(); i < newLeafNode->getSize(); i++, j++){
                        newLeafNode->updateData(virtualNode[j], i);
                    }

                    if(cursor == root){
                        int* newRootData = new int[2*t + 1];
                        Node** newRootPointers = new Node*[2*t + 2];
                        Node* newRootNode = new Node(newRootData, 0, newRootPointers, false);
                        indexNodeCount++;
                        newRootNode->updateData(newLeafNode->getData()[0], 0);
                        newRootNode->updatePointer(cursor, 0);
                        newRootNode->updatePointer(newLeafNode, 1);
                        newRootNode->setSize(1);
                        root = newRootNode;
                    }
                    else{
                        insertInternal(newLeafNode->getData()[0], parent, newLeafNode);
                    }
                }
            }
        }

        void insertInternal(int val, Node* cursor, Node* child){
            if(cursor->getSize() < (2*t + 1)){
                int i=0;
                while(val > cursor->getData()[i] && i < cursor->getSize()){
                    i++;
                }
                for(int j=cursor->getSize(); j > i; j--){
                    cursor->updateData(cursor->getData()[j-1], j);
                }
                for(int j=cursor->getSize()+1; j > i+1; j--){
                    cursor->updatePointer(cursor->getPointers()[j-1], j);
                }
                cursor->updateData(val, i);
                cursor->setSize(cursor->getSize()+1);
                cursor->updatePointer(child, i+1);
            }
            else{
                int* newInternalData = new int[2*t + 1];
                Node** newInternalPointers = new Node*[2*t + 2];
                Node* newInternalNode = new Node(newInternalData, 0, newInternalPointers, false);
                indexNodeCount++;
                int virtualKey[2*t + 2];
                Node* virtualPtr[2*t + 3];

                for(int i=0; i < 2*t + 1; i++){
                    virtualKey[i] = cursor->getData()[i];
                }
                for(int i=0; i < 2*t + 2; i++){
                    virtualPtr[i] = cursor->getPointers()[i];
                }

                int i=0, j;

                while(val > virtualKey[i] && i < 2*t + 1){
                    i++;
                }

                for(int j = 2*t + 2; j > i; j--){
                    virtualKey[j] = virtualKey[j-1];
                }
                virtualKey[i] = val;

                for(int j = 2*t + 3; j > i+1; j--){
                    virtualPtr[j] = virtualPtr[j-1];
                }
                virtualPtr[i+1] = child;

                cursor->setSize(t);
                newInternalNode->setSize(t+1);

                for(i=0, j=cursor->getSize()+1; i < newInternalNode->getSize(); i++, j++){
                    newInternalNode->updateData(virtualKey[j], i);
                }
                for(i=0, j=cursor->getSize()+1; i < newInternalNode->getSize(); i++, j++){
                    newInternalNode->updatePointer(virtualPtr[j], i);
                }

                if(cursor == root){
                    int* newRootData = new int[2*t + 1];
                    Node** newRootPointers = new Node*[2*t + 2];
                    Node* newRootNode = new Node(newRootData, 0, newRootPointers, false);
                    indexNodeCount++;
                    newRootNode->updateData(newInternalNode->getData()[0], 0);
                    newRootNode->updatePointer(cursor, 0);
                    newRootNode->updatePointer(newInternalNode, 1);
                    newRootNode->setSize(1);
                    root = newRootNode;
                }
                else{
                    insertInternal(newInternalNode->getData()[0], findParent(root, cursor), newInternalNode);
                }
            }
        }

        Node* findParent(Node* cursor, Node* child){
            Node* parent;

            if(cursor->checkDataNode() || (cursor->getPointers()[0]->checkDataNode())){
                return nullptr;
            }

            for(int i=0; i < cursor->getSize() + 1; i++){
                if(cursor->getPointers()[i] == child){
                    parent = cursor;
                    return parent;
                }
                else{
                    parent = findParent(cursor->getPointers()[i], child);
                    if(parent != nullptr){
                        return parent;
                    }
                }
            }

            return parent;
        }

        void showStatus(){
            cout<<indexNodeCount<<" "<<dataNodeCount<<" ";
            for(int i=0; i<root->getSize(); i++){
                cout<<root->getData()[i]<<" ";
            }
            cout<<"\n";
        }

        virtual ~BPlusTree(){

        }

    private:

        Node* root;
        int d;
        int t;
        int dataNodeCount;
        int indexNodeCount;

};

int main(){
    int d=0;
    int t=0;
    cin>>d>>t;

    BPlusTree tree(nullptr, d, t);

    int option = 3;
    do{
        cin>>option;

        switch(option){
            case 1 : {
                int val = 0;
                cin>>val;
                tree.insertData(val);
                break;
            }
            case 2 : {
                tree.showStatus();
            }
        }

    }while(option != 3);

    return 0;
}