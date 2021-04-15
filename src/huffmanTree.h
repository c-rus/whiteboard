#pragma once
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "color.h"
#include <list>
#include <fstream>

class HuffmanTree
{
private:
    /*
    Description: node structure to store a colors's integer value and its frequency
    */
    struct Node
    {
        Color color;
        unsigned long frequency;
        Node* left;
        Node* right;
        
        Node(unsigned long f=0, Color c=Color(0,0,0,0), Node* left=nullptr, Node* right=nullptr)
        {
            this->color = c;
            this->frequency = f;
            this->left = left;
            this->right = right;
        };

        ~Node()
        {
            if(left != nullptr) 
            {
                delete left;
                left = nullptr;
            }
            if(right != nullptr)
            {
                delete right;
                right = nullptr;
            }
        };
    };

    /*
    Description: overload operator to create custom compare function to determine order of colors in the heap
    */
    struct NodeCompare
    {
        bool operator()(Node* e1, Node* e2)
        {
            //true if e1 has larger frequency than e2 (larger frequencies in the end of line) or
            //same frequency and therefore sort by lower color integer values going toward heap front
            return e1->frequency > e2->frequency || (e1->color.getIntValue() > e2->color.getIntValue() && e1->frequency == e2->frequency);
        };
    };

    void stashCodes(Node* n, std::string code="");
    void computeHeight(Node* root, int curLevel, unsigned long& deepestLevel);

private:
    std::unordered_map<unsigned int, std::string> asciiCrypt;
    std::unordered_map<unsigned int, unsigned long> asciiFrequency;
    const int INVALID_VALUE = Color::Transparent;
    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> asciiHeap; //custom minHeap
    Node* root = nullptr;
    unsigned int colorCount;
    unsigned long longestBitWidth;
    std::queue<Color> waitToProcess;
    std::queue<unsigned char> bundle;
    
public:
    //must pack bits into groups of 8-bits (1 byte)
    HuffmanTree();
    ~HuffmanTree();

    HuffmanTree(std::fstream& file); //loading a huffman tree from file

    void emplace(Color& c); //update frequ. table and add to process queue

    void generate();
    
    std::string getCode(int value);

    std::queue<unsigned char> encode(); //process every color in the queue and return the bits

    std::string transform(unsigned char c);

    unsigned long getHeight();

    std::queue<Color> decode(std::queue<unsigned char>* compressed=nullptr);

    void save(std::fstream& file); //saving tree codes to file
};
