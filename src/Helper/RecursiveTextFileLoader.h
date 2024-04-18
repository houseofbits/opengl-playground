#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

class RecursiveTextFileLoader
{
public:
    inline static const std::string INCLUDE_IDENTIFIER = "#include ";

    static std::string load(std::string path)
    {
        std::string fullSourceCode = loadRecursive(path);

        // std::cout << "==================================" << path << std::endl;
        // std::cout << fullSourceCode << std::endl;

        return replaceGlobals(fullSourceCode);
    }

    static std::string loadRecursive(std::string path)
    {
        static bool isRecursiveCall = false;

        std::string fullSourceCode = "";
        std::ifstream file(path);

        if (!file.is_open())
        {
            std::cerr << "RecursiveTextFileLoader::File not found: " << path << "\n"
                      << std::endl;
            return fullSourceCode;
        }

        std::string lineBuffer;
        while (std::getline(file, lineBuffer))
        {
            if (lineBuffer.find(INCLUDE_IDENTIFIER) != lineBuffer.npos)
            {
                lineBuffer.erase(0, INCLUDE_IDENTIFIER.size());

                std::string pathOfThisFile;
                getFilePath(path, pathOfThisFile);
                lineBuffer.insert(0, pathOfThisFile);

                isRecursiveCall = true;
                fullSourceCode += loadRecursive(lineBuffer);

                continue;
            }

            fullSourceCode += lineBuffer + '\n';
        }

        if (!isRecursiveCall)
            fullSourceCode += '\0';

        file.close();

        return fullSourceCode;
    }

    static void registerGlobal(std::string name, std::string value)
    {
        globals.push_back(std::pair<std::string, std::string>(name, value));
    }

private:
    static std::list<std::pair<std::string, std::string>> globals;

    static void getFilePath(const std::string &fullPath, std::string &pathWithoutFileName)
    {
        size_t found = fullPath.find_last_of("/\\");
        pathWithoutFileName = fullPath.substr(0, found + 1);
    }

    static std::string replaceGlobals(std::string in)
    {
        // TODO

        return in;
    }
};
