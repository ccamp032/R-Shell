#include "TestCmd.h"
#include <algorithm>
#include <cstring>
TestCmd::TestCmd()
{

}
TestCmd::TestCmd(std::vector<std::string> arugmentListVector)
{
    this->arugmentListVector = arugmentListVector;
}
TestCmd::~TestCmd()
{
}
char* TestCmd::getFilename()
{
    return nullptr;
}

bool TestCmd::execute(int inputFile, int outputFile)
{

    // int stat(const char *path, struct stat *buf);
    // stat() stats the file pointed to by path and fills in buf.

    //A pointer to a buffer where the function can store the information
    // The path of the file or directory that you want information about.

    //On success, zero is returned.
    //On error, -1 is returned, and errno is set appropriately.

    struct stat statBuffer; //A pointer to a buffer where the function can
    // store the information
    
    bool flagExist = false;
    bool toExecuted = false;
    char *flagTestPath;
    std::vector<std::string> storage;
    std::vector<std::string> flags;

    storage.push_back("test");
    storage.push_back("[");
    storage.push_back("]");
    
    flags.push_back("-e");
    flags.push_back("-f");
    flags.push_back("-d");



    if(arugmentListVector.size() == 2)
    {
        if(std::find(flags.begin(),flags.end(),arugmentListVector.at(0)) !=
                flags.end())
        {
            flagExist = true;
            flagTestPath = new char[arugmentListVector.at(1).size()];
            strcpy(flagTestPath,arugmentListVector.at(1).c_str() );
        }
        else
        {
            std::string error = "test: invalid option -- '" +
                    arugmentListVector.at(0) + "'\n";
            throw error;
        }

    }
    else if( arugmentListVector.size() == 1 )
    {
        if(arugmentListVector.front().size() == 2 &&
                arugmentListVector.at(0).find("-") == 0 &&
                std::find(flags.begin(),flags.end(),arugmentListVector.at(0)) ==
                flags.end())
        {
            std::string error = "test: invalid option -- '" +
                    arugmentListVector.at(0) + "'\n";
            throw error;
        }
        else if (arugmentListVector.front().size() == 2 &&
                 arugmentListVector.at(0).find("-") == 0 &&
                 std::find(flags.begin(),flags.end(),arugmentListVector.at(0))
                 != flags.end())
        {
            std::cout << "(False)" << std::endl;
            return toExecuted; // return false
        }
        else
        {
            flagExist = false;
            flagTestPath = new char[arugmentListVector.at(0).size()];
            strcpy(flagTestPath,arugmentListVector.at(0).c_str() );
        }
    }
    else
    {
        std::cout << "(False)" << std::endl;
        return toExecuted; // return false
    }


    if (stat(flagTestPath, &statBuffer) == -1)// check if the stat function fails
    {
        std::cout << "(False)\n";
        return toExecuted; // return false
    }

    // Check for string literals
    // 'Software Construction'
    //  test -e "Software Construction"/

    //****************************************************************//

    if (flagExist)
    {
        if (  arugmentListVector.at(0) == flags.at(0)  &&
              (S_ISDIR(statBuffer.st_mode) || S_ISREG(statBuffer.st_mode) ) )
        {
            std::cout << "(True)\n";
            toExecuted = true;
        }
        else if ( arugmentListVector.at(0) == flags.at(1)  &&
                  S_ISREG(statBuffer.st_mode))
        {
            std::cout << "(True)\n";
            toExecuted = true;
        }
        else if ( arugmentListVector.at(0) == flags.at(2)  &&
                  S_ISDIR(statBuffer.st_mode))
        {
            std::cout << "(True)\n";
            toExecuted = true;
        }
        else
        {
            std::cout << "(False)\n";
            return false;
        }

    }
    else if( ( flagExist == false ) && ( S_ISDIR(statBuffer.st_mode) ||
                                         S_ISREG(statBuffer.st_mode) ) )
    {
        std::cout << "(True)\n";
        toExecuted = true;
    }
    else
    {
        std::cout << "(false)" << std::endl;
    }
    delete [] flagTestPath;
    flagTestPath = nullptr;
    return toExecuted;
}

// test is an object
// no flag is vector size is one
// flag is vector size is two
