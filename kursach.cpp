///////////////////////////////////KURSACH////////////////////////////////////////

#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////

class Node
{
public:
    char key[100];
    long long number;
    string oper;
    Node **next;
    Node(char*, long long, int);
    string setOperator(long long);
    Node()
    {
    }
};

Node::Node(char *key, long long number, int level)
{
    strcpy(this->key, key);
    this->number = number;
    this->oper = this->setOperator(number);
    next = new Node*[level + 1];
    memset(next, 0, sizeof(Node*)*(level + 1));
    
};

string Node::setOperator(long long num)
{
    long long n = (num / 10000000) - 38000;
    string oper;
    if (n == 50 || n == 95 || n == 66 || n == 99) oper = "Vodaphone";
    else if (n == 63 || n == 93 || n == 73) oper = "Lifecell";
    else if (n == 67 || n == 68 || n == 96 || n == 97 || n == 98) oper = "Kyivstar";
    else oper = "Undefined";
    return oper;
};

class SkipList
{
    const int MAXLVL;
    float P;
    int level;
    Node *header;
public:
    SkipList(int, float);
    int randomLevel();
    Node* createNode(char* key, long long number, int level);
    void insertElement(char* key, long long number);
    void displayList();
    long long search(char* key);
    void freeList(char* key);
    void deleteList();
    void createDictionary();
    void EditElement(char* key, long long number);
};

bool checkFile(string file_name)
{
    bool flag = false;
    ifstream file;
    file.open(file_name);
    if (!file) flag = true;
    return flag;
}

void SkipList::createDictionary()
{
    ofstream file("dictionary.txt", ios_base::app);
    Node *node = header->next[0];
    file << node->key << " " << node->number << " " << node->oper << endl;
    while (node->next[0] != NULL)
    {
        node = node->next[0];
        file << node->key << " " << node->number << " " << node->oper << endl;
    }
    file.close();
}

bool fileIsEmpty()
{
    ifstream file("dictionary.txt", ios_base::in);
    bool flag = file.is_open();
    if (flag)
    {
        file.seekg(0, ios_base::end);
        flag = file.tellg() > 0;
    }
    return flag;
}

void readDictionary()
{
    ifstream file;
    file.open("dictionary.txt");
    char buff[100];
    while (!file.getline(buff, sizeof(buff)).eof())
        cout << buff << endl;
}

SkipList::SkipList(int MAXLV, float P) :MAXLVL(MAXLV)
{
    this->P = P;
    level = 0;
    header = new Node("a", -1, MAXLVL);
};

int SkipList::randomLevel()
{
    float r = (float)rand() / RAND_MAX;
    int lvl = 0;
    while (r < P && lvl < MAXLVL)
    {
        lvl++;
        r = (float)rand() / RAND_MAX;
    }
    return lvl;
};

Node* SkipList::createNode(char* key, long long number, int level)
{
    Node *n = new Node(key, number, level);
    return n;
};

long long SkipList::search(char* key)
{
    Node *current = header;
    for (int i = level; i >= 0; i--)
    {
        while (current->next[i] != NULL && strcmp(current->key, key) != 0)
        {
            
            if (strcmp(current->next[i]->key, key) <= 0)
            {
                current = current->next[i];
            }
            else { break; }
            
        }
    }
    return current->number;
};

void SkipList::freeList(char* key)
{
    Node *current = header;
    int na = 0;
    Node * n = header;
    Node **update = new Node*[MAXLVL + 1];
    memset(update, 0, sizeof(Node*)*(MAXLVL + 1));
    for (int i = level; i >= 0; i--)
    {
        while (current->next[i] != NULL &&strcmp(current->key, key) != 0)
        {
            if (strcmp(current->next[i]->key, key) <= 0)
            {
                n = current;
                current = current->next[i];
                na = i;
                
            }
            else { break; }
            
        }
    }
    
    for (int i = na; i >= 0; i--)
    {
        while (n->next[i] != NULL && strcmp(n->next[i]->key, key) != 0)
        {
            n = n->next[i];
        }
        update[i] = current->next[i];
        n->next[i] = update[i];
    }
    delete current;
};

void SkipList::insertElement(char* key, long long number)
{
    Node *current = header;
    Node **update = new Node*[MAXLVL + 1];
    memset(update, 0, sizeof(Node*)*(MAXLVL + 1));
    for (int i = level; i >= 0; i--)
    {
        while (current->next[i] != NULL && strcmp(current->next[i]->key, key)<0)
            current = current->next[i];
        update[i] = current;
    }
    current = current->next[0];
    if (current == NULL || strcmp(current->key, key) != 0)
    {
        
        int rlevel = randomLevel();
        if (rlevel > level)
        {
            for (int i = level + 1;i<rlevel + 1;i++)
                update[i] = header;
            level = rlevel;
        }
        
        
        Node* n = createNode(key, number, rlevel);
        
        for (int i = 0;i <= rlevel;i++)
        {
            n->next[i] = update[i]->next[i];
            update[i]->next[i] = n;
        }
        cout << "Successfully Inserted key " << key << "\n";
    }
};

void SkipList::EditElement(char* key, long long num)
{
    Node *current = header;
    for (int i = level; i >= 0; i--)
    {
        while (current->next[i] != NULL && strcmp(current->key, key) != 0)
        {
            if (strcmp(current->next[i]->key, key) <= 0)
            {
                current = current->next[i];
            }
            else break;
        }
    }
    current->number = num;
    cout << "The element was successfully changed " << endl;
}

void SkipList::displayList()
{
    cout << "\n*****Skip List***" << "\n";
    for (int i = level; i >= 0; i--)
    {
        Node *node = header->next[i];
        cout << "Level " << i << ": " << endl;
        while (node != NULL)
        {
            cout << node->key << " " << node->number << " " << node->oper << endl;
            node = node->next[i];
        }
        cout << "\n";
    }
};

int counter()
{
    int count = 0;
    ifstream file("dictionary.txt");
    vector<string> v;
    while (!file.eof())
    {
        string s;
        getline(file, s, ' ');
        v.push_back(s);
    }
    for (unsigned int i = 0; i < v.size(); i++)
    {
        if (v[i] == "Session") count++;
    }
    return count + 1;
}

void SkipList::deleteList()
{
    Node *node;
    while (!header)
    {
        node = header;
        header = node->next[0];
        delete node;
    }
    *header = Node("", -1, MAXLVL);
}

void createSession(SkipList &list)
{
    ofstream file("dictionary.txt", ios_base::app);
    int i = counter();
    file << " Session №" << i << " "<< endl;
    list.createDictionary();
    file << endl;
    file.close();
}

int main()
{
    //setlocale(0, "rus");
    srand((unsigned)time(0));
    SkipList lst(4, 0.5);
    char *a;
    long long nomer;
    if (!fileIsEmpty())
    {
        int n;
        cout << "Введите количество контактов: ";
        cin >> n;
        for (int i = 0; i < n; i++)
        {
            a = new char[100];
            cout << "Имя: ";
            fflush(stdin);
            cin >> a;
            cout << "Номер: ";
            fflush(stdin);
            cin >> nomer;
            lst.insertElement(a, nomer);
            delete[]a;
            nomer = 0;
            cout << endl;
        }
        cin.get();
    }
    //system("clear");
    char b;
    while (b != 'e')
    {
        cout << "f-Find the number\n"
        << "d-Delete the contact\n"
        << "e-Exit\n"
        <<"a-Add the number\n"
        <<"c-Contacts in this session\n"
        <<"s-Save this session\n"
        <<"x-Edit contact\n"
        <<"l-Show the whole catalog" << endl;
        b = cin.get();
        if (b == 'f')
        {
            while (1)
            {
                Node node;
                long long sch;
                a = new char[100];
                cout << "Name: ";
                fflush(stdin);
                cin >> a;
                sch = lst.search(a);
                if (sch > 0)
                    cout << "Number: " << sch << endl;
                else
                    cout << "There is no such a number " << endl;
                delete[]a;
                char f;
                cout << "Do you want to show Operator?(y/n): ";
                fflush(stdin);
                cin >> f;
                if (f != 'n')
                {
                    cout << "Operator " << node.setOperator(sch) << endl;
                    break;
                }
                cout << "Do you want to continue?(y/n): ";
                fflush(stdin);
                cin >> f;
                if (f != 'y')
                {
                    break;
                }
            }
            cin.get();
        }
        else if (b == 'd')
        {
            while (1)
            {
               
                a = new char[100];
                cout << "Name you want to delete: ";
                fflush(stdin);
                cin >> a;
                lst.freeList(a);
                delete[]a;
                char f;
                cout << "Do you want to continue?(y/n): ";
                fflush(stdin);
                cin >> f;
                if (f != 'y')
                {
                    break;
                }
                cin.get();
            }
        }
        else if (b == 'e')
        {
            break;
        }
        else if (b == 'a')
        {
            while (1)
            {
                a = new char[100];
                cout << "Name: ";
                fflush(stdin);
                cin >> a;
                cout << "Number: ";
                fflush(stdin);
                cin >> nomer;
                lst.insertElement(a, nomer);
                cin.get();
                delete[]a;
                nomer = 0;
                char f;
                cout << "Do you want to continue?(y/n): ";
                fflush(stdin);
                cin >> f;
                if (f != 'y')
                {
                    break;
                }
            }
            cin.get();
        }
        else if (b == 'c')
        {
            lst.displayList();
            cin.get();
        }
        else if (b == 's')
        {
            createSession(lst);
            cout << "The session is successfully saved " << endl;
            lst.deleteList();
        }
        else if (b == 'x')
        {
            long long num;
            
            a = new char[100];
            cout << "Input the name you want to edit " << endl;
            cin >> a;
            cout << "Input the new number " << endl;
            cin >> num;

            lst.EditElement(a,num);
            cin.get();
            delete []a;
        }
        else if (b == 'l')
        {
            readDictionary();
            cin.get();
        }
        //system("clear");
    }
    cin.get();
    return 0;
}



