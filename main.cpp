#include "compression.h"
#include "decompression.h"
using namespace std;

int main()
{
    string choice;
    string inPath, outPath;
    cout << "Welcome! \n"
        << "What would you like to do ?\n"
        << "1- compress a file \n"
        << "2- decompress a file \n"
        << "Type in your choice : ";
    getline(cin, choice);

    while (choice != "1" && choice != "2")
    {
        cout << "Invalid choice! Please try again: ";
        getline(cin, choice);
    }

    if (choice == "1")
    {
        cout << "Enter the path of the input text file. \n";
        getline(cin, inPath);
        cout << "Enter the path to write the output file. \n";
        getline(cin, outPath);
        cout << endl << endl;
        ifstream in(inPath);
        if (!in.is_open())
        {
            cout << "error! " << inPath << " can't be opened \n" << endl; return 0;
        }
        in.seekg(0, in.end); //finding the number of characters in the file
        int length = in.tellg(); //length is the length of all characters in the file
        in.seekg(0, in.beg);
        char* file = new char[length];
        in.read(file, length); // inputting all the file characters in file array
        countFreq(file, length);

        char* data = new char[hm.size()];
        int* frequency = new int[hm.size()];
        int i = 0;

        for (auto it = hm.begin(); it != hm.end(); it++)
        {
            data[i] = it->first;
            frequency[i] = it->second;
            i++;
        }
        in.close();

        in.open(inPath);
        ofstream out;
        out.open(outPath, ios::out | ios::binary);
        vector<Node> allLetters;
        buildHeap(data, frequency, hm.size(), allLetters);

        file_compression(in, out);

        //creating compression ratio
        double len = 0;
        for (int i = 0; i < hm.size(); i++)
            len = len + (((double)allLetters[i].freq / length) * allLetters[i].codeword.size());
        double l = len / 8.0;

        //creating code efficency
        double efficiency = 0;
        for (int i = 0; i < hm.size(); i++)
        {
            efficiency = efficiency + (((double)allLetters[i].freq / length) * log2(length / allLetters[i].freq));
        }

        efficiency = efficiency / len;
        cout << ".............................................compression complete! tree has been saved in tree.txt\n";
        cout << "Compression ratio: " << l << endl;
        cout << "Encoding efficiency: " << efficiency * 100 << endl;

    }

    if (choice == "2")
    {
        string treePath;
        cout << "Enter the path of the input compressed file. \n";
        getline(cin, inPath);
        ifstream in(inPath, ios::out | ios::binary);
        cout << "Enter the path of the tree file. \n";
        getline(cin, treePath);
        ifstream tree(treePath);
        cout << "Enter the path to write the output file. \n";
        getline(cin, outPath);
        if (!in.is_open())
        {
            cout << "error! " << inPath << " can't be opened \n" << endl; return 0;
        }
        if (!tree.is_open())
        {
            cout << "error! " << inPath << " can't be opened \n" << endl; return 0;
        }
        ofstream out(outPath);
        string codeword = insert_codetable(in, tree);
        file_decompression(codeword, out);
        in.close();
        out.close();
        cout << endl << endl << ".............................................decompression complete! \n";
        //put the decompression code here
    }

    return 0;
}
