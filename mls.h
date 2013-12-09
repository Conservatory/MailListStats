/****************************************************************************
    MailListStat - print useful statistics on email messages
    main header file
    Copyright (C) 2001-2003  Marek Podmaka <marki@nexin.sk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ****************************************************************************/

#ifndef	_MLS_H
#define	_MLS_H	1

#define VERSION "1.3"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>

// maximum no. of substrings to match in regex
#define MAX_REGMATCH 7
// length of input buffer for reading mbox file
#define MAX_LINE 800
// maximum length of remembered subject
#define MAX_SUBJ 160
// maximum length of remembered author (email)
#define MAX_AUTH 160
// maximum length of remembered date
#define MAX_DATE 40
// TEXT will have min/max of AUTH and SUBJ
#if MAX_AUTH > MAX_SUBJ
#define MAX_TEXT MAX_AUTH
#define MIN_TEXT MAX_SUBJ
#else
#define MAX_TEXT MAX_SUBJ
#define MIN_TEXT MAX_AUTH
#endif

// for bit-mapped files variable (last 3 are for cache)
#define F_INPUT   2
#define F_OUTPUT  4
#define F_READ    8
#define F_WRITE  16
#define F_UPDATE 32

// for bit-mapped graph variable - what graphs to show
#define G_DAY    2
#define G_WEEK   4
#define G_MONTH  8
#define G_YEAR  16

// for myWarn - exit error codes
#define EXIT_NOERR  0
#define EXIT_CACHE  1
#define EXIT_PARAM  2
#define EXIT_FILEIO 3
#define EXIT_NOMEM  4
#define EXIT_REGEX  5

/* *** structures for BEST messages *** */
typedef struct bestQUOTE* nQptr;
struct bestQUOTE {
   char auth[MAX_AUTH];
   char subj[MAX_SUBJ];
   char date[MAX_DATE];
   long size;
   float perc;
};

typedef struct bestDLZKA* nDptr;
struct bestDLZKA {
   char auth[MAX_AUTH];
   char subj[MAX_SUBJ];
   char date[MAX_DATE];
   long size;
};

// structures w/ pointers to next in list - for best SUBJ & AUTH
typedef struct bestTEXT* nTptr;
struct bestTEXT {
   char text[MAX_TEXT];
   long size;
   long quote;
   long count;
   nTptr next;
};

// Reg. expressions
regex_t r_from, r_time, r_re, r_date, r_dow, r_mail,
        r_m_ims, r_m_bat, r_m_lot, r_m_oue, r_m_ouc, r_m_oum, r_m_out,
	r_m_cal, r_m_moz, r_m_peg, r_m_eud, r_m_ope, r_m_opw, r_m_pos,
	r_m_pob, r_m_kma, r_m_imp, r_m_mut, r_m_pin, r_m_pi2, r_m_syl,
	r_m_pan, r_m_4td, r_m_fag, r_m_mpg, r_m_xws, r_m_knd, r_m_hst,
	r_m_nnr;

/* ********************************************* FUNCTION DECLARATIONS *** */
void banner();
void help(char *);
void myWarn(char *, char *, int);
void myVerb(char *, char *);
void compile_mls_regex();
nTptr ReadZoz(FILE *);
void ReadCacheFile(char *, int);
void WriteCacheFile(char *);
void WriteZoz(FILE *, nTptr);


#endif /* mls.h */
