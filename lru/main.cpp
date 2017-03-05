#include <iostream>
#include <vector>
#include <map>
#include <List>
#include <assert.h>
using namespace std;

typedef struct Node{
    Node(int key, int data){
        this->key = key;
        this->data = data;
        //print();
    }
    Node& operator = (const Node& node){
        if (this  == &node) return  *this;

        this->key = node.key;
        this->data = node.data;
        //print();
        return *this;
    }
    void print(){
        cout<<"\tkey:"<<key<<"\tvalue:"<<data<<endl;
    }
    int key;
    int data;
}Node;

class LRU{

public:
    LRU(size_t _size);
    ~LRU();

public:
    const Node* Get(int key);
    int Put(int key, Node node);

    void Print();

private:
    int Attach(Node* node);
    Node* Detach();

private:
    map<int/*key*/,Node*> lru_map;
    list<Node*> idle;  //���е�
    list<Node*> lru_list; //�Ѿ�ʹ�õ��ڴ�
    size_t size;
};

LRU::LRU(size_t _size)
{
    cout<<__func__<<endl;
    Node * temp = NULL;
    this->size = _size;
    for(size_t i=0; i<this->size;i++){
        temp = new Node(0,0);
        this->idle.push_back(temp);
    }
}


LRU::~LRU()
{
    cout<<__func__<<endl;
    for(list<Node*>::iterator it= idle.begin(); it != idle.end(); it++){
        delete *it;
    }

    for(list<Node*>::iterator it= lru_list.begin(); it != lru_list.end(); it++){
        delete *it;
    }
}

int LRU::Attach(Node* node)
{
    cout<<__func__<<"\tkey:"<<node->key<<endl;
    this->lru_list.push_front(node); //������ʵ��´ο����õ�
    return 0;
}


Node* LRU::Detach()
{
    Node* temp = NULL;
    temp = this->lru_list.back(); //����һ��,�ʱ��û������
    this->lru_list.pop_back();
    cout<<__func__<<"\tkey:"<<temp->key<<endl;
    return temp;
}


int LRU::Put(int key, Node node)
{
    cout<<__func__<<endl;
    node.print();
    Node* temp = NULL;
    temp = this->lru_map[key];
    if (NULL == temp){ //����û��
        if (this->idle.empty()) //���пռ�û����
        { //��̭�ϵ�
          temp = this->Detach();
          this->lru_map.erase(temp->key);

        }else{
            temp = this->idle.front(); //����
            this->idle.pop_front();
        }
        *temp = node;
        this->lru_map[key] = temp;
        this->Attach(temp);
    }else{
        cout<<"����,ֱ���޸�!\n";
        *temp = node; //�޸�ֵ,������û���Ƶ� lru_list ͷ

        //����˳��
        for(list<Node*>::iterator it = this->lru_list.begin(); it!=this->lru_list.end(); it++)
        {
            if (temp->key == (*it)->key){
                this->lru_list.erase(it);
                break; //����break ��,���õ��ĵ���ʧЧ
            }
        }
        this->lru_list.push_front(temp); //������ʵ��´ο����õ�

    }
    return 0;
}

const Node* LRU::Get(int key)
{

    cout<<__func__<<endl;
    Node* temp = NULL;
    temp = this->lru_map[key];

        //����˳��
    for(list<Node*>::iterator it = this->lru_list.begin(); it!=this->lru_list.end();it++)
    {
        if (temp->key == (*it)->key){
            this->lru_list.erase(it);
            break; //����break ��,���õ��ĵ���ʧЧ
        }
    }
    this->lru_list.push_front(temp); //������ʵ��´ο����õ�
    return temp;
}

void LRU::Print()
{
    cout<<__func__<<endl;
    for(map<int, Node*>::iterator it = this->lru_map.begin(); it!=this->lru_map.end();it++)
    {
        cout<<"lru_map:";
        it->second->print();
    }

    for(list<Node*>::iterator it = this->lru_list.begin(); it!=this->lru_list.end();it++)
    {
        cout<<"lru_list:";
        (*it)->print();
    }

    cout<<"space size:"<<this->size<<"\tlru_map size:"<<this->lru_map.size()<<"\tlru_list size:"<<this->lru_list.size()<< \
    "\tidle size:" <<this->idle.size()<<endl;

    assert(this->lru_map.size() == this->lru_list.size());
    assert( (this->idle.size() + this->lru_list.size() ) ==  this->size);
}

int main()
{
    LRU lru(3);
    Node node(1,1);
    lru.Put(1, node);


    Node node2(2,2);
    lru.Put(2, node2);

    Node node3(3,3);
    lru.Put(3, node3);

    lru.Get(2);


    lru.Put(2, node2);

    lru.Put(1, node);


    Node node4(4,4);
    lru.Put(4, node4);


    lru.Get(2);

    lru.Print();
    //cout << "Hello world!!" << endl;
    return 0;
}
