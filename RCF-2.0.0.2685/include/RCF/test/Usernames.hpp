
//******************************************************************************
// RCF - Remote Call Framework
//
// Copyright (c) 2005 - 2013, Delta V Software. All rights reserved.
// http://www.deltavsoft.com
//
// RCF is distributed under dual licenses - closed source or GPL.
// Consult your particular license for conditions of use.
//
// If you have not purchased a commercial license, you are using RCF 
// under GPL terms.
//
// Version: 2.0
// Contact: support <at> deltavsoft.com 
//
//******************************************************************************

#ifndef INCLUDE_RCF_TEST_USERNAMES_HPP
#define INCLUDE_RCF_TEST_USERNAMES_HPP

#include <fstream>
#include <string>

class Usernames
{
public:
    std::string mLocalUsername;
    std::string mLocalPassword;
    std::string mLocalPasswordBad;
    std::string mAdUsername;
    std::string mAdPassword;
    std::string mAdPasswordBad;
    std::string mAdDomain;
};

bool getUsernames(Usernames & usernames)
{
    std::string whichFile = RCF::getRelativeTestDataPath() + "sspi.txt";

    std::ifstream fin(whichFile.c_str());
    
    if (!fin)
    {
        return false;
    }
    
    fin >> usernames.mLocalUsername;
    fin >> usernames.mLocalPassword;
    fin >> usernames.mLocalPasswordBad;
    fin >> usernames.mAdUsername;
    fin >> usernames.mAdPassword;
    fin >> usernames.mAdPasswordBad;
    fin >> usernames.mAdDomain;
    
    assert(fin);
    
    if (!fin)
    {
        return false;
    }

    fin.close();

    return true;
}

#endif // ! INCLUDE_RCF_TEST_USERNAMES_HPP
