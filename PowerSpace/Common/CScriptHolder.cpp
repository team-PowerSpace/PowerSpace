#include <stdafx.h>
#include <CScriptBuilder.h>
#include "CScriptHolder.h"

std::vector<IdType> CScriptHolder::getScripts( const IdType &key ) const
{
    return Scripts.at( key );
}

bool CScriptHolder::addScript( const IdType& key, const TPath& path )
{
    std::ifstream stream( path.data(), std::ifstream::in ); //The best way to check path validity is trying to open it
    if( !stream.good() ) {
        stream.close();
        std::cout << "The file doesn't exist" << std::endl;
        assert( false );
    }
    stream.close();

    //Deleteing the path and saving only script's filename (with extention)
    std::wstring scriptName = path.substr( path.find_last_of( L"\\/" ) + 1 );

    //Deleting the extention of script
    std::wstring scriptNameWithoutExtention = scriptName.substr( 0, scriptName.find( L"." ) );
    if( Scripts.find( key ) != Scripts.end() ) {
        Scripts.at( key ).push_back( scriptNameWithoutExtention );
        return true;
    } else {
        Scripts[key] = std::vector<IdType>();
        Scripts[key].push_back( scriptNameWithoutExtention );
    }
}

void CScriptHolder::removeScripts( const IdType& objectId )
{
    if( Scripts.find( objectId ) != Scripts.end() ) {
        Scripts.erase( objectId );
    }
}

size_t CScriptHolder::getSize() const
{
    return Scripts.size();
}
