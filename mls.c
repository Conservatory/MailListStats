/****************************************************************************
    MailListStat - print useful statistics on email messages
    main program file
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
#include "mls_text.h"
#include "mls_mime.h"
#include "mls_list.h"
#include "mls_stat.h"

/* ************************************************** GLOBAL VARIABLES *** */
FILE *fInp,*fOut;  // In/Out file handler
long nSprav=0;     // total number of messages
long nDlzka=0;     // total size of msgs (only body parts are counted)
int lang=99;       // language to use on output (99=not def.)
int verbose=0;     // verbose mode
int quiet=0;       // quiet mode
int onlytitle=0;   // show only user-defined text as title? (w/o oldest/newest date)
int graph=G_DAY | G_WEEK | G_MONTH; // bit-mapped: graphs to show

// variables used for statistics
nQptr msgQ=NULL;   // mess. with max. quoting
nDptr msgD=NULL;   // mess. with max. size
nTptr zozA=NULL;   // initial segment of AUTH list
nTptr zozS=NULL;   // initial segment of SUBJ list
nTptr zozQ=NULL;   // initial segment of QUOTE list
nTptr zozM=NULL;   // initial segment of MAILER list
nTptr zozAt=NULL;  // initial segment of AUTH (total-size) list
nTptr zozAa=NULL;  // initial segment of AUTH (average-size) list
long nDen[31];     // no. of msgs written during days of month
long nHod[24];     // no. of msgs written during hours of day
long nDOW[ 7];     // no. of msgs written during days of week
long nMon[12];     // no. of msgs written during months of year
time_t t;          // actual time - to be added to output
time_t t_oldest,
       t_newest;   // date of oldest/newest message found

/* ************************************************* Other * FUNCTIONS *** */
/* ***** banner ********************************************************** */
void banner() { // print greeting banner
  fprintf(stderr,"MailListStat v%s, (C) 2001-2003 by Marek -Marki- Podmaka <marki@nexin.sk>\n",VERSION);
}
/* ***** help ************************************************************ */
void help(char *commandline) { // print help text
  banner();
  fprintf(stderr,"Program for generating useful statistics on email messages in MBOX files\n");
//fprintf(stderr,"%s [-hvq] [-l lang] [-i file] [-o file] [-r|w|u file] [-t|T text] [-n XX] [-m mode]\n",commandline);
  fprintf(stderr,"  -h      ... print this help text and exit\n");
  fprintf(stderr,"  -v      ... verbose mode - print some \"useful\" info\n");
  fprintf(stderr,"  -q      ... be quiet (print only errors to stderr)\n");
  fprintf(stderr,"  -l lang ... select output language (EN,SK,IT,FR,DE,ES,SR,BR)*\n");
  fprintf(stderr,"  -i file ... name of input file (if not specified, stdin)\n");
  fprintf(stderr,"  -o file ... name of output file (if not specified, stdout)\n");
  fprintf(stderr,"  -r file ... read input from cache file instead of mailbox\n");
  fprintf(stderr,"  -w file ... write cache file (no stats produced)\n");
  fprintf(stderr,"  -u file ... update cache file (read cache, read input, write cache)\n");
  fprintf(stderr,"  -t text ... name of mailing list (see README)*\n");
  fprintf(stderr,"  -T text ... title text (only this will be printed as title)*\n");
  fprintf(stderr,"  -g xxxx ... graphs to show (Day, Week, Month, Year), use 1st letters*\n");
  fprintf(stderr,"  -m mode ... mode of output (text, html, html2)*\n");
  fprintf(stderr,"  -n XX   ... show TOP XX (default TOP 10)*\n");
  fprintf(stderr,"Options marked * are ignored when writing cache file (-w or -u option).\n");
  fprintf(stderr,"\n");
}

/* ***** myWarn ********************************************************** */
void myWarn(char *t1, char *t2, int err) { // print text on stderr, exit if err>0
  if (err==2) banner(); // errors about command-line options
  if (!quiet || err) fprintf(stderr,"%s %s\n",t1,t2); // if quiet, print only errors (err>0)
  if (err) exit(err);
}

/* ***** myVerb ********************************************************** */
void myVerb(char *t1, char *t2) { // if verbose, print text on stderr
  if (verbose) fprintf(stderr,"%s %s\n",t1,t2);
}

/* ***** ReadZoz ********************************************************* */
nTptr ReadZoz(FILE *fWhere) {
  nTptr akt=NULL, akt2=NULL, first=NULL;
  char string[MAX_LINE];
  long i,cnt;

  fgets(string, MAX_LINE, fWhere); if (sscanf(string, "%lu", &cnt) != 1) myWarn("Invalid data in cache file!","(zoz/cnt)",1);
  for (i=1; i<=cnt; i++) {
     akt2=akt;
     if ((akt=Alokuj())==NULL) myWarn("Not enough memory!","",4);
     if (akt2 !=NULL) akt2->next=akt;
     if (first==NULL) first=akt;
     fgets(string, MAX_LINE, fWhere); myCopy(akt->text, string, MAX_TEXT); RemoveCR(akt->text);
     fgets(string, MAX_LINE, fWhere); if (sscanf(string, "%lu", &akt->size ) != 1) myWarn("Invalid data in cache file!","(zoz->size)",1);
     fgets(string, MAX_LINE, fWhere); if (sscanf(string, "%lu", &akt->quote) != 1) myWarn("Invalid data in cache file!","(zoz->quote)",1);
     fgets(string, MAX_LINE, fWhere); if (sscanf(string, "%lu", &akt->count) != 1) myWarn("Invalid data in cache file!","(zoz->count)",1);
  }
  return first;
}

/* ***** ReadCacheFile *************************************************** */
void ReadCacheFile(char *filename, int update) {
  // update=1 when updating cache file (-u option) ... files & F_UPDATE in main()
  FILE *fCache;
  char string[MAX_LINE];
  int i;
  
  if (!(fCache=fopen(filename,"rb"))) {
    // updated cache file don't have to exist, so when updating print just warning, not error
    myWarn("Cannot open cache file for reading!",strerror(errno), update ? 0:3);
    if (update) return;
  }
  fgets(string, MAX_LINE, fCache);
  if (strncmp(string, "1.3\n", 4)) myWarn("Cache file from older version of MailListStat!","Please re-create it with version >=1.3",1);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &t       ) != 1) myWarn("Invalid data in cache file!","(time)",1);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &t_oldest) != 1) myWarn("Invalid data in cache file!","(t_oldest)",1);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &t_newest) != 1) myWarn("Invalid data in cache file!","(t_newest)",1);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &nSprav  ) != 1) myWarn("Invalid data in cache file!","(nSprav)",1);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &nDlzka  ) != 1) myWarn("Invalid data in cache file!","(nDlzka)",1);
  for (i=0; i < 31; i++) { fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &nDen[i]) != 1) myWarn("Invalid data in cache file!","(nDen[])",1); }
  for (i=0; i < 24; i++) { fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &nHod[i]) != 1) myWarn("Invalid data in cache file!","(nHod[])",1); }
  for (i=0; i <  7; i++) { fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &nDOW[i]) != 1) myWarn("Invalid data in cache file!","(nDOW[])",1); }
  for (i=0; i < 12; i++) { fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &nMon[i]) != 1) myWarn("Invalid data in cache file!","(nMon[])",1); }
  // msgQ
  fgets(string, MAX_LINE, fCache); myCopy(msgQ->auth, string, MAX_AUTH); RemoveCR(msgQ->auth);
  fgets(string, MAX_LINE, fCache); myCopy(msgQ->subj, string, MAX_SUBJ); RemoveCR(msgQ->subj);
  fgets(string, MAX_LINE, fCache); myCopy(msgQ->date, string, MAX_DATE); RemoveCR(msgQ->date);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &msgQ->size) != 1) myWarn("Invalid data in cache file!","(msgQ->size)",1);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%f" , &msgQ->perc) != 1) myWarn("Invalid data in cache file!","(msgQ->perc)",1);
  // msgD
  fgets(string, MAX_LINE, fCache); myCopy(msgD->auth, string, MAX_AUTH); RemoveCR(msgD->auth);
  fgets(string, MAX_LINE, fCache); myCopy(msgD->subj, string, MAX_SUBJ); RemoveCR(msgD->subj);
  fgets(string, MAX_LINE, fCache); myCopy(msgD->date, string, MAX_DATE); RemoveCR(msgD->date);
  fgets(string, MAX_LINE, fCache); if (sscanf(string, "%lu", &msgD->size) != 1) myWarn("Invalid data in cache file!","(msgD->size)",1);
  zozA =ReadZoz(fCache);
  zozS =ReadZoz(fCache);
  zozM =ReadZoz(fCache);
  zozQ =ReadZoz(fCache);
  zozAt=ReadZoz(fCache);
  zozAa=ReadZoz(fCache);
}

/* ***** WriteCacheFile ************************************************** */
void WriteCacheFile(char *filename) {
  FILE *fCache;
  int i;
  
  if (!(fCache=fopen(filename,"wb"))) myWarn("Cannot open cache file for output!",strerror(errno),3);
  fprintf(fCache, "1.3\n");
  fprintf(fCache, "%lu\n", t);
  fprintf(fCache, "%lu\n", t_oldest);
  fprintf(fCache, "%lu\n", t_newest);
  fprintf(fCache, "%lu\n", nSprav);
  fprintf(fCache, "%lu\n", nDlzka);
  for (i=0; i < 31; i++) fprintf(fCache, "%lu\n", nDen[i]);
  for (i=0; i < 24; i++) fprintf(fCache, "%lu\n", nHod[i]);
  for (i=0; i <  7; i++) fprintf(fCache, "%lu\n", nDOW[i]);
  for (i=0; i < 12; i++) fprintf(fCache, "%lu\n", nMon[i]);
  fprintf(fCache, "%s\n%s\n%s\n%lu\n", msgQ->auth, msgQ->subj, msgQ->date, msgQ->size);
  fprintf(fCache, "%f\n", msgQ->perc);
  fprintf(fCache, "%s\n%s\n%s\n%lu\n", msgD->auth, msgD->subj, msgD->date, msgD->size);
  WriteZoz(fCache, zozA);
  WriteZoz(fCache, zozS);
  WriteZoz(fCache, zozM);
  WriteZoz(fCache, zozQ);
  WriteZoz(fCache, zozAt);
  WriteZoz(fCache, zozAa);
  fclose(fCache);
}

/* ***** WriteZoz ******************************************************** */
void WriteZoz(FILE *fWhere, nTptr first) {
  nTptr akt;

  fprintf(fWhere, "%lu\n", GetCount(first));
  for (akt=first; akt!=NULL; akt=akt->next) {
     fprintf(fWhere, "%s\n", akt->text);
     fprintf(fWhere, "%lu\n", akt->size);
     fprintf(fWhere, "%lu\n", akt->quote);
     fprintf(fWhere, "%lu\n", akt->count);
  }
}

/* ***** compile_mls_regex *********************************************** */
void compile_mls_regex() {
 if (regcomp(&r_from, "^From .* (Mon|Tue|Wed|Thu|Fri|Sat|Sun)", REG_EXTENDED | REG_NOSUB) > 0) myWarn("Error compiling regex!","(from)",5);
 if (regcomp(&r_time, ".* ([0-9]{1,2}):.*", REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(time)",5);
 if (regcomp(&r_re  , " {0,}(re(\\[[0-9]*\\])?: {0,}){0,5}(.*)", REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(re)",5);
 if (regcomp(&r_date, "(^|.* )([0-9]{1,2}) {1,}(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec) {1,}([0-9]{4})", REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(date)",5);
 if (regcomp(&r_dow , "(.*[^a-z]|^)(Mon|Tue|Wed|Thu|Fri|Sat|Sun)[^a-z].*", REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(dow)",5);
 if (regcomp(&r_mail, "(.*[ <]){0,1}([0-9a-z!#$%^&()_+.,~-]*@[0-9a-z!#$%^&()_+.,~-]*)([ >].*){0,1}", REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(email)",5);
 // regex for X-Mailer/User-Agent/X-Newsreader/Message-ID
 if (regcomp(&r_m_ims, "^ {0,}(Internet Mail Service) \\(([0-9]\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_bat, "^ {0,}(The Bat!)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_lot, "^ {0,}(Lotus Notes Release [0-9]\\.[0-9])", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_oue, "^ {0,}(Microsoft Outlook Express [0-9]\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_ouc, "^ {0,}(Microsoft Outlook CWS)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_oum, "^ {0,}(Microsoft Outlook IMO)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_out, "^ {0,}(Microsoft Outlook), ", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_cal, "^ {0,}(Calypso Version [0-9])", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_moz, "^ {0,}(Mozilla( |/)[0-9]\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_peg, "^ {0,}(Pegasus Mail for Win)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_opw, "^ {0,}(Open WebMail)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_eud, "^ {0,}(QUALCOMM Windows Eudora)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_pos, "^ {0,}(POSTIE)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_pob, "^ {0,}(POBOX)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_kma, "^ {0,}(KMail)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_mut, "^ {0,}(Mutt)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_imp, "^ {0,}(IMP/PHP IMAP webmail)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_ope, "^ {0,}(Opera [0-9]\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_pin, "^ {0,}<(Pine)\\.", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_pi2, "^ {0,}<[^P][^<>]*@[^<>]+>$", REG_EXTENDED | REG_NOSUB) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 // r_m_pi2 - match normal Message-ID header (to be removed and not threated as mailer)
 if (regcomp(&r_m_syl, "^ {0,}(Sylpheed version [0-9]+\\.[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);

 // (these contributed by Urke MMI <urke@gmx.net>)
 if (regcomp(&r_m_pan, "^ {0,}(Pan( |/)[0-9]+\\.[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_4td, "^ {0,}(40tude_Dialog( |/)[0-9]+\\.[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_fag, "^ {0,}(Forte Agent [0-9]+\\.[0-9]?)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_mpg, "^ {0,}(MicroPlanet Gravity v[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_xws, "^ {0,}(Xnews.?[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_knd, "^ {0,}(KNode( |/)[0-9]+\\.[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 // Only Hamster local server w/o client identifier
 if (regcomp(&r_m_hst, "^ {0,}(Hamster/[0-9]+\\.[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);
 if (regcomp(&r_m_nnr, "^ {0,}(Noworyta News Reader/[0-9]+\\.)", REG_EXTENDED) > 0) myWarn("Error compiling regex!","(x-mailer)",5);

 compile_mime_regex();
}
/* ************************************************************** MAIN *** */
int main(int argc, char **argv) {
  long i,c;
  char *ch;          // just pointer
  short int files=0; // bit map: F_* defines in mls.h
  short int mode=0;  // mode of stats output: 0=text; 1=html; 2=html2
  int topXX=10;      // TOP 10 is default
  char title[40]="";
  char sCache[40]=""; // name of cache file
  char sInput[40]="";
  char sOutput[40]="";
  struct timeval t_beg,t_end; // for execution time
#ifndef __DJGPP__
  struct rusage r_usage;      // for resource usage stats
#endif

  if (gettimeofday(&t_beg,NULL)) myWarn("Error: gettimeofday()",strerror(errno),0);
  t_oldest=t_newest=0;
  // parse parameters
  opterr=0;
  while ((c=getopt(argc, argv, "hl:i:o:t:T:n:r:w:u:m:g:vq"))>=0) {
    switch (c) {
      case 'l': // output language
                lang=99;
                if (!strncasecmp(optarg,"SK\000",3)) lang=0;
                if (!strncasecmp(optarg,"EN\000",3)) lang=1;
                if (!strncasecmp(optarg,"IT\000",3)) lang=2;
                if (!strncasecmp(optarg,"FR\000",3)) lang=3;
                if (!strncasecmp(optarg,"DE\000",3)) lang=4;
                if (!strncasecmp(optarg,"ES\000",3)) lang=5;
                if (!strncasecmp(optarg,"SR\000",3)) lang=6;
                if (!strncasecmp(optarg,"BR\000",3)) lang=7;
		if (lang==99) myWarn("Invalid language!","\nSK=Slovak  EN=English IT=Italiano ES=Spanish\nSR=Serbian DE=Deutsch FR=Francais BR=Portugues Brasil",2);
                break;
      case 'm': // mode of output stats
                mode=99;
                if (!strncasecmp(optarg,"text\000" ,5)) mode=0;
                if (!strncasecmp(optarg,"html\000" ,5)) mode=1;
                if (!strncasecmp(optarg,"html2\000",6)) mode=2;
		if (mode==99) myWarn("Invalid output mode!","(text, html, html2)",2);
                break;
      case 'i': // input file-name
                if (files & F_READ) myWarn("Specify mailbox file or cache as input - not both!","",2);
                if (!(fInp=fopen(optarg,"rb"))) myWarn("Cannot open input file!",strerror(errno),3);
                myCopy(sInput,optarg,40);
		files = files | F_INPUT;
                break;
      case 'o': // output file-name
                if (files & (F_WRITE | F_UPDATE)) myWarn("Specify stats file or cache as output - not both!","",2);
                if (!(fOut=fopen(optarg,"wb"))) myWarn("Cannot open output file!",strerror(errno),3);
                myCopy(sOutput,optarg,40);
		files = files | F_OUTPUT;
                break;
      case 'r': // read cache
                if (files & F_INPUT ) myWarn("Specify mailbox file or cache as input - not both!","",2);
                if (files & F_WRITE ) myWarn("Use -u option to update cache file!","",2);
                if (files & F_UPDATE) myWarn("When updating cache file don't use -r and -w options!","",2);
                myCopy(sCache,optarg,40);
		files = files | F_READ;
                break;
      case 'w': // write cache
                if (files & F_OUTPUT) myWarn("Specify stats file or cache as output - not both!","",2);
                if (files & F_READ  ) myWarn("Use -u option to update cache file!","",2);
                if (files & F_UPDATE) myWarn("When updating cache file don't use -r and -w options!","",2);
                myCopy(sCache,optarg,40);
		files = files | F_WRITE;
                break;
      case 'u': // update cache
                if (files & F_OUTPUT) myWarn("Specify stats file or cache as output - not both!","",2);
                if (files & (F_READ | F_WRITE)) myWarn("When updating cache file don't use -r and -w options!","",2);
                myCopy(sCache,optarg,40);
		files = files | F_UPDATE;
                break;
      case 't': // title - name of mailing list
                myCopy(title,optarg,40);
		onlytitle=0;
                break;
      case 'T': // whole title - display only user-specified text as title
                myCopy(title,optarg,40);
		onlytitle=1;
                break;
      case 'g': // graphs to show
                graph=0;
                for (ch=optarg; *ch!='\0'; ch++) switch(toupper(*ch)) { // scan every char of param
                  case 'D': { graph = graph | G_DAY  ; break; }
                  case 'W': { graph = graph | G_WEEK ; break; }
                  case 'M': { graph = graph | G_MONTH; break; }
                  case 'Y': { graph = graph | G_YEAR ; break; }
                  case 'X': { break; }
		  default : myWarn("Invalid graph specification!","For example use -g DWM, if you want Day, Week and Month graph (X for none).",2);
		}
                break;
      case 'n': // TOP n
                topXX=atoi(optarg); // convert to long
                break;
      case 'h': // help
                help(argv[0]);
		exit(1);
		break;
      case 'v': // verbose
                if (quiet) myWarn("Can be either verbose or quiet, not both!","",2);
                verbose=1;
		break;
      case 'q': // quiet
                if (verbose) myWarn("Can be either verbose or quiet, not both!","",2);
                quiet=1;
		break;
      case '?': // unknown opt.
		help(argv[0]);
                myWarn("Unknown option!","",2);
                break;
      case ':': // inv. param.
		help(argv[0]);
                myWarn("Invalid parameter for option!","",2);
                break;
     }
  }

  if (!quiet) banner();

  if (files &(F_READ |
   F_WRITE | F_UPDATE)) myWarn("Cache file: ",sCache,0);
  if (files & F_INPUT ) myWarn("Input file: ",sInput,0);
  if (files & F_OUTPUT) myWarn("Output file:",sOutput,0);


  if (!(files & (F_INPUT | F_READ))) {
     myWarn("No input file specified, using stdin.","",0);
     fInp=stdin;
  }
  if (!(files & (F_OUTPUT | F_WRITE | F_UPDATE))) {
     myWarn("No output file specified, using stdout.","",0);
     fOut=stdout;
  }

  if ((lang==99) && !(files & (F_WRITE | F_UPDATE))) { // no language && doing normal output (= not doing cache write/update)
     myWarn("No output language specified, using default English.","",0);
     lang=1;
  }

  if ((msgQ=(nQptr)malloc(sizeof(struct bestQUOTE)))==NULL) myWarn("Not enough memory!","",4);
  msgQ->auth[0]='\0';
  msgQ->subj[0]='\0';
  msgQ->date[0]='\0';
  msgQ->size=0;
  msgQ->perc=0;

  if ((msgD=(nDptr)malloc(sizeof(struct bestDLZKA)))==NULL) myWarn("Not enough memory!","",4);
  msgD->auth[0]='\0';
  msgD->subj[0]='\0';
  msgD->date[0]='\0';
  msgD->size=0;

  compile_mls_regex(); // pre-compile regular expressions
  for (i=0; i < 31; i++) nDen[i]=0;
  for (i=0; i < 24; i++) nHod[i]=0;
  for (i=0; i <  7; i++) nDOW[i]=0;
  for (i=0; i < 12; i++) nMon[i]=0;

  // read cache - on cache read/update options
  if (files & (F_READ | F_UPDATE)) ReadCacheFile(sCache, files & F_UPDATE);
  
  // read mailbox - if not -r (cache read)
  if (!(files & F_READ)) {
    ParseInput();
    zozQ=SortQuote(zozA);
    zozAt=SortSize(zozA,0);
    zozAa=SortSize(zozA,1);
  }

  // write cache/stats
  if (files & (F_WRITE | F_UPDATE)) WriteCacheFile(sCache); else {
    if (!mode) PrintStatText(topXX, title);
    else PrintStatHtml(topXX, title, mode);
  }

  // end
  free(msgQ);
  free(msgD);
  DeAlokuj(zozA);
  DeAlokuj(zozS);
  DeAlokuj(zozQ);
  DeAlokuj(zozAt);
  DeAlokuj(zozAa);
  if (files & F_INPUT ) fclose(fInp);
  if (files & F_OUTPUT) fclose(fOut);
  // final system stats
  if (gettimeofday(&t_end,NULL)) myWarn("Error: gettimeofday()",strerror(errno),0);
  if (!quiet) fprintf(stderr,"Execution finished. Processed %lu messages in %lu seconds.\n",nSprav,t_end.tv_sec-t_beg.tv_sec);
#ifndef __DJGPP__
  if (!quiet) {
    if (getrusage(RUSAGE_SELF,&r_usage)) myWarn("Error: getrusage()",strerror(errno),0);
    fprintf(stderr,"User   time used: %lu seconds.\n",r_usage.ru_utime.tv_sec);
    fprintf(stderr,"System time used: %lu seconds.\n",r_usage.ru_stime.tv_sec);
  }
#endif
  exit(0);
} /** MAIN **/
