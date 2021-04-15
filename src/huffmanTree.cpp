#include "huffmanTree.h"

void HuffmanTree::emplace(Color& c)
{
    //stores frequencies for each element
    auto verify = asciiFrequency.emplace(c.getIntValue(), 0);
    if(verify.second == false)
        asciiFrequency[c.getIntValue()]++;

    waitToProcess.push(c);
}

HuffmanTree::HuffmanTree()
{
    longestBitWidth = 1;
    colorCount = 0;
}

//constructor for loading huffman tree
HuffmanTree::HuffmanTree(std::fstream& file)
{
    //how many colors
    unsigned int numOfColors = 0;
    file.read((char*)&numOfColors, sizeof(numOfColors));
    //create freqeuency table
    for(unsigned int i = 0; i < numOfColors; i++)
    {
        unsigned int colorT = 0;
        unsigned long freq = 0;
        file.read((char*)&colorT, sizeof(colorT));
        file.read((char*)&freq, sizeof(freq));
        asciiFrequency[colorT] = freq;
    }
    std::cout << "here2" << std::endl;
    //generate tree and codes
    generate();

    //read the data
    while(!file.eof())
    {
        unsigned char byte = 0;
        file.read((char*)&byte, sizeof(byte));
        std::cout << transform(byte) << std::endl;
        bundle.push(byte);
    }
    std::cout << "done loading" << std::endl;
}

//save frequency table
void HuffmanTree::save(std::fstream& file)
{
    //how many colors
    file.write((char*)&colorCount, sizeof(colorCount));
    //save freqeuency table
    for(auto it = asciiFrequency.begin(); it != asciiFrequency.end(); it++)
    {
        file.write((char*)&(it->first), sizeof(it->first));
        file.write((char*)&(it->second), sizeof(it->second));
    }

    //save bits
    std::queue<unsigned char> packed = encode();
    while(!packed.empty())
    {
        file.write((char*)&packed.front(), sizeof(packed.front()));
        std::cout << transform(packed.front()) << std::endl;
        packed.pop();
    }
}

void HuffmanTree::computeHeight(Node* root, int curLevel, unsigned long& deepestLevel)
{
    if(curLevel > deepestLevel)
        deepestLevel = curLevel;

    if(root->left != nullptr)
        computeHeight(root->left, curLevel+1, deepestLevel);

    if(root->right != nullptr)
        computeHeight(root->right, curLevel+1, deepestLevel);
};

void HuffmanTree::generate() 
{
    //loop through all ASCII values and only insert when an ASCII value appears (freqeuncy > 0)
    for(auto it = asciiFrequency.begin(); it != asciiFrequency.end(); it++)
    {
        asciiHeap.emplace(new Node(it->second, Color(it->first)));
    }

    colorCount = asciiHeap.size();

    if(colorCount == 0) //do not proceed without any characters
        return;
    
    //build huffman binary tree
    while(asciiHeap.size() > 1) //while heap has at least 2 nodes
    {
        Node* c1 = asciiHeap.top();
        asciiHeap.pop();
        Node* c2 = asciiHeap.top();
        asciiHeap.pop();

        unsigned long frequencySum = c1->frequency + c2->frequency;
            
        if(c1->frequency > c2->frequency)
            asciiHeap.emplace(new Node(frequencySum, Color(INVALID_VALUE), c2, c1));
        else
            asciiHeap.emplace(new Node(frequencySum, Color(INVALID_VALUE), c1, c2));
    }
    root = asciiHeap.top();
    asciiHeap.pop();

    //generate hashmap through full traversal of the huffman binary tree to easily find character codes later
    if(colorCount > 1)
        stashCodes(root);
    else //only 1 character exists to construct
        asciiCrypt[root->color.getIntValue()] = "0";  

    //longest bit-width?
    computeHeight(root, 0, longestBitWidth);
};

/*
Description: destructor to clean up dynamically allocated memory
Time complexity: worst case- O(1)
*/
HuffmanTree::~HuffmanTree()
{
    if(colorCount > 0)
        delete root;
};

/*
Time complexity: worst case- O(1)
*/
std::string HuffmanTree::getCode(int value)
{
    return asciiCrypt[value]; 
};

std::queue<unsigned char> HuffmanTree::encode()
{
    const int MAX_BITS = 8;
    int marker = 0;
    std::queue<unsigned char> package;
    unsigned char current = 0;
    while(!waitToProcess.empty())
    {
        Color& temp = waitToProcess.front();
        waitToProcess.pop();
        std::string code = getCode(temp.getIntValue());

        for(int i = 0; i < code.size(); i++)
        {
            if(code[i] == '1')
                current = current | 1; //input a '1' into lsb

            marker++;
            if(marker == MAX_BITS)  //push char to the result stream
            {
                package.push(current);
                current = 0;
                marker = 0;
            }
            else
                current = current << 1; //bit shift left
        }
    }
    if(marker > 0) 
    {
        //must push any remaining bits
        current = current << (MAX_BITS-marker-1);
        package.push(current);
    }
    return package; 
};

std::string HuffmanTree::transform(unsigned char c)
{
    std::string result = "";
    for(int i = 0; i < 8; i++)
    {
        unsigned char temp = c >> (7-i);
        bool isOne = ((c << i) & 128);
        if(isOne)
            result += "1";
        else
            result += "0";
    }
    return result;
}

unsigned long HuffmanTree::getHeight()
{
    return longestBitWidth;
}

/*
Description: Traverse through binary tree and assign codes to each valid color node
Time complexity: worst case- O(n) where n is the number of nodes in the binary tree
*/
void HuffmanTree::stashCodes(Node* n, std::string code)
{
    if(n->left != nullptr) 
    {
        std::string nextCode = code + "0";
        stashCodes(n->left, nextCode);
    }
    
    //add node's info to the array if it is a valid character
    if(n != nullptr && n->color.getIntValue() != INVALID_VALUE)
    {
        asciiCrypt[n->color.getIntValue()] = code;
        std::cout << "CODE: " << code << "\t" << "for " << n->color.getIntValue() << std::endl;
    }
    
    if(n->right != nullptr) 
    {
        std::string nextCode = code + "1";
        stashCodes(n->right, nextCode);
    }    
};

//decode bytes into a queue of colors
std::queue<Color> HuffmanTree::decode(std::queue<unsigned char>* compressed)
{
    if(compressed == nullptr)
        compressed = &bundle;

    std::queue<Color> decompressed;
    int index = 0;
    unsigned long endIndex = compressed->size()*8;
    //iterate through all bits of compressed string
    if(compressed->size() == 0)
        return decompressed;

    unsigned char byte = compressed->front();
    compressed->pop();

    Node* cur = root;   
    while(index < endIndex)
    {
        //travel down huffman binary tree using bits to guide until hitting a leaf
        //each travel in this loop pushes the iteration of the decode string index
        while(cur != nullptr && cur->color.getIntValue() == INVALID_VALUE && index < endIndex)
        {
            unsigned char temp = byte >> (7-(index%8));
            //which path to take?
            bool isOne = ((byte << (index%8)) & 128);
            if(isOne)
                cur = cur->right;
            else
                cur = cur->left;

            index++;
            if(!compressed->empty() && ((index % 8) == 0)) //are we onto the next byte?
            {
                byte = compressed->front();
                compressed->pop();
            }
        }
        //edge case detection if only one character exists in huffman binary tree
        if(root != nullptr && root->color.getIntValue() != INVALID_VALUE)
        {
            index++;
        }
        //if the final bits do not end at a leaf, then it is invalid
        if(cur != nullptr && cur->color.getIntValue() != INVALID_VALUE)
        {
            decompressed.push(cur->color);
            cur = root;
        }
        
    }

    return decompressed; 
}