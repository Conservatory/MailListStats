/****************************************************************************
    MailListStat - print useful statistics on email messages
    MIME header file
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

#ifndef	_MLS_MIME_H
#define	_MLS_MIME_H	1

regex_t r_qp, r_base64, r_hex;

/* ********************************************* FUNCTION DECLARATIONS *** */
int base64_table(char *lookup);
void replaceStr(char *input, int so, int eo, char *repl);
void Decode_QuotedPrint(char *encoded, char *charset);
void Decode_base64(char *encoded, char *charset);
void compile_mime_regex();
void Decode_mime_string(char *input, unsigned int max_size);

#endif /* mls_mime.h */
