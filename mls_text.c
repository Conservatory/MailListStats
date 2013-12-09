/****************************************************************************
    MailListStat - print useful statistics on email messages
    Output of computed stats
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
#include "mls_lang.h"
#include "mls_stat.h"

// because we must specify it when 2dimensional array is passed to function
#define SNUMB_LEN	4

extern int lang;
extern int graph;
extern int onlytitle;
extern FILE *fOut;
extern time_t t, t_oldest, t_newest;
extern long nSprav, nDlzka;
extern long nDen[31];
extern long nHod[24];
extern long nDOW[ 7];
extern long nMon[12];
extern nQptr msgQ;
extern nDptr msgD;
extern nTptr zozA, zozS, zozQ, zozM, zozAt, zozAa;


/* ***** GetCount ******************************************************** */
long GetCount(nTptr first) { // return number of members in list
  nTptr akt;
  long count=0;

  for (akt=first; akt!=NULL; akt=akt->next) count++;
  return count;
}

/* ***** GetSum ********************************************************** */
long GetSum(long pole[], int max_index) { // return sum of elements in array
  long sum=0;
  int i;
  
  for (i=0; i < max_index; i++) sum+=pole[i];
  return sum;
}

/* ***** GetSumZoz ******************************************************* */
long GetSumZoz(nTptr first, int max_index) { // return sum of count elements of first max_index members of list
  long sum=0;
  int i;
  nTptr akt;
  
  for (akt=first, i=0; (i < max_index) && (akt!=NULL); i++, akt=akt->next) sum+=akt->count;
  return sum;
}

/* ***** GetSumZozQ ****************************************************** */
long GetSumZozQ(nTptr first) { // return sum of quote elements of all members of list
  long sum=0;
  nTptr akt;
  
  for (akt=first; akt!=NULL; akt=akt->next) sum+=akt->quote;
  return sum;
}

/* ***** PrintTop ******************************************************** */
void PrintTop(nTptr first, int top) { // print TOP table
  nTptr akt;
  int i;
  
  for (i=1, akt=first; (i <= top) && (akt!=NULL); i++, akt=akt->next) {
      if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
      fprintf(fOut,"|%3i | %-44.44s |%5li |%6.2f %% |\n",i,akt->text,akt->count,(float)akt->count/nSprav*100);
  }
}

/* ***** PrintTopHtml **************************************************** */
void PrintTopHtml(nTptr first, int top) { // print TOP table (HTML format)
  nTptr akt;
  int i;
  
  for (i=1, akt=first; (i <= top) && (akt!=NULL); i++, akt=akt->next) {
      if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
      fprintf(fOut," <TR class=\"%s\"><td align=\"right\">%i<td>%s<td align=\"right\">%li<td align=\"right\">%6.2f %%\n",(i%2)?"topXXtr1":"topXXtr2",i,akt->text,akt->count,(float)akt->count/nSprav*100);
  }
}

/* ***** PrintTopQ ******************************************************* */
void PrintTopQ(nTptr first, int top) { // print TOP quote table
  nTptr akt;
  int i;
  
  for (i=1, akt=first; (i <= top) && (akt!=NULL); i++, akt=akt->next) {
      if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
      fprintf(fOut,"|%3i | %-45.45s       |%6.2f %% |\n",i,akt->text,(float)akt->count/100);
  }
}

/* ***** PrintGraph ****************************************************** */
void PrintGraph(long pole[], int max_index, int hole) { // print 10line graph
  // will print MAX_INDEX bars which will be HOLE chars apart
  long max;
  char line[80]; // one line of output
  int i,j;
  float a;
  // initialization
  if (hole<1) myWarn("Invalid arguments passed to PrintGraph()!","(hole too small)",4);
  if (hole*max_index > 80) myWarn("Invalid arguments passed to PrintGraph()!","(max_index too big)",4);
  max=pole[0];
  memset(line, 'E', 79);
  line[79]=line[hole*max_index]='\0';
  for (i=0; i < max_index; i++) {
    if (pole[i] > max) max=pole[i]; // find max.
    for (j=1; j < hole; j++) line[i*hole+j]='-'; // spaces between columns
  }
  // graph + description of y-axis
  for (i=10; i > 0; i--) { // for each line
    for (j=0; j < max_index; j++) {
      a=(float)pole[j]/max*10;
      line[j*hole]=( (int)a>=i )?'#':'-';
    }
    fprintf(fOut,"\n%3i%% -%s", i*10, line);
    if (i==10) fprintf(fOut," - %li", max);
    if (i== 9) fprintf(fOut," %s", langMess[lang]);
  }
  // footer - description of x-axis
  for (i=0; i < max_index; i++) {
    line[i*hole]='*';
    for (j=1; j < hole; j++) line[i*hole+j]=' '; // space between columns
  }
  fprintf(fOut,"\n      %s\n", line);
}

/* ***** PrintGraphHtml ************************************************** */
void PrintGraphHtml(long data[], char desc[][SNUMB_LEN], int max_index, char *sHeader) { // print graph using HTML tables
  int i;
  long max;
  float a;
/*
  ing j;
  char ch[11]; // used for ALT in IMG tag
*/

  max=data[0];
  for (i=0; i < max_index; i++) if (data[i] > max) max=data[i]; // find max.
  fprintf(fOut,"<TABLE class=\"graph\">\n");
// GRAPH w/ HORIZONTAL BARS (and ALT tags)
/*
  fprintf(fOut," <TR valign=\"middle\"><td class=\"graph\" align=\"center\"><i>%s</i>", sHeader);
  fprintf(fOut,"<td class=\"graph\" align=\"left\">&nbsp;<i>%s</i>\n", langMess[lang]);
  for (i=0; i < max_index; i++) {
    a=(float)data[i]/max;
    for (j=0; j < a*10; j++) ch[j]='#'; // print # for every 10%
    ch[j]='\0';
    a*=550; // max. width of bar
    fprintf(fOut," <TR valign=\"middle\"><td class=\"graph\" align=\"center\">%s", desc[i]);
    fprintf(fOut,"<td class=\"graph\">&nbsp;<img class=\"graphH\" alt=\"%s\" src=\"bar.gif\" width=\"%u\">&nbsp;%lu\n", ch, (int)a, data[i]);
  }
*/
// GRAPH w/ VERTICAL BARS
  fprintf(fOut, "<TR valign=\"bottom\"><td class=\"graph\" align=\"center\">%s\n", langMess[lang]);
  for (i=0; i < max_index; i++) {
    a=(float)data[i]/max;
    a*=150; // max. height of bar
    fprintf(fOut,"<td class=\"graphW\" align=\"center\">%lu<br>", data[i]);
    fprintf(fOut,"<img class=\"graphW\" alt=\"|\" src=\"bar.gif\" height=\"%u\">\n", (int)a);
  }
  fprintf(fOut, "<TR valign=\"middle\"><td class=\"graph\" align=\"center\">%s\n", sHeader);
  for (i=0; i < max_index; i++) fprintf(fOut,"<td class=\"graphW\" align=\"center\">%s", desc[i]);

  fprintf(fOut,"</TABLE>\n");
}

/* ***** PrintStatText *************************************************** */
void PrintStatText(int topXX, char *title) { // print statistics in plain-text format
  long i;
  struct tm *tm1;
  char string[801];
  nTptr akt;

  if (nSprav > 0) {
   tm1=localtime(&t_oldest);
   sprintf(string,"\n %s %i.%i.%i", langTit1[lang], tm1->tm_mday, tm1->tm_mon+1, tm1->tm_year+1900);
   tm1=localtime(&t_newest);
   sprintf(string+strlen(string)," %s %i.%i.%i", langTit2[lang], tm1->tm_mday, tm1->tm_mon+1, tm1->tm_year+1900);
   if (title[0]!='\0') sprintf(string+strlen(string)," %s %s",langTit3[lang],title);
   if (onlytitle==1) sprintf(string, "%s", title); // only user-defined title
   fprintf(fOut,"%s\n",string);
   for (i=0; i<strlen(string); i++) fprintf(fOut,"~");

   // TOP AUTHORS by no. of msgs
   fprintf(fOut,"\n***** %s\n",langTOPpeople[lang]);
// fprintf(fOut,"+----+----%s----------------------------------+%s+-Percent-+\n",langGAuthor[lang],langGMess[lang]);
   fprintf(fOut,"+----+----%s----------------------------------+%s+%s+\n",langGAuthor[lang],langGMess[lang],langPercent[lang]);
   PrintTop(zozA, topXX);
   fprintf(fOut,"+----+----------------------------------------------+------+---------+\n");
   i=nSprav - GetSumZoz(zozA, topXX);
   fprintf(fOut,"|    | %-44.44s |%5li | %5.2f %% |\n",langOstI[lang],i,(float)i/nSprav*100);
   fprintf(fOut,"+----+----------------------------------------------+------+---------+\n");

   // TOP AUTHORS by total size of msgs
   fprintf(fOut,"\n***** %s\n",langTOPauthorSizeT[lang]);
   fprintf(fOut,"+----+----%s------------------------------------------+-KBytes-+\n",langGAuthor[lang]);
   for (i=1, akt=zozAt; (i <= topXX) && (akt!=NULL); i++, akt=akt->next) {
       if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
       fprintf(fOut,"|%3li | %-52.52s | %6.1f |\n",i,akt->text,(float)akt->count/1024);
   }
   fprintf(fOut,"+----+------------------------------------------------------+--------+\n");

   // TOP AUTHORS by average size of msgs
   fprintf(fOut,"\n***** %s\n",langTOPauthorSizeA[lang]);
   fprintf(fOut,"+----+----%s-------------------------------------------+-bytes-+\n",langGAuthor[lang]);
   for (i=1, akt=zozAa; (i <= topXX) && (akt!=NULL); i++, akt=akt->next) {
       if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
       fprintf(fOut,"|%3li | %-53.53s |%6lu |\n",i,akt->text,akt->count);
   }
   fprintf(fOut,"+----+-------------------------------------------------------+-------+\n");

   // TOP SUBJ
   fprintf(fOut,"\n***** %s\n",langTOPsubj[lang]);
// fprintf(fOut,"+----+----Subject-----------------------------------+%s+-Percent-+\n",langGMess[lang]);
   fprintf(fOut,"+----+---%s----------------------------------+%s+%s+\n",langSubject[lang],langGMess[lang],langPercent[lang]);
   PrintTop(zozS, topXX);
   fprintf(fOut,"+----+----------------------------------------------+------+---------+\n");
   i=nSprav - GetSumZoz(zozS, topXX);
   fprintf(fOut,"|    | %-44.44s |%5li | %5.2f %% |\n",langOstE[lang],i,(float)i/nSprav*100);
   fprintf(fOut,"+----+----------------------------------------------+------+---------+\n");

   // TOP MAILERS
   fprintf(fOut,"\n***** %s\n",langTOPmail[lang]);
// fprintf(fOut,"+----+----Mailer------------------------------------+%s+-Percent-+\n",langGMess[lang]);
   fprintf(fOut,"+----+---%s----------------------------------+%s+%s+\n",langMailer[lang],langGMess[lang],langPercent[lang]);
   PrintTop(zozM, topXX);
   fprintf(fOut,"+----+----------------------------------------------+------+---------+\n");
   i=nSprav - GetSumZoz(zozM, topXX);
   fprintf(fOut,"|    | %-44.44s |%5li | %5.2f %% |\n",langOstE[lang],i,(float)i/nSprav*100);
   fprintf(fOut,"+----+----------------------------------------------+------+---------+\n");

   // TOP QUOTING
   fprintf(fOut,"\n***** %s\n",langTOPquote[lang]);
// fprintf(fOut,"+----+----%s-----------------------------------------+-Percent-+\n",langGAuthor[lang]);
   fprintf(fOut,"+----+----%s-----------------------------------------+%s+\n",langGAuthor[lang],langPercent[lang]);
   for (i=1, akt=zozQ; (i <= topXX) && (akt!=NULL); i++, akt=akt->next) {
       if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
       fprintf(fOut,"|%3li | %-45.45s       |%6.2f %% |\n",i,akt->text,(float)akt->count/100);
   }
   fprintf(fOut,"+----+-----------------------------------------------------+---------+\n");
   fprintf(fOut,"|    | %-45.45s       | %5.2f %% |\n",langAvg[lang],(float)GetSumZozQ(zozQ)/nDlzka*100);
   fprintf(fOut,"+----+-----------------------------------------------------+---------+\n");

   // Hours of day
   if (graph & G_DAY) {
     fprintf(fOut,"\n***** %s\n",langGhour[lang]);
     PrintGraph(nHod,24,2);
     fprintf(fOut,"%s 0         5           11          17          23\n",langHour[lang]);
     if (GetSum(nHod, 24) != nSprav) fprintf(fOut,"!!!!! %s: %li %s.\n",langWarn[lang],nSprav - GetSum(nHod,24),langCount[lang]);
   }

   // Days of month
   if (graph & G_MONTH) {
     fprintf(fOut,"\n\n***** %s\n",langGday[lang]);
     PrintGraph(nDen,31,2);
     fprintf(fOut," %s 1         6           12          18          24            31\n",langDay[lang]);
     if (GetSum(nDen, 31) != nSprav) fprintf(fOut,"!!!!! %s: %li %s.\n",langWarn[lang],nSprav - GetSum(nDen,31),langCount[lang]);
   }

   // Days of week
   if (graph & G_WEEK) {
     fprintf(fOut,"\n\n***** %s\n",langGdow[lang]);
     PrintGraph(nDOW,7,4);
     fprintf(fOut,"     %s\n",langDOW[lang]);
     if (GetSum(nDOW, 7) != nSprav) fprintf(fOut,"!!!!! %s: %li %s.\n",langWarn[lang],nSprav - GetSum(nDOW,7),langCount[lang]);
   }

   // Months of year
   if (graph & G_YEAR) {
     fprintf(fOut,"\n\n***** %s\n",langGmon[lang]);
     PrintGraph(nMon,12,4);
     fprintf(fOut,"     %s\n",langMON[lang]);
     if (GetSum(nMon, 12) != nSprav) fprintf(fOut,"!!!!! %s: %li %s.\n",langWarn[lang],nSprav - GetSum(nMon,12),langCount[lang]);
   }

   fprintf(fOut,"\n\n***** %s\n",langMaxQuot[lang]);
   fprintf(fOut,"      %s %s\n",langAuth[lang], msgQ->auth);
   fprintf(fOut,"      %s %s\n",langSubj[lang], msgQ->subj);
   fprintf(fOut,"      %s %s\n",langDate[lang], msgQ->date);
   fprintf(fOut,"      %s %0.2f%% / %li %s\n",langRate[lang], msgQ->perc*100,msgQ->size, langByte[lang]);

   fprintf(fOut,"\n***** %s\n",langMaxLeng[lang]);
   fprintf(fOut,"      %s %s\n",langAuth[lang], msgD->auth);
   fprintf(fOut,"      %s %s\n",langSubj[lang], msgD->subj);
   fprintf(fOut,"      %s %s\n",langDate[lang], msgD->date);
   fprintf(fOut,"      %s %li %s\n",langSize[lang], msgD->size, langByte[lang]);

   fprintf(fOut,"\n***** %s\n",langMaxSubj[lang]);
   fprintf(fOut,"      %s %s\n" ,langSubj[lang], zozS->text);
   fprintf(fOut,"      %s %li\n",langNumb[lang], zozS->count);
   fprintf(fOut,"      %s %li %s\n",langSSiz[lang], zozS->size, langByte[lang]);

   fprintf(fOut,"\n***** %s\n",langEndStat[lang]);
   fprintf(fOut,"      %s %li\n",langTotMess[lang], nSprav);
   fprintf(fOut,"      %s %li\n",langTotAuth[lang], GetCount(zozA));
   fprintf(fOut,"      %s %li\n",langTotSubj[lang], GetCount(zozS));
   fprintf(fOut,"      %s %li %s\n",langTotLeng[lang], nDlzka, langByte[lang]);
   fprintf(fOut,"      %s %li %s\n",langAvgLeng[lang], nDlzka/nSprav, langByte[lang]);

   fprintf(fOut,"\n");
   fprintf(fOut,"***** Generated by MailListStat v%s, (C) 2001-2003 <marki (at) nexin.sk>\n",VERSION);
   fprintf(fOut,"***** See http://freshmeat.net/projects/mls for details...\n");
 } else fprintf(fOut,"\nNo messages found. Therefore no stats can be computed!\n");
}

/* ***** PrintStatHtml *************************************************** */
void PrintStatHtml(int topXX, char *title, short mode) { // print statistics in HTML format
  long i;
  struct tm *tm1;
  nTptr akt;
  char sNumb[32][SNUMB_LEN];

  for (i=0; i <= 31; i++) sprintf(sNumb[i], "%lu", i); // for description

  if (mode!=2) { // don't print HTML header/footer in html2 mode
    fprintf(fOut,"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
    fprintf(fOut,"<HTML><HEAD>\n");
    fprintf(fOut,"  <META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=%s\">\n",langCharset[lang]);
    fprintf(fOut,"  <META NAME=\"GENERATOR\" CONTENT=\"MailListStat by (C) Marki, 2001-2003\">\n");
    fprintf(fOut,"  <META NAME=\"AUTHOR\" CONTENT=\"(marki (at) nexin.sk)  (http://freshmeat.net/projects/mls)\">\n");
    fprintf(fOut,"  <LINK href=\"style_mls.css\" rel=\"stylesheet\" type=\"text/css\">\n");
    fprintf(fOut,"  <LINK rel=\"Help\"   href=\"http://freshmeat.net/projects/mls\" title=\"\">\n");
    fprintf(fOut,"  <LINK rel=\"Author\" href=\"http://freshmeat.net/projects/mls\" title=\"\">\n");
    fprintf(fOut,"  <TITLE>MailListStat - Statistics");
    if (title[0]!='\0') fprintf(fOut," (%s)",title);
    fprintf(fOut,"</TITLE>\n");
    fprintf(fOut,"</HEAD>\n");
    fprintf(fOut,"<BODY>\n");
  }

  if (nSprav > 0) {
   fprintf(fOut,"\n<h1 id=\"start\">");
   if (onlytitle==1) fprintf(fOut, "%s", title); // only user-defined title
   else { // normal title
     tm1=localtime(&t_oldest);
     fprintf(fOut,"%s %i.%i.%i ", langTit1[lang], tm1->tm_mday, tm1->tm_mon+1, tm1->tm_year+1900);
     tm1=localtime(&t_newest);
     fprintf(fOut,"%s %i.%i.%i" , langTit2[lang], tm1->tm_mday, tm1->tm_mon+1, tm1->tm_year+1900);
     if (title[0]!='\0') fprintf(fOut,"<br>%s %s",langTit3[lang],title);
   }
   fprintf(fOut,"</h1>\n");
   fprintf(fOut,"<!-- === -->\n"); // marker for PHP wrapper

   // TOP AUTHORS by no. of msgs
   fprintf(fOut,"\n<h2 id=\"zozA\">%s</h2>\n",langTOPpeople[lang]);
   fprintf(fOut,"<TABLE class=\"topXX\">\n");
   fprintf(fOut," <TR class=\"topXXhead\">");
// fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;Percent&nbsp;\n",langGAuthorHtml[lang],langGMessHtml[lang]);
   fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;\n",langGAuthorHtml[lang],langGMessHtml[lang],langPercentHtml[lang]);
   PrintTopHtml(zozA, topXX);
   i=nSprav - GetSumZoz(zozA, topXX);
   fprintf(fOut," <TR class=\"topXXtrO\"><td>&nbsp;<td>%s<td align=\"right\">%li<td align=\"right\">%5.2f %%\n",langOstI[lang],i,(float)i/nSprav*100);
   fprintf(fOut,"</TABLE>\n");

   // TOP AUTHORS by total size of msgs
   fprintf(fOut,"\n<h2 id=\"zozAt\">%s</h2>\n",langTOPauthorSizeT[lang]);
   fprintf(fOut,"<TABLE class=\"topXX\">\n");
   fprintf(fOut," <TR class=\"topXXhead\">");
   fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;KBytes&nbsp;\n",langGAuthorHtml[lang]);
   for (i=1, akt=zozAt; (i <= topXX) && (akt!=NULL); i++, akt=akt->next) {
       if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
       fprintf(fOut," <TR class=\"%s\"><td align=\"right\">%li<td>%s<td align=\"right\">%6.1f\n",(i%2)?"topXXtr1":"topXXtr2",i,akt->text,(float)akt->count/1024);
   }
   fprintf(fOut,"</TABLE>\n");

   // TOP AUTHORS by average size of msgs
   fprintf(fOut,"\n<h2 id=\"zozAa\">%s</h2>\n",langTOPauthorSizeA[lang]);
   fprintf(fOut,"<TABLE class=\"topXX\">\n");
   fprintf(fOut," <TR class=\"topXXhead\">");
   fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;bytes&nbsp;\n",langGAuthorHtml[lang]);
   for (i=1, akt=zozAa; (i <= topXX) && (akt!=NULL); i++, akt=akt->next) {
       if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
       fprintf(fOut," <TR class=\"%s\"><td align=\"right\">%li<td>%s<td align=\"right\">%lu\n",(i%2)?"topXXtr1":"topXXtr2",i,akt->text,akt->count);
   }
   fprintf(fOut,"</TABLE>\n");

   // TOP SUBJ
   fprintf(fOut,"\n<h2 id=\"zozS\">%s</h2>\n",langTOPsubj[lang]);
   fprintf(fOut,"<TABLE class=\"topXX\">\n");
   fprintf(fOut," <TR class=\"topXXhead\">");
// fprintf(fOut,"<td>&nbsp;<td>&nbsp;Subject&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;Percent&nbsp;\n",langGMessHtml[lang]);
   fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;\n",langSubjectHtml[lang],langGMessHtml[lang],langPercentHtml[lang]);
   PrintTopHtml(zozS, topXX);
   i=nSprav - GetSumZoz(zozS, topXX);
   fprintf(fOut," <TR class=\"topXXtrO\"><td>&nbsp;<td>%s<td align=\"right\">%li<td align=\"right\">%5.2f %%\n",langOstE[lang],i,(float)i/nSprav*100);
   fprintf(fOut,"</TABLE>\n");

   // TOP MAILERS
   fprintf(fOut,"\n<h2 id=\"zozM\">%s</h2>\n",langTOPmail[lang]);
   fprintf(fOut,"<TABLE class=\"topXX\">\n");
   fprintf(fOut," <TR class=\"topXXhead\">");
// fprintf(fOut,"<td>&nbsp;<td>&nbsp;Mailer&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;Percent&nbsp;\n",langGMessHtml[lang]);
   fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;\n",langMailerHtml[lang],langGMessHtml[lang],langPercentHtml[lang]);
   PrintTopHtml(zozM, topXX);
   i=nSprav - GetSumZoz(zozM, topXX);
   fprintf(fOut," <TR class=\"topXXtrO\"><td>&nbsp;<td>%s<td align=\"right\">%li<td align=\"right\">%5.2f %%\n",langOstE[lang],i,(float)i/nSprav*100);
   fprintf(fOut,"</TABLE>\n");

   // TOP QUOTING
   fprintf(fOut,"\n<h2 id=\"zozQ\">%s</h2>\n",langTOPquote[lang]);
   fprintf(fOut,"<TABLE class=\"topXX\">\n");
   fprintf(fOut," <TR class=\"topXXhead\">");
// fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;Percent&nbsp;\n",langGAuthorHtml[lang]);
   fprintf(fOut,"<td>&nbsp;<td>&nbsp;%s&nbsp;<td>&nbsp;%s&nbsp;\n",langGAuthorHtml[lang],langPercentHtml[lang]);
   for (i=1, akt=zozQ; (i <= topXX) && (akt!=NULL); i++, akt=akt->next) {
       if (akt->text[0]=='\0') myCopy(akt->text, langUnknown[lang], MAX_TEXT);
       fprintf(fOut," <TR class=\"%s\"><td align=\"right\">%li<td>%s<td align=\"right\">%6.2f %%\n",(i%2)?"topXXtr1":"topXXtr2",i,akt->text,(float)akt->count/100);
   }
   fprintf(fOut," <TR class=\"topXXtrO\"><td>&nbsp;<td>%s<td align=\"right\">%6.2f %%\n",langAvg[lang],(float)GetSumZozQ(zozQ)/nDlzka*100);
   fprintf(fOut,"</TABLE>\n");

   // Hours of day
   if (graph & G_DAY) {
     fprintf(fOut,"\n<h2 id=\"Ghour\">%s</h2>\n",langGhour[lang]);
     PrintGraphHtml(nHod, sNumb, 24, langHour[lang]);
     if (GetSum(nHod, 24) != nSprav) fprintf(fOut,"<br><b>%s:</b> <i>%li %s</i>.<br>\n",langWarn[lang],nSprav - GetSum(nHod,24),langCount[lang]);
   }

   // Days of month
   if (graph & G_MONTH) {
     fprintf(fOut,"\n<h2 id=\"Gmonth\">%s</h2>\n",langGday[lang]);
     PrintGraphHtml(nDen, sNumb+1, 31, langDay[lang]);
     if (GetSum(nDen, 31) != nSprav) fprintf(fOut,"<br><b>%s:</b> <i>%li %s</i>.<br>\n",langWarn[lang],nSprav - GetSum(nDen,31),langCount[lang]);
   }

   // Days of week
   if (graph & G_WEEK) {
     for (i=0; i <= 7; i++) { // fill the sNumb array with week-day names from langDOW[lang]
        strncpy(sNumb[i], langDOW[lang]+i*4, 3);
        sNumb[i][3]='\0';
     }
     fprintf(fOut,"\n<h2 id=\"Gweek\">%s</h2>\n",langGdow[lang]);
     PrintGraphHtml(nDOW, sNumb, 7, "");
     if (GetSum(nDOW,  7) != nSprav) fprintf(fOut,"<br><b>%s:</b> <i>%li %s</i>.<br>\n",langWarn[lang],nSprav - GetSum(nDOW, 7),langCount[lang]);
   }

   // Months of year
   if (graph & G_YEAR) {
     for (i=0; i <=12; i++) { // fill the sNumb array with month-names from langMON[lang]
        strncpy(sNumb[i], langMON[lang]+i*4, 3);
        sNumb[i][3]='\0';
     }
     fprintf(fOut,"\n<h2 id=\"Gmonth\">%s</h2>\n",langGmon[lang]);
     PrintGraphHtml(nMon, sNumb,12, "");
     if (GetSum(nMon, 12) != nSprav) fprintf(fOut,"<br><b>%s:</b> <i>%li %s</i>.<br>\n",langWarn[lang],nSprav - GetSum(nMon,12),langCount[lang]);
   }

   fprintf(fOut,"\n<h2 id=\"endstat\">%s</h2>\n",langMaxQuot[lang]);
   fprintf(fOut,"<TABLE class=\"endstat\">\n");
   fprintf(fOut," <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %s \n",langAuth[lang], msgQ->auth);
   fprintf(fOut," <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %s \n",langSubj[lang], msgQ->subj);
   fprintf(fOut," <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %s \n",langDate[lang], msgQ->date);
   fprintf(fOut," <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %0.2f%% / %li %s\n",langRate[lang], msgQ->perc*100,msgQ->size, langByte[lang]);
   fprintf(fOut,"</TABLE>\n");

   fprintf(fOut,"\n<h2>%s</h2>\n",langMaxLeng[lang]);
   fprintf(fOut,"<TABLE class=\"endstat\">\n");
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %s\n",langAuth[lang], msgD->auth);
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %s\n",langSubj[lang], msgD->subj);
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %s\n",langDate[lang], msgD->date);
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li %s\n",langSize[lang], msgD->size, langByte[lang]);
   fprintf(fOut,"</TABLE>\n");

   fprintf(fOut,"\n<h2>%s</h2>\n",langMaxSubj[lang]);
   fprintf(fOut,"<TABLE class=\"endstat\">\n");
   fprintf(fOut," <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %s\n" ,langSubj[lang], zozS->text);
   fprintf(fOut," <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li\n",langNumb[lang], zozS->count);
   fprintf(fOut," <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li %s\n",langSSiz[lang], zozS->size, langByte[lang]);
   fprintf(fOut,"</TABLE>\n");

   fprintf(fOut,"\n<h2>%s</h2>\n",langEndStat[lang]);
   fprintf(fOut,"<TABLE class=\"endstat\">\n");
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li\n",langTotMess[lang], nSprav);
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li\n",langTotAuth[lang], GetCount(zozA));
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li\n",langTotSubj[lang], GetCount(zozS));
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li %s\n",langTotLeng[lang], nDlzka, langByte[lang]);
   fprintf(fOut,"  <TR class=\"endstat\"><td class=\"endtit\"> <i>%s</i> <td class=\"endval\"> %li %s\n",langAvgLeng[lang], nDlzka/nSprav, langByte[lang]);
   fprintf(fOut,"</TABLE>\n");

   fprintf(fOut,"\n<HR>\n");
   fprintf(fOut,"<TABLE cellSpacing=0 cellPadding=0 width=\"100%%\"><TR valign=\"top\">\n");
   fprintf(fOut,"<td align=\"left\">Input file last updated: %s", ctime(&t));
   fprintf(fOut,"<td align=\"right\">Generated by <a href=\"http://freshmeat.net/projects/mls\"><b>MailListStat</b></a> v%s\n",VERSION);
   fprintf(fOut,"</TABLE>\n");

  } else fprintf(fOut,"\n<P><b>No messages found. Therefore no stats can be computed!</b></P>\n");
  if (mode!=2) fprintf(fOut,"</BODY></HTML>\n"); // don't print HTML header/footer in html2 mode
}
