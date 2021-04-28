#pragma once
#include <fstream>
#include "board.h"

class FileManager
{
private:
    std::string workDir, fileDir, fullDir;
    bool loadedFile;
    const std::string ext = ".wb";

public:
    FileManager(std::string& workDir);

    Board* load(std::string& fname, int w, int h, std::string title="");
    bool save(Board& b, bool force=false);
};