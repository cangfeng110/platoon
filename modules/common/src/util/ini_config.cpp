#include "modules/common/include/util/ini_config.h"
using namespace std;

namespace atd {
namespace common {
IniConfig::IniConfig( string filename, string delimiter,string comment )
               : m_Delimiter(delimiter),
                 m_Comment(comment)
{
    // Construct a Config, getting keys and values from given file
    std::ifstream in( filename.c_str() );
    if( !in ) throw File_not_found( filename );
    in >> (*this);
}


IniConfig::IniConfig()
    : m_Delimiter( string(1,'=') ),
      m_Comment( string(1,'#') )
{
    // Construct a Config without a file; empty
}


/**
 * @brief Config::KeyExists:Indicate whether key is found
 * @param key
 * @return
 */
bool IniConfig::KeyExists( const string& key ) const
{
    mapci p = m_Contents.find( key );
    return ( p != m_Contents.end() );
}


/**
 * @brief Config::Trim:Remove leading and trailing whitespace
 * @param inout_s
 */
void IniConfig::Trim( string& inout_s )
{
    static const char whitespace[] = " \n\t\v\r\f";
    inout_s.erase( 0, inout_s.find_first_not_of(whitespace) );
    inout_s.erase( inout_s.find_last_not_of(whitespace) + 1U );
}


/**
 * @brief operator <<:Save a Config to os
 * @param os
 * @param cf
 * @return
 */
std::ostream& operator<<( std::ostream& os, const IniConfig& cf )
{
    for( IniConfig::mapci p = cf.m_Contents.begin();p != cf.m_Contents.end();++p )
    {
        os << p->first << " " << cf.m_Delimiter << " ";
        os << p->second << std::endl;
    }
    return os;
}


/**
 * @brief Config::Remove:Remove key and its value
 * @param key
 */
void IniConfig::Remove( const string& key )
{
    m_Contents.erase( m_Contents.find( key ) );
    return;
}


/**
 * @brief operator >>:Load a Config from is,Read in keys and values, keeping internal whitespace
 * @param is
 * @param cf
 * @return
 */
std::istream& operator>>( std::istream& is, IniConfig& cf )
{
    typedef string::size_type pos;
    const string& delim  = cf.m_Delimiter;  // separator
    const string& comm   = cf.m_Comment;    // comment
    const pos skip = delim.length();        // length of separator

    string nextline = "";  // might need to read ahead to see where value ends

    while( is || nextline.length() > 0 )
    {
        // Read an entire line at a time
        string line;
        if( nextline.length() > 0 )
        {
            line = nextline;  // we read ahead; use it now
            nextline = "";
        }
        else
        {
            std::getline( is, line );
        }

        // Ignore comments
        line = line.substr( 0, line.find(comm) );

        // Parse the line if it contains a delimiter
        pos delimPos = line.find( delim );
        if( delimPos < string::npos )
        {
            // Extract the key
            string key = line.substr( 0, delimPos );
            line.replace( 0, delimPos+skip, "" );

            // See if value continues on the next line
            // Stop at blank line, next line with a key, end of stream,
            // or end of file sentry
            bool terminate = false;
            while( !terminate && is )
            {
                std::getline( is, nextline );
                terminate = true;

                string nlcopy = nextline;
                IniConfig::Trim(nlcopy);
                if( nlcopy == "" ) continue;

                nextline = nextline.substr( 0, nextline.find(comm) );
                if( nextline.find(delim) != string::npos )
                    continue;

                nlcopy = nextline;
                IniConfig::Trim(nlcopy);
                if( nlcopy != "" ) line += "\n";
                line += nextline;
                terminate = false;
            }

            // Store key and value
            IniConfig::Trim(key);
            IniConfig::Trim(line);
            cf.m_Contents[key] = line;  // overwrites if key is repeated
        }
    }
    return is;
}


/**
 * @brief Config::FileExist:Check whether the file exist
 * @param filename
 * @return
 */
bool IniConfig::FileExist(std::string filename)
{
    bool exist= false;
    std::ifstream in( filename.c_str() );
    if( in )
        exist = true;
    return exist;
}


/**
 * @brief Config::ReadFile
 * @param filename
 * @param delimiter
 * @param comment
 */
void IniConfig::ReadFile( string filename, string delimiter,string comment )
{
    m_Delimiter = delimiter;
    m_Comment = comment;
    std::ifstream in( filename.c_str() );

    if( !in ) throw File_not_found( filename );

    in >> (*this);
}


/**
 * @brief Config::WriteFile
 * @param filename
 * @param delimiter
 * @param comment
 */
void IniConfig::WriteFile(string filename, string delimiter, string comment)
{
    m_Delimiter = delimiter;
    m_Comment = comment;
    std::ofstream out(filename.c_str());
    if(!out)
        throw File_not_found(filename);
    out << (*this);
}

}
}


