#ifndef CONFIG_H_
#define CONFIG_H_

//#include "global.hpp"
#include <sstream>
#include <map>
#include <fstream>

namespace atd {
namespace common {

class IniConfig
{
protected:
    std::string m_Delimiter;  //!< separator between key and value
    std::string m_Comment;    //!< separator between value and comments
    std::map<std::string,std::string> m_Contents;  //!< extracted keys and values

    typedef std::map<std::string,std::string>::iterator mapi;
    typedef std::map<std::string,std::string>::const_iterator mapci;
public:

    IniConfig( std::string filename,std::string delimiter = "=",std::string comment = "#" );
    IniConfig();
    template<class T> T Read( const std::string& in_key ) const;  //!<Search for key and read value or optional default value, call as read<T>
    template<class T> T Read( const std::string& in_key, const T& in_value ) const;
    template<class T> bool ReadInto( T& out_var, const std::string& in_key ) const;
    template<class T>
    bool ReadInto( T& out_var, const std::string& in_key, const T& in_value ) const;
    bool FileExist(std::string filename);
    void ReadFile(std::string filename,std::string delimiter = "=",std::string comment = "#" );

    void WriteFile(std::string filename,std::string delimiter = "=",std::string comment = "#");

    // Check whether key exists in configuration
    bool KeyExists( const std::string& in_key ) const;

    // Modify keys and values
    template<class T> void Add( const std::string& in_key, const T& in_value );
    void Remove( const std::string& in_key );
    template<class T> void Change( const std::string& in_key, const T& in_value);

    // Check or change configuration syntax
    std::string GetDelimiter() const { return m_Delimiter; }
    std::string GetComment() const { return m_Comment; }
    std::string SetDelimiter( const std::string& in_s )
    { std::string old = m_Delimiter;  m_Delimiter = in_s;  return old; }
    std::string SetComment( const std::string& in_s )
    { std::string old = m_Comment;  m_Comment =  in_s;  return old; }

    // Write or read configuration
    friend std::ostream& operator<<( std::ostream& os, const IniConfig& cf );
    friend std::istream& operator>>( std::istream& is, IniConfig& cf );

protected:
    template<class T> static std::string T_as_string( const T& t );
    template<class T> static T string_as_T( const std::string& s );
    static void Trim( std::string& inout_s );


    // Exception types
public:
    struct File_not_found
    {
        std::string filename;
        File_not_found( const std::string& filename_ = std::string() )
            : filename(filename_) {}
    };
    struct Key_not_found
    {
        std::string key;
        Key_not_found( const std::string& key_ = std::string() )
            : key(key_) {}
    };
};


/**
 * static
 * @brief Config::T_as_string:Convert from a T to a string,Type T must support << operator
 * @param t
 * @return
 */
template<class T>
std::string IniConfig::T_as_string( const T& t )
{
    std::ostringstream ost;
    ost << t;
    return ost.str();
}


/**
 * @brief Config::string_as_T:Convert from a string to a T,Type T must support >> operator
 * @param s
 * @return
 */
template<class T>
T IniConfig::string_as_T( const std::string& s )
{
    T t;
    std::istringstream ist(s);
    ist >> t;
    return t;
}


/**
 * @brief Config::string_as_T<std::string> :Convert from a string to a string
 * @param s
 * @return
 */
template<>
inline std::string IniConfig::string_as_T<std::string>( const std::string& s )
{
    return s;
}


/**
 * @brief Config::string_as_T<bool>:Convert from a string to a bool;
 * Interpret "false", "F", "no", "n", "0" as false
 * Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
 * @param s
 * @return
 */
template<>
inline bool IniConfig::string_as_T<bool>( const std::string& s )
{
    bool b = true;
    std::string sup = s;
    for( std::string::iterator p = sup.begin(); p != sup.end(); ++p )
        *p = toupper(*p);  // make string all caps
    if( sup==std::string("FALSE") || sup==std::string("F") ||
        sup==std::string("NO") || sup==std::string("N") ||
        sup==std::string("0") || sup==std::string("NONE") )
        b = false;
    return b;
}

/**
 * @brief Config::Read:Read the value corresponding to key
 * @param key
 * @return
 */
template<class T>
T IniConfig::Read( const std::string& key ) const
{
    mapci p = m_Contents.find(key);
    if( p == m_Contents.end() ) throw Key_not_found(key);
    return string_as_T<T>( p->second );
}

/**
 * @brief Config::Read:Return the value corresponding to key or given default value,if key is not found
 * @param key
 * @param value
 * @return
 */
template<class T>
T IniConfig::Read( const std::string& key, const T& value ) const
{
    mapci p = m_Contents.find(key);
    if( p == m_Contents.end() ) return value;
    return string_as_T<T>( p->second );
}

/**
 * @brief Config::ReadInto:Get the value corresponding to key and store in var
 * @param var
 * @param key
 * @return : Return true if key is found,Otherwise leave var untouched
 */
template<class T>
bool IniConfig::ReadInto( T& var, const std::string& key ) const
{
    mapci p = m_Contents.find(key);
    bool found = ( p != m_Contents.end() );
    if( found ) var = string_as_T<T>( p->second );
    return found;
}

/**
 * @brief Config::ReadInto:Get the value corresponding to key and store in var
 * @param var
 * @param key
 * @param value
 * @return :Return true if key is found,Otherwise set var to given default
 */
template<class T>
bool IniConfig::ReadInto( T& var, const std::string& key, const T& value ) const
{
    mapci p = m_Contents.find(key);
    bool found = ( p != m_Contents.end() );
    if( found )
        var = string_as_T<T>( p->second );
    else
        var = value;
    return found;
}

/**
 * @brief Config::Add:Add a key with given value
 * @param in_key
 * @param value
 */
template<class T>
void IniConfig::Add( const std::string& in_key, const T& value )
{
    std::string v = T_as_string( value );
    std::string key=in_key;
    Trim(key);
    Trim(v);
    m_Contents[key] = v;
    return;
}

/**
 * @brief Config::Change:Change the value of the a key
 * @param in_key
 * @param in_value
 */
template<class T>
void IniConfig::Change(const std::string& in_key, const T& in_value)
{
    mapci p = m_Contents.find(in_key);
    std::string v = T_as_string( in_value );
    if( p == m_Contents.end())
        throw Key_not_found(in_key);
    m_Contents[in_key] = v;
    return;
}

}
}


#endif
