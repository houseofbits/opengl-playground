#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

class ShaderSourceLoader
{
public:
    typedef std::pair<std::string, std::string> NameValuePair;

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
        globals.push_back(NameValuePair(name, value));
    }

    static void registerGlobal(std::string name, int value)
    {
        registerGlobal(name, std::to_string(value));
    }

    static void registerGlobal(std::string name, unsigned int value)
    {
        registerGlobal(name, std::to_string(value));
    }

    static void registerGlobal(std::string name, float value)
    {
        registerGlobal(name, std::to_string(value));
    }

    static std::list<NameValuePair> globals;

    static void getFilePath(const std::string &fullPath, std::string &pathWithoutFileName)
    {
        size_t found = fullPath.find_last_of("/\\");
        pathWithoutFileName = fullPath.substr(0, found + 1);
    }

    static std::string replaceGlobals(std::string in)
    {
        for (const auto &value : globals)
        {
            std::string replace_word = "${" + value.first + "}";
            std::string replace_by = value.second;

            size_t pos = in.find(replace_word);

            while (pos != std::string::npos)
            {
                in.replace(pos, replace_word.size(), replace_by);
                pos = in.find(replace_word, pos + replace_by.size());
            }
        }
        return in;
    }
};
