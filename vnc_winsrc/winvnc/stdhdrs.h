//  Copyright (C) 1999 AT&T Laboratories Cambridge. All Rights Reserved.
//
//  This file is part of the VNC system.
//
//  The VNC system is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// TightVNC distribution homepage on the Web: http://www.tightvnc.com/
//
// If the source code for the VNC system is not available from the place 
// whence you received this file, check http://www.uk.research.att.com/vnc or contact
// the authors on vnc@uk.research.att.com for information on obtaining it.

#pragma once
#define VC_EXTRALEAN
#define STRICT

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <crtdbg.h>
#include <omnithread.h>

extern const char* g_buildTime;

// LOGGING SUPPORT

#include "Log.h"
extern Log vnclog;

// No logging at all
#define LL_NONE		0
// Log server startup/shutdown
#define LL_STATE	0
// Log connect/disconnect
#define LL_CLIENTS	1
// Log connection errors (wrong pixfmt, etc)
#define LL_CONNERR	0
// Log socket errors
#define LL_SOCKERR	4
// Log internal errors
#define LL_INTERR	0

// Log internal warnings
#define LL_INTWARN	8
// Log internal info
#define LL_INTINFO	9
// Log socket errors
#define LL_SOCKINFO	10
// Log everything, including internal table setup, etc.
#define LL_ALL		10

enum vncMutexLevel // ordering is from innermost to outermost level
{
	eRegionLock,
	eClientsLock,
	eDesktopLock,
};

template<int level>
struct vncMutex : omni_mutex
{
public:
	vncMutex(): omni_mutex(1 << level) { }
};

#define omni_mutex_lock \
	struct _CRT_APPEND(omni_mutex_lock_, __LINE__) : omni_mutex_lock \
	{ \
		_CRT_APPEND(omni_mutex_lock_, __LINE__)(omni_mutex &m) \
		: omni_mutex_lock(m, vnclog.Validate(m, VNCLOG("Potential deadlock -> %s\n"))) { } \
	}

