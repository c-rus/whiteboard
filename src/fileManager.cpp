#include "fileManager.h"

FileManager::FileManager(std::string& workDir)
{
    this->workDir = workDir;
    this->fileDir = "";
    this->fullDir = "";
    loadedFile = false;
}

Board* FileManager::load(std::string& fname, int w, int h, std::string title)
{
    if(fname.size() < 1)
        return new Board(w, h, title);
    
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

    std::fstream file(fname, std::ios_base::in | std::ios_base::binary);
    std::cout << "actual filepath: " << fname << std::endl;
    //parsing actual file name
    int index = fname.find_last_of('/')+1;
    int len = (fname.find_last_of('.')!=-1) ? fname.find_last_of('.')-index : fname.size()-index;
    title = fname.substr(index, len);

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
    loadedFile = true;
    fullDir = fname;
    return new Board(file, w, h, title);
}

bool FileManager::save(Board& b, bool force)
{
    if(loadedFile)
    {
        std::fstream prevFile(fullDir, std::ios_base::in);
        if(prevFile.is_open())
        {
            prevFile.close();
            std::fstream saveFile(fullDir, std::ios_base::out | std::ios_base::binary);
            b.save(saveFile);
            return true;
        }
    }
    else if(!force)
        return false;

    //save as a new file
    fullDir = workDir+"/"+b.getName()+ext;
    loadedFile = true;
    std::fstream newFile(fullDir, std::ios_base::out | std::ios_base::binary);
    b.save(newFile);
    return true;
}