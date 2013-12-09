/****************************************************************************
    MailListStat - print useful statistics on email messages
    Stats header file
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

#ifndef	_MLS_STAT_H
#define	_MLS_STAT_H	1

/* ********************************************* FUNCTION DECLARATIONS *** */
void GetEmail(char *);
void RemoveCR(char *);
void GetSubj(char *);
void GetMailer(char *);
void AddStat(char *, char *, char *, char *, long, long);
void myCopy(char *, char *, size_t);
void PrintProgress();
void ParseInput();

#endif /* mls_stat.h */
