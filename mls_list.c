/****************************************************************************
    MailListStat - print useful statistics on email messages
    Functions for "linked list" (spajany zoznam)
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

#include "mls.h"
#include "mls_list.h"
#include "mls_stat.h"

/* ****************************************** Spaj. zoznam * FUNCTIONS *** */
/* ***** Alokuj ********************************************************** */
nTptr Alokuj() { // alocate memory for 1 member of bestTEXT
  return (nTptr)malloc(sizeof(struct bestTEXT));
}

/* ***** DeAlokuj ******************************************************** */
void DeAlokuj(nTptr first) { // deallocate entire list from memory
  nTptr akt,de;
  akt=first;
  while(akt!=NULL) {
    de=akt->next;
    free(akt);
    akt=de;
  }
}

/* ***** compCount ******************************************************* */
int compCount(nTptr a, nTptr b) {
  return (a->count > b->count);
}

/* ***** compSize ******************************************************** */
int compSize(nTptr a, nTptr b) {
  return (a->size > b->size);
}

/* ***** AddEntry (PridajZaznam) ***************************************** */
nTptr AddEntry(nTptr first, char *t, long size, long quote, int (*comp)()) { // change existing/add new member
  nTptr prev,akt,newone; // pointers to previous, actual, new entry

  // try to find him
  prev=NULL;
  akt=first;
  while (akt!=NULL) {
    if (!strncmp(akt->text,t,MIN_TEXT)) { // found
       (akt->count)++;
       (akt->size)+=size;
       (akt->quote)+=quote;
       // delete & add again (to maintain sort order)
       if (prev!=NULL) prev->next=akt->next; // delete
       if (akt==first)  first=akt->next;
       return InsertEntry(first,akt,comp); // add
    }
    prev=akt;
    akt=akt->next;
  }

  // not there yet --> add
  if ((newone=Alokuj())==NULL) myWarn("Not enough memory!","",4);
  myCopy(newone->text,t,MAX_TEXT);
  newone->size=size;
  newone->quote=quote;
  newone->count=1;
  newone->next=NULL;
  return InsertEntry(first,newone,comp);
}

/* ***** InsertEntry (VlozZaznam) **************************************** */
nTptr InsertEntry(nTptr first, nTptr newone, int (*comp)()) { // add member to list maintaining sort order
  nTptr akt,akt2;

  if (first==NULL) return newone; // it's the only one --> it's 1st
  if (comp(newone,first)) { // should be 1st
     newone->next=first;
     return newone;
  }
  akt=first;
  if (akt->next!=NULL) while (!(comp(newone,akt->next))) {
    akt=akt->next;
    if (akt->next==NULL) break;
  } // find such AKT, NEWone not less than AKT->next
  // NEWone should go after AKT
  akt2=akt->next;
  akt->next=newone;
  newone->next=akt2;
  return first;
}

/* ***** SortQuote ******************************************************* */
nTptr SortQuote(nTptr oldfirst) { // copy entries to new list - sorted by quote ratio
  // will use member 'count' as percent - 12.34% ==> 1234
  nTptr akt,akt2,newfirst; // actual in old, new

  newfirst=NULL;
  for (akt=oldfirst; akt!=NULL; akt=akt->next) { // for every member
     // copy it & change count
     if ((akt2=Alokuj())==NULL) myWarn("Not enough memory!","",4);
     myCopy(akt2->text,akt->text,MAX_TEXT);
     akt2->size =akt->size;
     akt2->quote=akt->quote;
     akt2->count=(int)((float)(akt->quote)/(akt->size)*10000);
     akt2->next=NULL;
     newfirst=InsertEntry(newfirst,akt2,compCount); // add to new list
  }
  return newfirst;
}

/* ***** SortSize ******************************************************** */
nTptr SortSize(nTptr oldfirst, int avg) { // copy entries to new list - sorted by size-quoted
  // if (avg) --> store average size instead of total size
  // will use member 'count' for storing size
  nTptr akt,akt2,newfirst; // actual in old, new

  newfirst=NULL;
  for (akt=oldfirst; akt!=NULL; akt=akt->next) { // for every member
     // copy it & change count
     if ((akt2=Alokuj())==NULL) myWarn("Not enough memory!","",4);
     myCopy(akt2->text,akt->text,MAX_TEXT);
     akt2->size =akt->size;
     akt2->quote=akt->quote;
     akt2->count=(akt->size - akt->quote) / ((avg) ? akt->count:1);
     akt2->next=NULL;
     newfirst=InsertEntry(newfirst,akt2,compCount); // add to new list
  }
  return newfirst;
}

