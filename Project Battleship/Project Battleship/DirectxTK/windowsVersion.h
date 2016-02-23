/***********************************************
* Filename:  		windowsVerstion.h
* Date:      		07/2/2015
* Mod. Date: 		07/25/2015
* Mod. Initials:	VG
* Author:    	    Volodymyr Golovan
* Purpose:   		This class was created to fix warnings 
*                   and have a place to define support for Windows 7 Machines
************************************************/

#pragma once
#ifdef WINVER
#undef WINVER
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define WINVER 0x0601
#define _WIN32_WINNT 0x0601