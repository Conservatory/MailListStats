/****************************************************************************
    MailListStat - print useful statistics on email messages
    stats gathering & computing functions
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
#include "mls_stat.h"
#include "mls_mime.h"
#include "mls_list.h"

extern int verbose;
extern time_t t, t_oldest, t_newest;
extern long nSprav, nDlzka;
extern long nDen[31];
extern long nHod[24];
extern long nDOW[ 7];
extern long nMon[12];
extern nQptr msgQ;
extern nDptr msgD;
extern nTptr zozA, zozS, zozQ, zozM, zozAt, zozAa;
extern FILE *fInp;

/* ***** GetEmail ******************************************************** */
void GetEmail(char *auth) { // return only email address
  regmatch_t pm[MAX_REGMATCH];  // buffer for matching substrings
  char auth2[MAX_AUTH];
  RemoveCR(auth);
  if (regexec(&r_mail, auth, MAX_REGMATCH, pm, 0)) return; // should not happen
  myCopy(auth2, auth+pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so + 1);
  myCopy(auth, auth2, MAX_AUTH);
}

/* ***** RemoveCR ******************************************************** */
void RemoveCR(char *text) { // remove '\n' from end
  if (text[strlen(text)-2]=='\n') text[strlen(text)-2]='\0';
  if (text[strlen(text)-1]=='\n') text[strlen(text)-1]='\0';
}

/* ***** GetSubj ********************************************************* */
void GetSubj(char *subj) { // MIME-decode & remove "Re:" from begin of subj.
  regmatch_t pm[MAX_REGMATCH];  // buffer for matching substrings
  char sub2[MAX_SUBJ];
  RemoveCR(subj);
  Decode_mime_string(subj, MAX_SUBJ);
  if (regexec(&r_re, subj, MAX_REGMATCH, pm, 0)) return; // should not happen
  myCopy(sub2, subj+pm[3].rm_so, pm[3].rm_eo - pm[3].rm_so + 1);
  myCopy(subj, sub2, MAX_SUBJ);
}

/* ***** GetMailer ******************************************************* */
void GetMailer(char *mail) { // remove versions from X-Mailer/User-Agent/X-Newsreader line
  regmatch_t pm[MAX_REGMATCH];  // buffer for matching substrings
  char sub2[MAX_SUBJ];          // temp for storing result, will be copied back to mail at the end
  RemoveCR(mail);
  myCopy(sub2, mail, MAX_SUBJ); // copy original there for case no regex will match

  if (!regexec(&r_m_bat, mail, MAX_REGMATCH, pm, 0)) { // The Bat!
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_ims, mail, MAX_REGMATCH, pm, 0)) { // Internet Mail Service
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so]=' ';
    myCopy(sub2 + pm[1].rm_eo - pm[1].rm_so + 1, mail+pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so + pm[2].rm_eo - pm[2].rm_so+1]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so + pm[2].rm_eo - pm[2].rm_so+2]='\0';
  }
  if (!regexec(&r_m_moz, mail, MAX_REGMATCH, pm, 0)) { // Mozilla
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_ope, mail, MAX_REGMATCH, pm, 0)) { // Opera
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_oue, mail, MAX_REGMATCH, pm, 0)) { // Outlook Express
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_ouc, mail, MAX_REGMATCH, pm, 0)) { // Outlook CWS
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_oum, mail, MAX_REGMATCH, pm, 0)) { // Outlook IMO
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_out, mail, MAX_REGMATCH, pm, 0)) { // Outlook
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_lot, mail, MAX_REGMATCH, pm, 0)) { // Lotus Notes
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_cal, mail, MAX_REGMATCH, pm, 0)) { // Calypso
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_peg, mail, MAX_REGMATCH, pm, 0)) { // Pegasus4win
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_opw, mail, MAX_REGMATCH, pm, 0)) { // Open Webmail
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_eud, mail, MAX_REGMATCH, pm, 0)) { // Eudora
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_pos, mail, MAX_REGMATCH, pm, 0)) { // POSTIE
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_pob, mail, MAX_REGMATCH, pm, 0)) { // POBOX
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_kma, mail, MAX_REGMATCH, pm, 0)) { // KMail
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_mut, mail, MAX_REGMATCH, pm, 0)) { // Mutt
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_imp, mail, MAX_REGMATCH, pm, 0)) { // IMP webmail
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_syl, mail, MAX_REGMATCH, pm, 0)) { // Sylpheed
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_pin, mail, MAX_REGMATCH, pm, 0)) { // Pine
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
  }
  if (!regexec(&r_m_pi2, mail, MAX_REGMATCH, pm, 0)) { // Pine work-around
    // remove Message-ID in case no mailer was found there
    // Message-ID is used only if other headers are not present
    *sub2='\0';
  }

  // (these contributed by Urke MMI <urke@gmx.net>)
  if (!regexec(&r_m_pan, mail, MAX_REGMATCH, pm, 0)) { // Pan
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_4td, mail, MAX_REGMATCH, pm, 0)) { // 40tude_Dialog
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_fag, mail, MAX_REGMATCH, pm, 0)) { // Forte Agent
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_mpg, mail, MAX_REGMATCH, pm, 0)) { // MicroPlanet Gravity
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_xws, mail, MAX_REGMATCH, pm, 0)) { // Xnews
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_knd, mail, MAX_REGMATCH, pm, 0)) { // KNode
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_hst, mail, MAX_REGMATCH, pm, 0)) { // Hamster
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }
  if (!regexec(&r_m_nnr, mail, MAX_REGMATCH, pm, 0)) { // Noworyta News Reader
    myCopy(sub2, mail+pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so + 1);
    sub2[pm[1].rm_eo - pm[1].rm_so+0]='x';
    sub2[pm[1].rm_eo - pm[1].rm_so+1]='\0';
  }

  myCopy(mail, sub2, MAX_SUBJ);
}

/* ***** PrintProgress *************************************************** */
void PrintProgress() { // print no. of messages processed
  long delim=10;
  if (!verbose) return;
  while ((float)(nSprav/delim)>10) delim*=10;
  if (!(nSprav%delim)) fprintf(stderr,"(i) Processing message no.%5lu\n",nSprav);
}

/* ***** AddStat ********************************************************* */
void AddStat(char *A, char *S, char *D, char *M, long sQ, long sN) {
/* A ... Author (From: line)
   S ... Subject line
   D ... Date line
   M ... X-Mailer/User-Agent/X-Newsreader line
  sQ ... size of quoted text
  sN ... size of non-quoted text
*/
  regmatch_t pm[MAX_REGMATCH];  // buffer for matching substrings
  long sT; // total size of message body
  long day,mon,yea;  // day, month, year of curr. mess.
  time_t t_mess;     // date of current message (seconds)
  struct tm tm_mess; // date of curr. message
  char time[6];

  sT=sQ+sN; // sum of quoted + non-quoted
  // quote
  if ((float)sQ/sT > msgQ->perc) {
     myCopy(msgQ->auth, A, MAX_AUTH);
     myCopy(msgQ->subj, S, MAX_SUBJ);
     myCopy(msgQ->date, D, MAX_DATE);
     msgQ->size=sT;
     msgQ->perc=(float)sQ/sT;
  }
  // size
  if (sT > msgD->size) {
     myCopy(msgD->auth, A, MAX_AUTH);
     myCopy(msgD->subj, S, MAX_SUBJ);
     myCopy(msgD->date, D, MAX_DATE);
     msgD->size=sT;
  }
  zozA=AddEntry(zozA, A, sT, sQ, compCount); // author
  zozS=AddEntry(zozS, S, sT, sQ, compCount); // subject
  zozM=AddEntry(zozM, M, sT, sQ, compCount); // mailer
  // time
  if (!regexec(&r_time, D, MAX_REGMATCH, pm, 0)) { // if matched
     sT=pm[1].rm_eo - pm[1].rm_so + 1;
     myCopy(time, D + pm[1].rm_so, (sT>3)?3:sT); // it should not match more than 2 chars (+ \0)
     sT=atoi(time);
     if ((sT>=0) && (sT<24)) nHod[sT]++;
  }  else myVerb(" !  Invalid time: ",D);

  // day of week (DOW)
  if (!regexec(&r_dow, D, MAX_REGMATCH, pm, 0)) { // if matched
     sT=pm[2].rm_eo - pm[2].rm_so + 1;
     myCopy(time, D + pm[2].rm_so, (sT>4)?4:sT); // it should not match more than 3 chars (+ \0)
     sT=9; // default (invalid) value
     if (!strcmp(time, "Mon")) sT=0;
     if (!strcmp(time, "Tue")) sT=1;
     if (!strcmp(time, "Wed")) sT=2;
     if (!strcmp(time, "Thu")) sT=3;
     if (!strcmp(time, "Fri")) sT=4;
     if (!strcmp(time, "Sat")) sT=5;
     if (!strcmp(time, "Sun")) sT=6;
     if ((sT>=0) && (sT<7)) nDOW[sT]++;
  }  else myVerb(" !  Invalid day of week: ",D);

  // date
  day=mon=yea=0;
  if (!regexec(&r_date, D, MAX_REGMATCH, pm, 0)) { // if matched
     sT=pm[2].rm_eo - pm[2].rm_so + 1;
     if (sT > 0) {
        myCopy(time, D + pm[2].rm_so, (sT>3)?3:sT); // it should not match more than 2 chars (+ \0)
        sT=atoi(time)-1; // days in month begin from 1 (but our array from 0)
        if ((sT>=0) && (sT<31)) nDen[sT]++;
     }
     // oldest/newest message - match whole date, convert it to seconds
     day=sT+1;
     sT=pm[3].rm_eo - pm[3].rm_so + 1; // month in text form
     if (sT > 0) {
        myCopy(time, D + pm[3].rm_so, (sT>4)?4:sT); // it should not match more than 3 chars (+ \0)
        if (!strncmp(time,"Jan",3)) mon=1;
        if (!strncmp(time,"Feb",3)) mon=2;
        if (!strncmp(time,"Mar",3)) mon=3;
        if (!strncmp(time,"Apr",3)) mon=4;
        if (!strncmp(time,"May",3)) mon=5;
        if (!strncmp(time,"Jun",3)) mon=6;
        if (!strncmp(time,"Jul",3)) mon=7;
        if (!strncmp(time,"Aug",3)) mon=8;
        if (!strncmp(time,"Sep",3)) mon=9;
        if (!strncmp(time,"Oct",3)) mon=10;
        if (!strncmp(time,"Nov",3)) mon=11;
        if (!strncmp(time,"Dec",3)) mon=12;
     }
     sT=pm[4].rm_eo - pm[4].rm_so + 1; // year
     if (sT > 0) {
        myCopy(time, D + pm[4].rm_so, (sT>5)?5:sT); // it should not match more than 4 chars (+ \0)
        yea=atoi(time);
     }
     if (!day || !mon || !yea) myVerb(" !  Unable to get message date for oldest/newest: ",D);
     tm_mess.tm_sec=tm_mess.tm_min=0;
     tm_mess.tm_hour=12; // just to be sure that timezone won't change the day
     tm_mess.tm_isdst=-1; // unknown timezone
     tm_mess.tm_mday=day;
     tm_mess.tm_mon =mon-1;
     tm_mess.tm_year=yea-1900;
     t_mess=mktime(&tm_mess); // convert it to seconds since epoch
     if (mon) nMon[mon-1]++; else myVerb(" !  Invalid month: ",D);
     if (strncmp("DON'T DELETE THIS MESSAGE -- FOLDER INTERNAL DATA",S,49)) {
       if (!t_oldest) t_oldest=t_mess; // initialize oldest message to current
       if (t_mess < t_oldest) t_oldest=t_mess;
       if (t_mess > t_newest) t_newest=t_mess;
     }
  }  else myVerb(" !  Invalid date: ",D);

}

/* ***** myCopy ********************************************************** */
void myCopy(char *dst, char *src, size_t max) {
  // safe copy - will add \0 to end of destination region
  strncpy(dst,src,max);
  dst[max-1]='\0';
}

/* ***** ParseInput ****************************************************** */
void ParseInput() { // parse whole input file
  long i;
  char string[MAX_LINE];  // temp. for reading lines from input
  char riadok[MAX_LINE];  // 1st part of line (to be processed)
  int a_empt=1;      // finite state automat (empty,body,header)
  int a_head=0;
  int a_body=0;
  int nDlzRiad=0;    // size of current line
  int nDlzQuot=0;    // size of quoted lines of curr. email
  int nDlzNorm=0;    // size of non-quoted lines of curr. email
  char sAuth[MAX_AUTH]=""; // From: header line
  char sSubj[MAX_SUBJ]=""; // Subject: header line
  char sDate[MAX_DATE]=""; // Date: header line
  char sMail[MAX_SUBJ]=""; // X-Mailer/User-Agent/X-Newsreader/Message-ID header line

  while (!feof(fInp) && fgets(string,MAX_LINE,fInp)) {
    // read beginning of line
    nDlzRiad=i=strlen(string);
    strcpy(riadok,string); // both are < MAX_LINE chars, so it's safe
    while (!feof(fInp) && i>1 && string[i-1]!='\n') { // read up to the end of line
      fgets(string,MAX_LINE,fInp);
      i=strlen(string);
      nDlzRiad+=i;
    } // end of line
    // parse line & change automat state
    if (a_empt && a_head) { a_head=0; a_body=1; a_empt=0; }
    if (a_empt) { // begin of file || end of body
       // find "From "
       if (regexec(&r_from, riadok, 0, NULL, 0)==0) {
          if (a_body) AddStat(sAuth,sSubj,sDate,sMail,nDlzQuot,nDlzNorm);
          // we're at beginning of new message
	  a_head=1;
	  a_body=0;
	  nSprav++;
	  PrintProgress();
	  nDlzQuot=nDlzNorm=0;
	  *sAuth='\0'; *sSubj='\0'; *sDate='\0'; *sMail='\0';
       }
    }
    if (nDlzRiad==1) a_empt=1; else a_empt=0;

    // parse body
    if (a_body) nDlzka+=nDlzRiad;
    if (a_body) riadok[9]='\0'; // search only in first 9 chars
    if (a_body && strstr(riadok,">")) nDlzQuot+=nDlzRiad;
                     else if (a_body) nDlzNorm+=nDlzRiad;

    // parse header
    if (a_head) {
       if (!strncasecmp(riadok,"Subject: ",9)) {
          myCopy(sSubj,riadok+9,MAX_SUBJ);
	  GetSubj(sSubj);
       }
       if (!strncasecmp(riadok,"Date: ",6)) {
          myCopy(sDate,riadok+6,MAX_DATE);
	  RemoveCR(sDate);
       }
       if (!strncasecmp(riadok,"From: ",6)) {
          myCopy(sAuth,riadok+6,MAX_AUTH);
	  GetEmail(sAuth);
       }
       // will search for mailers in these
       // Message-ID will be used only if no other of these is present
       if (!strncasecmp(riadok,"Message-ID: ",12) && sMail=='\0') { // for Pine mailer
          myCopy(sMail,riadok+12,MAX_SUBJ);
	  GetMailer(sMail);
       }
       if (!strncasecmp(riadok,"X-Newsreader: ",14)) {
          myCopy(sMail,riadok+14,MAX_SUBJ);
	  GetMailer(sMail);
       }
       if (!strncasecmp(riadok,"User-Agent: ",12)) {
          myCopy(sMail,riadok+12,MAX_SUBJ);
	  GetMailer(sMail);
       }
       if (!strncasecmp(riadok,"X-Mailer: ",10)) {
          myCopy(sMail,riadok+10,MAX_SUBJ);
	  GetMailer(sMail);
       }
    }
  } /* while fEOF */
  time(&t);
  // compute stats of last email if needed
  if (a_body) AddStat(sAuth,sSubj,sDate,sMail,nDlzQuot,nDlzNorm);
}
