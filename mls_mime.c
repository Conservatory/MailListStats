/****************************************************************************
    MailListStat - print useful statistics on email messages
    MIME & charset conversion functions
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
#include "mls_mime.h"
#include "mls_stat.h"

void replaceStr(char *input, int so, int eo, char *repl) {
 // replace part of input from index so to eo with repl
 // repl should be smaller than eo-so+1!
 // max. length of remainder of input is 256!
 char tmp[256];
 int l;
 myCopy(tmp,input+eo+1,256); // put remainder of input to tmp
 l=strlen(repl);
 strncpy(input+so,repl,l);  // put repl to right place (no \0 after)
 strcpy(input+so+l,tmp);    // it should fit
}

void Decode_QuotedPrint(char *encoded, char *charset) {
 // decode quoted-printable encoded-word
 // copy result to input -- its length will be smaller or equal, so it's safe
 regmatch_t pm[10];  // buffer for matching substrings
 char hex[4];
 char *tmp;
 int ihex;
 // change underscore to space [RFC-1522 4.2.(2)]
 for (tmp=encoded; *tmp!='\0'; tmp++) if (*tmp=='_') *tmp=' ';
 while (!regexec(&r_hex, encoded, 10, pm, 0)) {
   myCopy(hex,encoded+pm[1].rm_so+1,3);
   ihex=strtol(hex,NULL,16);
   hex[0]=ihex;
   hex[1]='\0';
   replaceStr(encoded,pm[1].rm_so,pm[1].rm_eo-1,hex);
 }
}

int base64_table(char *lookup) {
 // The base64 alphabet according to RFC-1521 sec. 5.2, Table 1
 if (*lookup>='A' && *lookup<='Z') return *lookup-65;
 if (*lookup>='a' && *lookup<='z') return *lookup-71;
 if (*lookup>='0' && *lookup<='9') return *lookup+04;
 if (*lookup=='+') return 62;
 if (*lookup=='/') return 63;
 return 0;
}

void Decode_base64(char *encoded, char *charset) {
 // decode base64 encoded-word
 // copy result to input -- its length will be smaller or equal, so it's safe
 char decoded[76]="";
 char ch;
 char *tmp,*tmp2;
 unsigned long bits24,mask24;
 int i;
 
 tmp2=decoded;
 for (tmp=encoded; *tmp!='\0'; tmp+=4) {
   bits24=0;
   for (i=0; i<4; i++) {
     if (*(tmp+i)!='=') bits24+=base64_table(tmp+i); // ignore padding char (=)
     bits24 = bits24 << 6; // shift 6bits left
   }
   bits24 = bits24 >> 6; // shift 6bits right
   // now we have 24bits of encoded data
   // split into 3 8bit chars
   mask24=255 << 16;
   for (i=2; i>=0; i--) {
     ch=(short)((bits24 & mask24) >> (i*8));
     if (ch!='\0') *(tmp2++)=ch; // ignore chars made of padding
     mask24 = mask24 >> 8;
   }
 }
 *tmp2='\0';
 strcpy(encoded,decoded); // decoded should be smaller!
}

void compile_mime_regex() {
 char sB[]="(=\\?([0-9a-z-]*)\\?B\\?([0-9a-z+/=]*)\\?=)";
 char sQ[]="(=\\?([0-9a-z-]*)\\?Q\\?(([!-<]|[>-~]|=[0-9a-f]{2}|\\0x9| |_)*)\\?=)";
 char sH[]="(=[0-9a-z]{2})";

 if (regcomp(&r_base64, sB, REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(mime_base64)",5);
 if (regcomp(&r_qp    , sQ, REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(mime_qp)",5);
 if (regcomp(&r_hex   , sH, REG_EXTENDED | REG_ICASE) > 0) myWarn("Error compiling regex!","(mime_hex)",5);
}

void Decode_mime_string(char *txt, unsigned int max_size) {
 // decode string containing 0 or more MIME-encoded parts; no newlines
 // overwrite input, but it should be at most max_size
 // decoded string will be smaller (at most equal)
 // charset conversion can make it bigger

 int i;
 char charset[40];   // charset name
 char enc[76];       // encoded text
 regmatch_t pm[10];  // buffer for matching substrings
 // try to decode Qouted-Printable parts
 while (!regexec(&r_qp, txt, 10, pm, 0)) {
   i=pm[2].rm_eo - pm[2].rm_so + 1;
   myCopy(charset, txt+pm[2].rm_so, (i>40)?40:i);
   i=pm[3].rm_eo - pm[3].rm_so + 1;
   myCopy(enc, txt+pm[3].rm_so, (i>76)?76:i);
   Decode_QuotedPrint(enc,charset);
   replaceStr(txt,pm[1].rm_so,pm[1].rm_eo-1,enc);
 }
 // try to decode Base64 parts
 while (!regexec(&r_base64, txt, 10, pm, 0)) {
   i=pm[2].rm_eo - pm[2].rm_so + 1;
   myCopy(charset, txt+pm[2].rm_so, (i>40)?40:i);
   i=pm[3].rm_eo - pm[3].rm_so + 1;
   myCopy(enc, txt+pm[3].rm_so, (i>76)?76:i);
   Decode_base64(enc,charset);
   replaceStr(txt,pm[1].rm_so,pm[1].rm_eo-1,enc);
 }
}
