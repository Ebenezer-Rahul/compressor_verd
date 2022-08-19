#include <bits/stdc++.h>
#include <sstream>
using namespace std;

unordered_map<char,string> charsEncodedMap;

vector<string> binary_rep_chars = {"00000000", "00000001", "00000010", "00000011", "00000100", "00000101", "00000110", "00000111", "00001000", "00001001", "00001010", "00001011", "00001100", "00001101", "00001110", "00001111", "00010000", "00010001", "00010010", "00010011", "00010100", "00010101", "00010110", "00010111", "00011000", "00011001", "00011010", "00011011", "00011100", "00011101", "00011110", "00011111", "00100000", "00100001", "00100010", "00100011", "00100100", "00100101", "00100110", "00100111", "00101000", "00101001", "00101010", "00101011", "00101100", "00101101", "00101110", "00101111", "00110000", "00110001", "00110010", "00110011", "00110100", "00110101", "00110110", "00110111", "00111000", "00111001", "00111010", "00111011", "00111100", "00111101", "00111110", "00111111", "01000000", "01000001", "01000010", "01000011", "01000100", "01000101", "01000110", "01000111", "01001000", "01001001", "01001010", "01001011", "01001100", "01001101", "01001110", "01001111", "01010000", "01010001", "01010010", "01010011", "01010100", "01010101", "01010110", "01010111", "01011000", "01011001", "01011010", "01011011", "01011100", "01011101", "01011110", "01011111", "01100000", "01100001", "01100010", "01100011", "01100100", "01100101", "01100110", "01100111", "01101000", "01101001", "01101010", "01101011", "01101100", "01101101", "01101110", "01101111", "01110000", "01110001", "01110010", "01110011", "01110100", "01110101", "01110110", "01110111", "01111000", "01111001", "01111010", "01111011", "01111100", "01111101", "01111110", "01111111", "10000000", "10000001", "10000010", "10000011", "10000100", "10000101", "10000110", "10000111", "10001000", "10001001", "10001010", "10001011", "10001100", "10001101", "10001110", "10001111", "10010000", "10010001", "10010010", "10010011", "10010100", "10010101", "10010110", "10010111", "10011000", "10011001", "10011010", "10011011", "10011100", "10011101", "10011110", "10011111", "10100000", "10100001", "10100010", "10100011", "10100100", "10100101", "10100110", "10100111", "10101000", "10101001", "10101010", "10101011", "10101100", "10101101", "10101110", "10101111", "10110000", "10110001", "10110010", "10110011", "10110100", "10110101", "10110110", "10110111", "10111000", "10111001", "10111010", "10111011", "10111100", "10111101", "10111110", "10111111", "11000000", "11000001", "11000010", "11000011", "11000100", "11000101", "11000110", "11000111", "11001000", "11001001", "11001010", "11001011", "11001100", "11001101", "11001110", "11001111", "11010000", "11010001", "11010010", "11010011", "11010100", "11010101", "11010110", "11010111", "11011000", "11011001", "11011010", "11011011", "11011100", "11011101", "11011110", "11011111", "11100000", "11100001", "11100010", "11100011", "11100100", "11100101", "11100110", "11100111", "11101000", "11101001", "11101010", "11101011", "11101100", "11101101", "11101110", "11101111", "11110000", "11110001", "11110010", "11110011", "11110100", "11110101", "11110110", "11110111", "11111000", "11111001", "11111010", "11111011", "11111100", "11111101", "11111110", "11111111"};

class TreeNode {
    public :
        TreeNode *left,*right;
        int freq;
        char c;
        TreeNode (char _c,int _freq)
        {
            c = _c;
            freq = _freq;
            left = right = NULL;
        }
};

struct myComp 
{
    bool operator()(TreeNode* &a, TreeNode* &b)
    {
        return a->freq > b->freq;
    }
};


string INPUT_STRING;
string ENCODED_STRING_BINARY;
int OFFSET;
string ENCODED_STRING;
string DECODED_STRING;
void loadFile(string FILE_NAME, string &str)
{
    INPUT_STRING = "";
    ifstream input_file(FILE_NAME);
    input_file.seekg(0, std::ios::end);
    str.reserve(input_file.tellg());
    input_file.seekg(0, std::ios::beg);
    
    str.assign((std::istreambuf_iterator<char>(input_file)),
            std::istreambuf_iterator<char>());

}

TreeNode *generate_encoding_tree(string &INPUT_STRING)
{
    unordered_map<char, int> freqs;
    for(char &c : INPUT_STRING)
        freqs[c] += 1;
    priority_queue<TreeNode*, vector<TreeNode*>, myComp> pq;
    TreeNode *curr;
    for(auto it : freqs)
    {
        curr = new TreeNode(it.first,it.second);
        pq.push(curr);
    }
    TreeNode *left,*right;
    int total = 0;
    while(pq.size() > 1)
    {
        left = pq.top();
        pq.pop();
        total = left->freq;
        if(pq.size() > 0)
        {
            right = pq.top();
            pq.pop();
            total += right->freq;
        }
        else 
            right = NULL;

        curr = new TreeNode(0,total);
        curr->left = left;
        curr->right = right;
        pq.push(curr);
    }
    return pq.top();
}

void mapTobits(TreeNode *root,string &curr)
{
    if(root == NULL)
        return;
    if(root->left == NULL && root->right == NULL)
    {
        charsEncodedMap[root->c] = curr; 
        return;
    }
    curr.push_back('0');
    mapTobits(root->left,curr);
    curr.pop_back();
    curr.push_back('1');
    mapTobits(root->right,curr);
    curr.pop_back();

}

void encode(string &INPUT_STRING)
{
    ENCODED_STRING = "";
    ENCODED_STRING_BINARY = "";

    for(char &c : INPUT_STRING)
    {
        for(char &t : charsEncodedMap[c])
            ENCODED_STRING_BINARY.push_back(t);
    }
    int total_size = (int)ENCODED_STRING_BINARY.length();
    OFFSET = total_size % 8;
    for(int i = 0; i < 8 - OFFSET; i++)
        ENCODED_STRING_BINARY.push_back('0');

    unsigned char curr = 0;
    int j;
    for(int i = 0; i < ENCODED_STRING_BINARY.length();) 
    {
        curr = 0;
        for(j = i; j < i + 8; j++)
        {
            curr *= 2;
            curr += ENCODED_STRING_BINARY[j] - '0';
        }
        i = j;
        ENCODED_STRING.push_back((char)curr);
    }
}

void decode(string &ENCODED_STRING , TreeNode *root, int OFFSET)
{
    ENCODED_STRING_BINARY = "";
    int index;
    for(char &c : ENCODED_STRING)
    {
        if((int)c < 0)
            index = (int) c + 256;
        else 
            index = (int)c;
        for(char &t : binary_rep_chars[index])
            ENCODED_STRING_BINARY.push_back(t);
    }
    
    int i = 0;
    int total_size = (int)ENCODED_STRING_BINARY.length() - (8 - OFFSET);
    TreeNode *curr;
    DECODED_STRING = "";
    while(i < total_size)
    {
        curr = root;
        while(!(curr->left == NULL && curr->right == NULL))
        {
            if(ENCODED_STRING_BINARY[i] == '1')
            {
                curr = curr->right;
            }
            else
            {
                curr = curr->left;
            }
            i++;
        }
        DECODED_STRING.push_back(curr->c);
    }
}


void write_file(string &ENCODED_STRING, string FILE_NAME)
{
    ofstream file(FILE_NAME);
    file << ENCODED_STRING;
}




int main()
{
    string FILE_NAME;
    cout<<"ENTER THE NAME OF FILE YOU WANT TO COMPRESS : ";
    cin >> FILE_NAME;
    cout<<"COMPRESSING........"<<endl;
    loadFile(FILE_NAME ,INPUT_STRING);
    TreeNode *root = generate_encoding_tree(INPUT_STRING);
    string temp;
    mapTobits(root, temp);
    encode(INPUT_STRING);
    cout<<"Size of the orginal file is : "<<root->freq<<" bits .."<<endl;
    cout<<"Size of the compressed FILE is : "<<ENCODED_STRING.size()<< " bits .."<<endl;
    float compression_ratio = (float)root->freq / (int) ENCODED_STRING.size();
    cout<<"compression ratio : "<<compression_ratio;
    if(compression_ratio > 1 && compression_ratio < 1.5)
        cout<<" it went great";
    else if(compression_ratio > 1.5)
        cout<<" it went fucking Great!!!!!";
    cout<<endl;
    string FILENAME_EXT_REM = "";
    for(int i = 0; i < (int)FILE_NAME.length() - 4; i++)
        FILENAME_EXT_REM.push_back(FILE_NAME[i]);
    write_file(ENCODED_STRING, FILENAME_EXT_REM + "_compressed.txt");
    cout<<"COMPRESSON DONE -_- ! "<<endl;
    loadFile(FILENAME_EXT_REM + "_compressed.txt", ENCODED_STRING); 
    decode(ENCODED_STRING, root, OFFSET);
    cout<<"DECOMPRESSING THE FILE ........"<<endl;
    write_file(DECODED_STRING,FILENAME_EXT_REM + "_uncompressed.txt");
    cout<<"DECOMPRESSION DONE -_- !"<<endl;
    return 0;
}

