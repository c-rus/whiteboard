#include "fileManager.h"

FileManager::FileManager(std::string& workDir)
{
    this->workDir = workDir;
    this->fileDir = "";
}

Board* FileManager::load(std::string& fname, int w, int h)
{
    if(fname.size() < 1)
        return new Board(w, h);
    if(fname.find_last_of('.') == -1 || fname.find_last_of('.') == 0)
        fname += ext;
    std::cout << workDir + fname << std::endl;

    int slash2 = fname.find_last_of('/');
    int slash1 = fname.find_first_of('/')+1;
    if(slash2 > slash1)
    {
        fileDir = fname.substr(slash1, slash2-slash1+1);
    }

    std::fstream file(workDir+fname, std::ios_base::in | std::ios_base::binary);

    if(!file.is_open()) //does that file exist?
        return new Board(w, h);

    int index = 0;
    index = fname.find_last_of('/')+1;
    int len = fname.size()-index;
    if(fname.find_last_of('.')!=-1)
        len = fname.find_last_of('.')-index;
    
    std::string title = fname.substr(index, len);
    std::cout << "TITLE: " << title << std::endl;
    std::cout << "LOADED SUCCESSFUL" << std::endl;
    return new Board(file, w, h, title);
}

bool FileManager::save(Board& b)
{
    std::fstream file(workDir+fileDir+b.getName()+ext, std::ios_base::out | std::ios_base::binary);
    b.save(file);
    return true;
}