#include <iostream>
#include "stringParser.h"
#include "CMD.h"
#include "cmdTranslator.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/wait.h>  // wait  
#include <limits.h> // HOST_Name_MAX


int main()
{
    std::vector<std::string> dList;
    std::vector<std::string> dIList;
    std::vector<std::string> limiters;
    dList.push_back(" ");
    dList.push_back("||");
    dList.push_back("&&");
    dList.push_back(";");
    dList.push_back("(");
    dList.push_back(")");
    dList.push_back("[ ");
    dList.push_back(" ]");
    dIList.push_back(" ");
    limiters.push_back("\"");
    limiters.push_back("\'");
    StringParser parsing(dList,dIList,limiters);
    std::string input;
    std::vector<std::string> tokens;
     CMDLine * toExecute = nullptr;
     std::vector<CMDLine *> allocatedCommands;

   const int theInput = 0;
    const int theOutput = 1;
    
    std::cout << "$ ";
    getline(std::cin,input);
    try
    {
    tokens = parsing.parse_string(input);
    std::cout << "token list\n";
    for(size_t i = 0 ; i < tokens.size(); ++i)
    {
	    std::cout << tokens.at(i) << "\n";
        std::cout << "size: " << tokens.at(i).size() << "\n";
    }
    std::cout << "\n";
    

    CMDTranslator translator;
        toExecute = translator.translate(tokens,allocatedCommands);
        std::cout << "executeing\n";
        if(toExecute)
            toExecute->execute(theInput,theOutput);
        else
            std::cout << "to execute failed\n";
    } 
    catch (const std::string e) 
    {
        std::cout << e;
    }


    std::cout << "here! \n";
    std::cout << "\n";
    while (!allocatedCommands.empty()) 
    {
        delete allocatedCommands.back();
        allocatedCommands.back() = nullptr;
        allocatedCommands.pop_back();
    }
    std::cout << "end!\n";
}

