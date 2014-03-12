/*
 * Copyright (c) [2004-2014] Novell, Inc.
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H


#include <boost/noncopyable.hpp>

#include "storage/SystemInfo/ProcParts.h"
#include "storage/SystemInfo/ProcMounts.h"
#include "storage/SystemInfo/ProcMdstat.h"
#include "storage/SystemInfo/CmdBlkid.h"
#include "storage/SystemInfo/CmdLsscsi.h"
#include "storage/SystemInfo/CmdParted.h"
#include "storage/SystemInfo/CmdDasdview.h"
#include "storage/SystemInfo/CmdDmsetup.h"
#include "storage/SystemInfo/CmdCryptsetup.h"
#include "storage/SystemInfo/CmdDmraid.h"
#include "storage/SystemInfo/CmdMultipath.h"
#include "storage/SystemInfo/CmdBtrfs.h"
#include "storage/SystemInfo/CmdLvm.h"
#include "storage/SystemInfo/Dev.h"


namespace storage
{
    using std::map;
    using std::list;


    class SystemInfo : boost::noncopyable
    {

    public:

	SystemInfo();
	~SystemInfo();

	const UdevMap& getUdevMap(const string& path) { return udevmaps.get(path); }
	const MdLinks& getMdLinks() { return mdlinks.get(); }
	const ProcParts& getProcParts() { return procparts.get(); }
	const ProcMounts& getProcMounts() { return procmounts.get(); }
	const ProcMdstat& getProcMdstat() { return procmdstat.get(); }
	const MdadmDetails& getMdadmDetails(const string device) { return mdadmdetails.get(device); }
	const MdadmExamine& getMdadmExamine(const list<string>& devices) { return mdadmexamines.get(devices); }
	const Blkid& getBlkid() { return blkid.get(); }
	const Lsscsi& getLsscsi() { return lsscsi.get(); }
	const Parted& getParted(const string& device) { return parteds.get(device); }
	const Dasdview& getDasdview(const string& device) { return dasdviews.get(device); }
	const CmdDmsetupInfo& getCmdDmsetupInfo() { return cmddmsetupinfo.get(); }
	const CmdCryptsetup& getCmdCryptsetup(const string& name) { return cmdcryptsetups.get(name); }
	const CmdDmraid& getCmdDmraid() { return cmddmraid.get(); }
	const CmdMultipath& getCmdMultipath() { return cmdmultipath.get(); }
	const CmdBtrfsShow& getCmdBtrfsShow() { return cmdbtrfsshow.get(); }
	const CmdVgs& getCmdVgs() { return cmdvgs.get(); }
	const CmdVgdisplay& getCmdVgdisplay(const string& name) { return vgdisplays.get(name); }
	const MajorMinor& getMajorMinor(const string& device) { return majorminors.get(device); }

    private:

	/* LazyObject and LazyObjects cache the object and a potential
	   exception during object construction. HelperBase does the common
	   part. */

	template <class Object, typename... Args>
	class HelperBase
	{
	public:

	    const Object& get(Args... args)
	    {
		if (e)
		    rethrow_exception(e);

		if (!object)
		{
		    try
		    {
			object.reset(new Object(args...));
		    }
		    catch (exception)
		    {
			e = current_exception();
			rethrow_exception(e);
		    }
		}

		return *object;
	    }

	private:

	    shared_ptr<Object> object;
	    exception_ptr e;

	};

	template <class Object>
	class LazyObject : public HelperBase<Object>, boost::noncopyable
	{
	};

	template <class Object, class Key = string>
	class LazyObjects : boost::noncopyable
	{
	public:

	    typedef HelperBase<Object, Key> Helper;

	    const Object& get(const Key& k)
	    {
		typename map<Key, Helper>::iterator pos = data.lower_bound(k);
		if (pos == data.end() || typename map<Key, Helper>::key_compare()(k, pos->first))
		    pos = data.insert(pos, typename map<Key, Helper>::value_type(k, Helper()));
		return pos->second.get(k);
	    }

	private:

	    map<Key, Helper> data;

	};

	LazyObjects<UdevMap> udevmaps;
	LazyObject<MdLinks> mdlinks;
	LazyObject<ProcParts> procparts;
	LazyObject<ProcMounts> procmounts;
	LazyObject<ProcMdstat> procmdstat;
	LazyObjects<MdadmDetails> mdadmdetails;
	LazyObjects<MdadmExamine, list<string>> mdadmexamines;
	LazyObject<Blkid> blkid;
	LazyObject<Lsscsi> lsscsi;
	LazyObjects<Parted> parteds;
	LazyObjects<Dasdview> dasdviews;
	LazyObject<CmdDmsetupInfo> cmddmsetupinfo;
	LazyObjects<CmdCryptsetup> cmdcryptsetups;
	LazyObject<CmdDmraid> cmddmraid;
	LazyObject<CmdMultipath> cmdmultipath;
	LazyObject<CmdBtrfsShow> cmdbtrfsshow;
	LazyObject<CmdVgs> cmdvgs;
	LazyObjects<CmdVgdisplay> vgdisplays;
	LazyObjects<MajorMinor> majorminors;

    };

}


#endif
