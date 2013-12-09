/****************************************************************************
    MailListStat - print useful statistics on email messages
    "Linked list" header file
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

#ifndef	_MLS_LIST_H
#define	_MLS_LIST_H	1

/* ********************************************* FUNCTION DECLARATIONS *** */
nTptr Alokuj();
void DeAlokuj(nTptr);
nTptr AddEntry(nTptr, char *, long, long, int (*)());
nTptr InsertEntry(nTptr, nTptr, int (*)());
nTptr SortQuote(nTptr);
nTptr SortSize(nTptr, int);
int compCount(nTptr, nTptr);
int compSize(nTptr, nTptr);

#endif /* mls_list.h */
