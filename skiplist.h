#ifndef SKIPLIST_H_INCLUDED
#define SKIPLIST_H_INCLUDED



#endif // SKIPLIST_H_INCLUDED
#include<iostream>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<mutex>
#include<fstream>

#define STORE_FILE "store/dumpFile"

std::mutex mtx;
std::string delimiter = ":";

//定义存储结点基本结构
template<typename K, typename V>
class Node
{
public:

    Node(){}

    Node(K k, V v, int level);

    ~Node();

    K get_key();

    V get_value();

    void set_value(V);

    //用于指出不同层次的下一结点
    Node<K,V> **nextarray;

    int node_level;

private:

    K key;

    V value;
};


template<typename K, typename V>
Node<K,V>::Node(const K k,const V v, int level)
{
    this.key = k;
    this.value = v;
    this.node_level = level;

    //为该节点申请不同索引层次的空间
    this.nextarray = new Node<K, V> *[level + 1];
    //初始化
    memset(this.nextarray, 0, sizeof(Node<K,V>*) * (level + 1));
}

template<typename K, typename V>
Node<K,V>::~Node()
{
    delete []nextarray;
}

template<typename K, typename V>
K Node<K,V>::get_key()
{
    return this.key;
}

template<typename K, typename V>
V Node<K,V>::get_value()
{
    return this.value;
}

template<typename K, typename V>
void Node<K,V>::set_value(V v)
{
    this.value = v;
}

template<typename K, typename V>
class SkipList
{
public:
    SkipList(int);

    ~SkipList();

    Node<K,V>* create_node(K, V, int);

    int get_random_level();

    int insert_element(K,V);

    void display_list();

    bool search_element(K);

    void delete_element(K);

    void dump_file();

    void load_file();

    int size();

private:

    void get_key_value_from_string(const std::string &str, std::string * key, std::string* value);
    bool is_valid_string(const std::string & str);
    //跳表索引最多有多少层
    int _max_level;
    //当前跳表索引层（当前跳表中所有节点的最高索引层）
    int _skip_list_level;

    Node<K,V> *_header;

    std::ofstream _file_writer;
    std::ifstream _file_reader;

    //跳表总元素个数
    int _element_count;
};

template<typename K, typename V>
bool SkipList<K,V>::search_element(K key)
{

    std::cout<<"search element-------------"<<std::endl;
    Node<K,V> *current = _header;
    for(int i = _skip_list_level; i>=0; i--)
    {
        while(current->nextarray[i]!= nullptr && current->nextarray[i]->get_key() < key)
            current = current->nextarray[i];
    }
    current = current->nextarray[0];
    if(current!= nullptr && current->get_key == key)
    {

        std::cout<<"found key:"<<key<<", value:"<<current->get_value()<<std::endl;
        return true;
    }
    std::cout<<"not found key:"<<key<<std::endl;
    return false;
}

template<typename K, typename V>
int SkipList<K,V>::get_random_level()
{
    int k =1;
    while(rand() %2)
        k++;
    k = (k< _max_level)?k:_max_level;
    return k;
}

template<typename K, typename V>
SkipList<K,V>::SkipList(int n)
{
    this->_element_count = n;
}

template<typename K, typename V>
SkipList<K,V>::~SkipList()
{
    if(_file_writer.is_open())
        _file_writer.close();
    if(_file_reader.is_open())
        _file_reader.close();
    delete _header;
}

template<typename K, typename V>
Node<K,V>* SkipList<K,V>::create_node(K k, V v, int n)
{
    Node<K,V> *node = new Node<K,V>(k,v,n);
    return node;
}

template<typename K , typename V>
void SkipList<K,V>::display_list()
{
    std::cout<<"\n*********skip list***********\n"<<std::endl;
    for(int i=0; i<= _skip_list_level; i++)
    {
        Node<K,V> *node = this->_header->nextarray[i];
        std::cout<<"level"<<i<<":"<<std::endl;
        while(node!=nullptr)
        {
            std::cout<<node->get_key()<<":"<<node->get_value()<<";";
            node = node->nextarray[i];
        }
        std::cout<<std::endl;
    }
}

template<typename K, typename V>
void SkipList<K,V>::dump_file()
{

    std::cout<<"dump file-------------"<<std::endl;
    _file_writer.open(STORE_FILE);
    Node<K,V> *node = this->_header->nextarray[0];
    while(node != nullptr)
    {
        _file_writer<<node->get_key<<":"<<node->get_value<<"\n";
        std::cout<<node->get_key<<":"<<node->get_value<<"\n";
        node = node->nextarray[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return;
}

template<typename K, typename V>
void SkipList<K,V>::load_file()
{
    _file_reader.open(STORE_FILE);
    std::cout<<"load file ------"<<std::endl;
    std::string line;
    std::string * key = new std::string();
    std::string * value = new std::string();
    while(getline(_file_reader, line))
    {
        get_key_value_from_string(line, key, value);
        if(key->empty()||value->empty())
            continue;
        insert_element(*key, *value);
        std::cout<<"key:"<<*key<<"value:"<<*value<<std::endl;
    }
    _file_reader.close();
}

template<typename K, typename V>
int SkipList<K,V>::size()
{
    return _element_count;
}

template<typename K, typename V>
void SkipList<K,V>::get_key_value_from_string(const std::string &str, std::string *key, std::string *value)
{

    if(!is_valid_string(str))
        return;
    *key = str.substr(0,str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1, str.length());
    //问题？对于对象结构的value如何重建对象
}

template<typename K, typename V>
bool SkipList<K,V>::is_valid_string(const std::string &str)
{

    if(str.empty())
        return false;
    if(str.find(delimiter) == std::string::npos)
        return false;
    return true;
}

template<typename K, typename V>
void SkipList<K,V>::delete_element(K key)
{

    mtx.lock();
    Node<K,V> *current = this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update,0,sizeof(Node<K,V>*) * (_max_level+1));

    for(int i= _skip_list_level; i>=0; i--)
    {

        while(current->nextarray[i] != nullptr && current->nextarray[i]->get_key < key)
            current = current->nextarray[i];
        update[i] = current;
    }

    current = current->nextarray[0];
    if(current != nullptr && current->get_key == key)
    {
        for(int i =0; i<= _skip_list_level ;i++)
        {
            if(update[i]->nextarray[i]  != current)
                break;
            update[i]->nextarray[i] = current->nextarray[i];
        }
		
		//删除节点时，需要注意是否会改变当前list的索引层数
        while(_skip_list_level>0 &&_header->nextarray[_skip_list_level] == 0)
        {
            _skip_list_level --;
        }
        std::cout <<"Successfully deleted key"<<key<<std::endl;
        _element_count --;
    }
    else if(current->get_key != key)
    {

        std::cout<<"no the element with key is"<<key<<std::endl;
    }

    mtx.unlock();
    return;


}

template<typename K, typename V>
int SkipList<K,V>::insert_element(const K key, const V value)
{
    mtx.lock();
    Node<K,V> *current = this->_header;
    Node<K,V> *updata[_max_level+1];
    memset(updata,0,sizeof(Node<K,V>*) * (_max_level+1));

    for(int i= _max_level; i>=0; i--)
    {
        while(current->nextarray[i] != nullptr && current->nextarray[i]->get_key() < key)
            current = current->nextarray[i];
        updata[i] = current;
    }

    current = current->nextarray[0];
    if(current != nullptr && current->get_key() == key)
    {
        std::cout<<"key:"<<key<<",exists"<<std::endl;
        mtx.unlock();
        return 1;
    }

    if((current == nullptr)|| current->get_key != key)
    {
        int random_level = get_random_level();
		
		/*
		updata中找到的是max层对应的节点
		但对于当前最高层---->max层那些updata[]都是nullptr
		如果新插入节点的索引层数为k		当前最高层<k<max层
		则需要更新updata[当前最高层:k]
		然后更新当前最高层
		*/
        if(random_level > _skip_list_level)
        {
            for(int i=_skip_list_level+1; i<random_level+1; i++)
                updata[i] = _header;
            _skip_list_level =random_level;
        }

        Node<K,V> * insert_node = create_node(key, value, random_level);
        for(int i =0; i<random_level; i++)
        {
            insert_node->nextarray[i] = updata[i]->nextarray[i];
            updata[i]->nextarray[i] = insert_node;
        }

        std::cout<< "Successfully inserted key:"<<key<<", value"<<value<<std::endl;
        ++_element_count;

    }
    mtx.unlock();
    return 0;
}
