#include <iostream>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

class TreeNode
{

public:
    int freq;
    char c;
    TreeNode *left, *right;

    TreeNode(int freq_in)
    {
        freq = freq_in;
        left = right = NULL;
        c = 0;
    }
};

struct compareNodes
{
    bool operator()(TreeNode *const &root1, TreeNode *const &root2)
    {
        return root1->freq > root2->freq;
    }
};

TreeNode *encodingTree(string file_name)
{

    FILE *input_file = fopen(file_name.c_str(), "r");

    string text = "";
    char c;

    unordered_map<char, int> freq_map;

    priority_queue<TreeNode *, vector<TreeNode *>, compareNodes> pq;

    TreeNode *curr;

    while ((c = fgetc(input_file)) != EOF)
    {
        freq_map[c] += 1;
    }

    for (auto key : freq_map)
    {
        //cout << key.first;
        curr = new TreeNode(key.second);
        curr->c = key.first;

        pq.push(curr);
    }

    TreeNode *left, *right;

    while (pq.size() > 1)
    {
        left = pq.top();
        pq.pop();

        if (pq.size() > 0)
        {
            right = pq.top();
            pq.pop();
        }
        else
            right = NULL;

        curr = new TreeNode(left->freq + right->freq);
        curr->c = 0;

        curr->left = left;
        curr->right = right;

        pq.push(curr);
    }
    fclose(input_file);
    return curr;
}

void mapToBits(TreeNode *root, unordered_map<char, vector<char>> &map, vector<char> curr)
{
    if (root == NULL)
        return;

    if (root->c != 0)
        map[root->c] = curr;
    /*  if (root->c != 0)
    {
        cout << root->c;
        for (int i = 0; i < curr.size(); i++)
        {
            cout << " " << curr[i];
        }
        cout << "\n";
    }
 */
    curr.push_back('0');
    mapToBits(root->left, map, curr);
    curr.pop_back();
    curr.push_back('1');
    mapToBits(root->right, map, curr);
}

char vecTochar(vector<char> &BUFFER)
{
    char temp = 0;
    while (BUFFER.size() > 0)
    {
        temp *= 2;
        if (BUFFER[BUFFER.size() - 1] != '0')
            temp += 1;
        BUFFER.pop_back();
    }
    //cout << int(temp);
    return temp;
}

void write(vector<char> &compressed)
{
    FILE *fptr = fopen("./_compressed.txt", "w");

    for (int i = 0; i < compressed.size(); i++)
    {
        fprintf(fptr, "%c", compressed[i]);
    }
    fclose(fptr);

    return;
}

string stringify(vector<char> &v)
{
    string ret = "";
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == '0')
            ret += "0";
        else
            ret += "1";
    }

    return ret;
}

unordered_map<string, char> encode(string file_name)
{
    TreeNode *root = encodingTree(file_name);

    unordered_map<char, vector<char>> map;

    mapToBits(root, map, {});

    FILE *input_file = fopen(file_name.c_str(), "r");

    vector<char> BUFFER;
    vector<char> *curr = NULL;
    int i = 0;

    vector<char> compressed;

    char c;

    while ((c = fgetc(input_file)) != EOF)
    {
        curr = &map[c];

        i = 0;
        while (i < curr->size())
        {
            //cout << curr->at(i);
            if (BUFFER.size() < 8)
                BUFFER.push_back(curr->at(i));

            else
            {
                //proces;
                compressed.push_back(vecTochar(BUFFER));

                BUFFER.push_back(curr->at(i));
            }
            i++;
        }
    }

    int padding = 0;

    if (BUFFER.size() > 0)
    {
        while (BUFFER.size() < 8)
        {
            BUFFER.push_back('0');
            padding += 1;
        }
        compressed.push_back(vecTochar(BUFFER));
    }

    write(compressed);
    cout << compressed.size();
    cout << "\n"
         << padding << "\n";

    unordered_map<string, char> decompressor_map;
    //cout << buffer_key;

    for (auto it : map)
    {
        decompressor_map[stringify(it.second)] = it.first;
    }

    fclose(input_file);
    //cout << "\n\n\n";
    return decompressor_map;
}

vector<string> *charTovec(char c)
{
    vector<string> *v = new vector<string>(8);
    int a = c;
    //cout << a << "\n";
    //a = (a + 255) % 256;

    //cout << a << "\n";
    bool neg = false;
    if (a < 0)
    {
        a = 128 + a;
        neg = true;
    }

    for (int i = 0; i < 7; i++)
    {
        if (a % 2 == 0)
            v->push_back("0");
        else
            v->push_back("1");
        a /= 2;
    }

    if (neg)
        v->push_back("1");
    else
        v->push_back("0");

    int i = 0;
    int j = 7;
    string t;
    while (i < j)
    {
        t = v->at(i);
        v->at(i) = v->at(j);
        v->at(j) = t;
        i++;
        j--;
    }
    return v;
}

void decode(string file_name, unordered_map<string, char> map)
{

    FILE *fptr = fopen(file_name.c_str(), "r");

    unordered_map<char, vector<string> *> cache;
    vector<string> *curr;

    string output;
    int i = 0;
    char c;
    string buffer = "";
    string t;
    int count = 0;
    while ((c = fgetc(fptr)) != EOF)
    {
        // /if (count > 100)
        //break;
        //cout << c << ":";
        for (int j = 0; j < curr->size(); j++)
            ;

        //cout << curr->at(j);
        //cout << "\n";
        if (cache.find(c) != cache.end())
            curr = cache[c];
        else
        {
            curr = charTovec(c);
            cache[c] = curr;
        }

        i = 0;
        while (i < curr->size())
        {
            //cout << curr->at(i);
            count++;
            if (map.find(buffer) == map.end())
            {
                buffer = buffer.append(curr->at(i));
                i++;
            }
            else
            {
                //process
                cout << map[buffer];
                buffer = "";
                buffer = buffer.append(curr->at(i));
                i++;
            }
        }
        //cout << buffer << "\n";
        if (map.find(buffer) != map.end())
            ;
    }
}

int main()
{
    unordered_map<char, vector<char>> map;
    //mapToBits(encodingTree("example.txt"), map, {});
    unordered_map<string, char> decompressor_map = encode("example.txt");
    for (auto it : decompressor_map)
    {
        //cout << it.first << " : " << it.second << "\n";
    }
    //cout << "\n\n\nend\n\n\n";
    decode("_compressed.txt", decompressor_map);
    return 0;
}
