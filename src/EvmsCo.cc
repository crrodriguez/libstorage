#include <iostream> 

#include "y2storage/EvmsCo.h"
#include "y2storage/AppUtil.h"
#include "y2storage/Evms.h"

EvmsCo::EvmsCo( Storage * const s, const string& Name ) : 
    Container(s,Name,staticType())
    {
    dev = "/dev/evms";
    if( nm.length()>0 )
	{
	dev += "/" + nm;
	is_container = true;
	}
    else
	is_container = false;
    if( nm == "" )
	{
	pe_size = 0;
	num_pv = 1;
	addToList( new Evms( *this, "sda1" ));
	addToList( new Evms( *this, "sda2" ));
	addToList( new Evms( *this, "sda3" ));
	}
    else if( nm == "vg1" )
	{
	pe_size = 16*1024*1024;
	num_pv = 10;
	addToList( new Evms( *this, "lv1" ));
	addToList( new Evms( *this, "lv2", 2 ));
	addToList( new Evms( *this, "lv3", 3 ));
	addToList( new Evms( *this, "lv4", 4 ));
	addToList( new Evms( *this, "lv5", 5 ));
	addToList( new Evms( *this, "lv6", 6 ));
	}
    else if( nm == "vg2" )
	{
	num_pv = 5;
	pe_size = 256*1024*1024;
	addToList( new Evms( *this, "lv_va2", 2 ));
	}
    else
	{
	num_pv = 1;
	pe_size = 4*1024*1024;
	}
    y2milestone( "constructed evms co %s", dev.c_str() );
    }

EvmsCo::~EvmsCo()
    {
    y2milestone( "destructed evmc co %s", dev.c_str() );
    }


