#include "cmdTranslator.h"
#include "Exit.h"
#include "And.h"
#include "Or.h"
#include "Semicolon.h"
#include <cstdlib>

CMDTranslator::CMDTranslator()
{
    connectors.push_back("&&");
    connectors.push_back("||");
    connectors.push_back(";");
}

CMDTranslator::~CMDTranslator()
{
}

void CMDTranslator::remove_comment(std::vector<std::string> &tokenList)
{
    tokenList.erase(tokenList.begin(), tokenList.end());
}

void ::CMDTranslator::remove_literals(std::string &token)
{
    bool done = false;
    size_t pos;
    while (!done)
    {
        if ((pos = token.find("\"")) < token.size())
        {
            token.erase(pos, 1);
        }
        else if ((pos = token.find("\'")) < token.size())
        {
            token.erase(pos, 1);
        }
        else
        {
            done = true;
        }
    }
}

CMD *CMDTranslator::make_CMD(std::vector<std::string> &tokenList)
{
    std::vector<std::string> arguments;
    CMD *cmd = NULL;

    //while the token list is not empty and is is not a connector,
    //and is not a comment
    // then pushes the token into the argument list
    while (!tokenList.empty() && !is_Connector(tokenList.front())
           && !(tokenList.front().find("#") == 0))
    {
        remove_literals(tokenList.front());
        arguments.push_back(tokenList.front());
        tokenList.erase(tokenList.begin());
    }

    //if the argument is only exit
    if (arguments.front() == "exit")
    {
        cmd = new Exit();
    }
    //give the argumentlist to the newly created command object if
    //it is not a exit command
    else
    {
        cmd = new CMD(arguments);
    }

    return cmd;
}

Connector *CMDTranslator::make_connector(std::vector<std::string> &tokenList)
{
    Connector *newConnector = NULL;
    if (tokenList.front() == connectors.at(0))
    {
        newConnector = new And();
    }
    else if (tokenList.front() == connectors.at(1))
    {
        newConnector = new Or();
    }
    else
    {
        newConnector = new Semicolon();
    }

    tokenList.erase(tokenList.begin());
    return newConnector;
}

bool CMDTranslator::is_Connector(const std::string &item)
{
    return (std::find(connectors.begin(), connectors.end(), item) !=
            connectors.end());
}

CMDLine *CMDTranslator::translate(std::vector<std::string> tokenList,
                                  std::vector<CMDLine *> &returnList)
{
    CMDLine *toReturn = NULL;
    std::vector<CMDLine *> items;
    Connector *temp = NULL;

    if (!tokenList.empty() && is_Connector(tokenList.front()))
    {
        const std::string error =
            "Error : -bash: syntax error near unexpected token "
                + tokenList.front() + "\n";
        throw error;
    }
    //while there are still tokens
    while (!tokenList.empty())
    {
        //if there is less than 2 items to connect
        if (items.size() < 2)
        {
            //if it is a comment at the front
            //then erase all the tokens in the list
            if (tokenList.front().find("#") == 0)
            {
                remove_comment(tokenList);
            }
            //else if the token is a connector and the previous item
            //is not a connector
            //then make a new connector item
            else if (temp == NULL && is_Connector(tokenList.front()))
            {
                temp = make_connector(tokenList);
                returnList.push_back(temp);
            }
            //else if the token not a connector, then create a command
            else if (!is_Connector(tokenList.front()))
            {
                items.push_back(make_CMD(tokenList));
                returnList.push_back(items.back());
            }
            //else it is an invalid statement and should throw and error
            else
            {
                const std::string error2 =
                    "Error : -bash: syntax error near unexpected token "
                        + tokenList.front() + "\n";
                throw error2;
            }
        }
        else
        {
            //settting the left and right children of the connector
            //and add it to the item list
            temp->setRight(items.back());
            items.pop_back();
            temp->setLeft(items.back());
            items.pop_back();
            items.push_back(temp);
            temp = NULL;
        }
    }

    //if there is still a connector and one item left
    if (temp && (items.size() == 1))
    {
        temp->setLeft(items.back());
        items.pop_back();
        toReturn = temp;
    }
    //if there is still a connector and two itms left
    else if (temp && (items.size() == 2))
    {
        temp->setRight(items.back());
        items.pop_back();
        temp->setLeft(items.back());
        items.pop_back();
        toReturn = temp;
    }
    //if there was only a command
    else if (!temp && (items.size() == 1))
    {
        toReturn = items.back();
        items.pop_back();
    }
    //the line had nothing in the first place
    else
    {
        std::vector<std::string> emptyArg;
        toReturn = new CMD(emptyArg);
        returnList.push_back(toReturn);
    }

    return toReturn;
}
