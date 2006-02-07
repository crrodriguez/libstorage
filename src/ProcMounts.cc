// Maintainer: fehr@suse.de
/*
  Textdomain    "storage"
*/

#include <sstream>

#include "y2storage/AppUtil.h"
#include "y2storage/SystemCmd.h"
#include "y2storage/ProcMounts.h"

using namespace std;
using namespace storage;

ProcMounts::ProcMounts() 
    {
    ifstream mounts( "/proc/mounts" );
    string line;
    getline( mounts, line );
    while( mounts.good() )
	{
	y2mil( "line:\"" << line << "\"" );
	string dev = extractNthWord( 0, line );
	if( dev!= "rootfs" && dev!="/dev/root" )
	    {
	    co[dev] = extractNthWord( 1, line );
	    }
	getline( mounts, line );
	}
    SystemCmd mt( "mount | grep \" / \"" );
    if( mt.numLines()>0 )
	{
	string dev = extractNthWord( 0, *mt.getLine(0));
	co[dev] = "/";
	}
    mounts.close();
    mounts.clear();
    mounts.open( "/proc/swaps" );
    getline( mounts, line );
    getline( mounts, line );
    while( mounts.good() )
	{
	y2mil( "line:\"" << line << "\"" );
	string::size_type pos;
	string dev = extractNthWord( 0, line );
	if( (pos=dev.find( "\\040(deleted)" ))!=string::npos )
	    {
	    y2mil( "dev:" << dev );
	    dev.erase( pos );
	    }
	co[dev] = "swap";
	getline( mounts, line );
	}
    }

string 
ProcMounts::getMount( const string& dev ) const
    {
    string ret;
    map<string,string>::const_iterator i = co.find( dev );
    if( i!=co.end() )
	ret = i->second;
    return( ret );
    }

string
ProcMounts::getMount( const list<string>& dl ) const
    {
    string ret;
    list<string>::const_iterator i = dl.begin();
    while( ret.empty() && i!=dl.end() )
	{
	ret = getMount( *i );
	++i;
	}
    return( ret );
    }

map<string,string>
ProcMounts::allMounts() const
    {
    map<string,string> ret;
    map<string,string>::const_iterator i = co.begin();
    for( map<string,string>::const_iterator i = co.begin(); i!=co.end(); ++i )
	{
	ret[i->second] = i->first;
	}
    return( ret );
    }


