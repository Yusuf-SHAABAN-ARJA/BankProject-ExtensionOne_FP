#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace MyStringLib
{
   
    vector<string> vSplitString(string S1, string Delim = "#//#")
    {
        vector<string> vString;
        short pos = 0;
        string sWord; 

       
        while ((pos = S1.find(Delim)) != std::string::npos)
        {
            sWord = S1.substr(0, pos);
            if (sWord != "")
            {
                vString.push_back(sWord);
            }
            S1.erase(0, pos + Delim.length());
        }

        if (S1 != "")
        {
            vString.push_back(S1); 
        }

        return vString;
    }
}