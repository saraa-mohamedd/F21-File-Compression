#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

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

unordered_map <string, char> decoding;

string insert_codetable(ifstream& compressedfile, ifstream& tree)
{
    string temp, codeword = "";
    char c; string str;

    compressedfile.get(c);
    while (!compressedfile.eof())
    {
        codeword += c;
        compressedfile.get(c);
    }

    if (!tree.is_open()) cout << "error opening the file" << endl;

    while (!tree.eof())
    {
        tree.get(c);
        tree >> temp;

        decoding.insert({ temp, c });
        tree.get(c);
    }
    tree.close();
    return codeword;
}


string bitstream_decomp(string codes)
{
    string str = "";
    for (int i = 0; i < codes.size() - 2; i++)
    {
        str = str + bitset<8>(codes[i]).to_string();
    }
    int dec = int(codes[codes.size() - 1]);
    string temp = bitset<8>(codes[codes.size() - 2]).to_string();
    str = str + temp.substr(dec, str.size());
    return str;
}


void file_decompression(string words, ofstream& decomp)
{
    string codeword = bitstream_decomp(words);
    if (!decomp.is_open())
        cout << "error!" << endl;
    string temp = "";
    for (int i = 0; i < codeword.length(); i++)
    {
        temp += codeword[i];
        if (decoding.find(temp) != decoding.end())
        {
            decomp << decoding.at(temp);
            temp = "";
        }
    }
}



#endif // !DECOMPRESSION_H
