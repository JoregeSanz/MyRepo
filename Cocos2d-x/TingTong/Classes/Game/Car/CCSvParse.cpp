//
//  CCSvParse.cpp
//  TingTong
//
//  Created by admin on 5/26/14.
//
//

#include "CCSvParse.h"
CCSVParse::~CCSVParse(void)
{
    
}

void CCSVParse::StringSplit( const string& str, vector<string>& tokens, const char& delimiters )
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos-lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}


void CCSVParse::split( vector<string>& field, string line )
{
    string fld;
    unsigned int i,j=0;
    
    if( line.length() ==0 )
        return;
    i=0;
    
    do
    {
        if(j<line.length() && line[i]=='"')
            j = advquoted(line, fld, ++i);
        else
            j = advplain(line, fld, i);
        
        field.push_back(fld);
        i = j+1;
    } while (j<line.length());
}



int CCSVParse::advplain( const string& s, string& fld, int i)
{
    unsigned int j;
    j = s.find_first_of(fieldsep, i);
    if(j>s.length())
        j=s.length();
    fld = string(s,i,j-i);
    return j;
}

int CCSVParse::advquoted( const string& s, string& fld, int i)
{
    unsigned int j;
    fld = "";
    for (j=i; j<s.length(); ++j)
    {
        if(s[j]=='"' && s[++j]!='"')
        {
            unsigned int k = s.find_first_of(fieldsep, j);
            if(k>s.length())
                k = s.length();
            for(k-=j; k-->0;)
                fld += s[j++];
            break;
        }
        fld += s[j];
    }
    return j;
}

//解析 CVS 文件
bool CCSVParse::openFile( const char* fileName )
{
    string pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    unsigned char* pBuffer = NULL;
    unsigned long bufferSize = 0;
    pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pathKey.c_str(), "r", &bufferSize);
    
    string s = (char*)pBuffer;
    string str = s.substr(0,bufferSize);
    
    vector<string> line;
    StringSplit(str, line, '\n');
    for(unsigned int i=0; i<line.size(); ++i)
    {
        vector<string> field;
        split(field, line[i]);
        data.push_back(field);
        cols = max(cols, (int)field.size());
    }
    
    return true;
}

//获取指定行列的数据
string CCSVParse::getData(unsigned int rows, unsigned int cols )
{
    if (rows<0 || rows>=data.size() || cols<0 || cols>=data[rows].size())
    {
        return "";
    }
    return data[rows][cols];
}

//获取指定数据的列下标
int CCSVParse::findColsData( int cols, const char* value )
{
    for (unsigned int i=0; i<data.size(); ++i)
    {
        if(strcmp(getData(i,cols).c_str(),value)==0)
            return i;
    }
    return -1;
}
