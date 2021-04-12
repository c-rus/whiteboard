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
    
    //does it have the proper extension? (optional) -parse extension
    std::string fileExt = "";
    if(fname.find_last_of('.') == -1 || fname.find_last_of('.') == 0)
    {
        fname += ext;
        fileExt = ext;
    }
    else
        fileExt = fname.substr(fname.find_last_of('.'));

    //parsing any additional file path directories
    int slash2 = fname.find_last_of('/');
    int slash1 = fname.find_first_of('/')+1;
    if(slash2 > slash1)
        fileDir = fname.substr(slash1, slash2-slash1+1);

    std::fstream file(workDir+fname, std::ios_base::in | std::ios_base::binary);

    //parsing actual file name
    int index = fname.find_last_of('/')+1;
    int len = (fname.find_last_of('.')!=-1) ? fname.find_last_of('.')-index : fname.size()-index;
    std::string title = fname.substr(index, len);

    //does that file exist?
    if(!file.is_open()) 
    {   
        //create a new file if not
        if(fileExt != ext)
            title="";
        Board* b = new Board(w, h, title);
        if(title.size() > 0)
            save(*b, true); //automatically create and save new file
        return b;
    }
    //load from file if so
    std::cout << "Load successful!" << std::endl;
    return new Board(file, w, h, title);
}

bool FileManager::save(Board& b, bool force)
{
    if(!force)
    {
        std::fstream fileExist(workDir+fileDir+b.getName()+ext, std::ios_base::in);
        if(!fileExist.is_open())
        {
            std::cout << "file does not exist!" << std::endl;
            return false;
        }
        fileExist.close();
    }

    std::fstream file(workDir+fileDir+b.getName()+ext, std::ios_base::out | std::ios_base::binary);
    b.save(file);
    return true;
}