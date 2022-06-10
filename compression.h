#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <iterator>
#include <cmath>
#include <bitset>
using namespace std;


unordered_map<char, string> encoding;
unordered_map<char, int> hm;

struct Node
{
    char data;
    string codeword;
    int freq;
    Node* left;
    Node* right;

    Node(char d, int f)
    {
        left = right = NULL;
        data = d;
        freq = f;
        codeword = "";
    }
};

struct compare_freq {

    bool operator()(Node* l, Node* r)

    {
        return (l->freq > r->freq);
    }
};


void countFreq(char arr[], int n)
{
    for (int i = 0; i < n; i++)
        hm[arr[i]]++;
}


int number(char x)
{
    return hm[x];
}


void print_tree(struct Node* root, string n, vector<Node>& allLetters)
{
    if (root == NULL)
        return;

    if (root->data != '+')
    {
        root->codeword = root->codeword + n;
        allLetters.push_back(*root);
        encoding.insert({ root->data, root->codeword });
    }

    print_tree(root->left, n + "1", allLetters);
    print_tree(root->right, n + "0", allLetters);
}


void buildHeap(char data[], int freq[], int n, vector<Node>& allLetters)
{
    struct Node* left, * right, * rect = NULL;
    priority_queue<Node*, vector<Node*>, compare_freq> minTree;

    for (int i = 0; i < n; ++i)
    {
        struct Node* newnode = new Node(data[i], freq[i]);
        minTree.push(newnode);
    }
    while (minTree.size() != 1)
    {
        left = minTree.top();
        minTree.pop();

        right = minTree.top();
        minTree.pop();
        rect = new Node('+', left->freq + right->freq);
        rect->left = left;
        rect->right = right;

        minTree.push(rect);
    }
    struct Node* root = minTree.top();
    minTree.pop();

    print_tree(root, "", allLetters);
}


void bitstream_comp(queue<string> codes, ofstream& output)
{
    string str = "";
    do
    {
        str = str + codes.front();
        codes.pop();
        while (str.size() >= 8)
        {
            string r = str.substr(0, 8);
            str = str.substr(8, str.size());
            bitset<8> temp(r);
            output << char(temp.to_ulong());
        }
    } while (!codes.empty());
    
    bitset<8> temp(str);
    output << char(temp.to_ulong());
    output << char(8 - str.size());
}


void file_compression(ifstream& inputfile, ofstream& output)
{
    char c;
    inputfile.get(c);
    queue<string> codes;
    while (!inputfile.eof())
    {
        codes.push(encoding.at(c));
        inputfile.get(c);
    }
    bitstream_comp(codes, output);
    ofstream in("tree.txt");
    if (!in.is_open()) cout << "error opening the file" << endl;

    for (auto& it : encoding)
    {
        in << it.first << " " << it.second << endl;
    }
}


#endif