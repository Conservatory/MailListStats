/****************************************************************************
    MailListStat - print useful statistics on email messages
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

 Usage information
~~~~~~~~~~~~~~~~~~~
* How to compile - see file INSTALL

* How to run it
  If you have compiled version, just run it with "-h" parameter to see usage
  information. No parameter is mandatory, if it's missing, default value is
  used.

* Command-line parameters and their description
  Default values are in {brackets}
  -h      ... print help text and exit, other options are ignored
  -v      ... turn on verbose mode {off}
              will print some progress info and also some diagnostic messages
	      about not recognised fields
  -q      ... be quiet (print only errors to stderr)
  -i file ... name of input file - should exist and be readable {stdin}
  -o file ... name of output file - will be overwritten {stdout}
  -r file ... read input from cache file instead of mailbox
  -w file ... write cache file (no stats produced)
  -u file ... update cache file (read cache, read input, write cache)
  -l lang ... output language - SK (Slovak), EN (English), IT (Italiano),
              FR (Francais), DE (Deutsch), ES (Spanish), SR (Serbian),
	      BR (Portugues Brasil) {EN}
              diagnostic messages are always printed in English
  -m mode ... mode of output (text, html, html2) {text}
  -n XX   ... print TOP XX authors, subjects and quoting {10}
  -t text ... name of mailing list, see below for details
  -T text ... title text (only this will be printed as title)
  -g xxxx ... graphs to show (Day, Week, Month, Year, Xnone) {dwm}

  You can read input either from mailbox or cache file, not both!
  You can either produce text output or write cache file, not both!
  To add some messages to existing cache file (for example for use with
  procmailrc), use -u option and read section about cache files.
  When writing cache file, output-related options are ignored.

* Input
  On input, there should be mailbox file in standard MBOX format. If the file
  is in different format, the results are unpredictable. There should be at
  least one email message, otherwise no stats can be computed.

  Warning: Be sure that no special messages are in input files (such as that
  with "DON'T DELETE THIS MESSAGE -- FOLDER INTERNAL DATA" subject), because
  they will be also analysed. Many programs (POP3/IMAP daemons, email readers)
  put their special messages to the mailbox. This message is only ignored when
  reporting oldest message found.

* Output
  Statistics is put into output file (or stdout if unspecified) in specified
  language. All diagnostic messages are written to stderr and are in English.
  Output consists of several statistical data - tables, graphs and summaries.
  The title has two formats depending on "-t" parameter. If it's not specified,
  it looks like "Statistics from 16.8.2001 to 7.9.2001", where first date is
  date of the oldest message found in input and second is date of the newest
  one. If there is for example "-t mobil@mobil.sk" parameter, it will look like
  "Statistics from 16.8.2001 to 7.9.2001 for mobil@mobil.sk". The problem is
  that date of oldest & newest msg is often wrong (thanks to bad date/time
  settings on PC of msg author), so you can specify entire title using
  command-line option "-T". When used, only your text will be printed as title,
  nothing more. There you can put for example something like "Statistics for
  mobil@mobil.sk".

  Now you have option (-g) to specify which graphs you want to show -
  hours of Day, days of Week, days of Month, months of Year. Use 1st letters
  as argument to -g option (so -g dw will print just hours of Day and days of
  Week). Use -g x to disable printing of any graph. For example you don't want
  to show graph for months of Year if you are presenting stats for one month,
  but for full-year stats you probably want it.

* HTML output
  You can choose between 2 modes of output - TEXT and HTML. When in HTML mode,
  mls will produce the output as HTML page. When you specify HTML2 mode, only
  the body of HTML document is produced (no header/footer) - it can be used
  to have different HTML header/footer when calling mls as CGI or when using
  PHP wrapper. The output consists of HTML tables and bar graphs. Almost every
  aspect of how it looks can be configured by modifying CSS style-sheet. Please
  note that files "style_mls.css" and "bar.gif" must be present in the same
  directory as produced HTML file. You can, however, modify both to best suit
  your needs. Everything should be clear after reading comments in CSS file and
  looking at the produced HTML source.

  I was unsure what type of graphs to produce. I have tried also horizontal
  bar graphs and if you want to try them, just uncomment part of code in
  PrintGraphHtml() in mls_text.c.

* Cache file support
  Instead of producing statistics in text format, you can save all the
  generated values/results into "cache" file. Retrieving information from this
  file is very fast, so it is useful for integration with web pages. Now you
  can update the cache file just after new mail was received. Users can view
  actual stats using MLS as CGI script. It has an advantage over static stats
  that user can choose language and others options and it will be generated
  in a moment!

  To update cache file, use the -u option. It works like this: first, the stats
  are loaded from cache file (doesn't have to exist) and then new message(s)
  to be added are read from stdin (or from -i file) and added to the stats.
  Finally the updated stats are written back to the cache file. The process
  is really quick, because usually only one message is added at a time. This is
  useful mainly for updating cache files upon receiving new message. In the
  "examples/" subdir, you can find examples of integration with your .forward
  and .procmailrc files. By running MLS more than once, you can generate cache
  files for individual months and also for whole years (see examples). Then use
  some PHP script to present list of these cache files to user.

  Format of cache files was changed in version 1.3, because of new stats added.
  Now it contains version info, so mls can inform you that you have to
  re-create that cache file with new version. Unfortunately, you have to
  re-create them also when you want new email clients to be recognized also in
  old (already processed) messages. Note that email clients detection was buggy
  in 1.2.2 (a lot of clients not recognized).

* PHP wrapper
  I have written also PHP wrapper for MLS to make it more "interactive". It has
  two major advantages over plain HTML output from MLS: User can choose output
  language and number of TOP items to show. It works by running MLS with
  appopriate command-line options. It's safe, because only two items from user
  are language and topXX which are checked using regexp, so running arbitrary
  code is not possible. You can also alter MLS output - for example change @
  in email addresses to (at) to prevent spamming.
  
  You can have normal MBOX file as input, but I recommend using cache file.
  When using cache file, the stats are produced in a moment. You can see how
  long it took to generate the page, see the last line of HTML source. However,
  there is minor speed problem. It takes longer when you specify to show many
  topXX (like 999). The problem is regexp that searches for @. It has to search
  for it in whole MLS output together and when it is large, it takes a while
  (1.1 seconds on my 2.1GHz pentium4). I have added an option which should use
  Perl-compatible regex function (preg_replace) instead of POSIX (ereg_replace)
  if available in your PHP module. This will result in MUCH faster execution
  (50ms instead of 1.1sec).

* How it is all computed
  or
* What is quoting? Why I have it 95%?

  OK, so let's start from beginning - the format of MBOX file. It's plain text
  file containing some email messages delimited with one empty line. Each
  message starts with line like this "From abc@a.sk  Thu Aug 16 15:48:58 2001".
  After this line, there are few headers, one empty line and message text.
  Storing emails in this format is quite common - your incoming mail is usually
  saved in MBOX format and also your folders in mail-readers like elm, pine,
  mutt...

  Who is author of an email message? It's taken from "From:" header field and
  everything except the actual email address (like your full name) is stripped
  off using quite simple regular expression (regexp).

  Subject is taken from "Subject:" header field. If it contains some "Re:",
  those will be stripped off. There can be up to 5 of them. Also counted format
  ("Re[5]:") is supported. For example The Bat! email client uses it.
  MIME-decoding is applied to subject lines (see below).
  
  Date is just everything in the "Date:" header. This header is generated by
  the email client, so it's date of message creation and it doesn't have to
  be present in each message. If it isn't, you are warned by message like
  "Warning: 1 message(s) not counted." in output. Some client doesn't put
  full date there and usually the day of week is missing and you are warned.
  No timezones are considered, the date is taken as-is.

  Message size is everything between end of message header and beginning of
  new email (or end of file). So only actual size of message text (body) is
  counted, not headers.

  Email clients are taken from "X-Mailer:" or "User-Agent:" or "X-Newsreader:"
  headers and some grouping is done to avoid different versions of the same
  mailer to take the whole TOP 10. There is also work-around for Pine mailer
  (MLS will search also "Message-ID:" header).

  What is quoting? When you reply to some message, you can insert part of the
  original message there, you quote the author of original message. Every line
  of original text is usually prepended with ">" or "MP>", where MP are
  initials of the original sender's name (for example The Bat! uses this
  second format).

  And what is "quote ratio"? It's size of quoted text divided by total size
  of message, specified in percent. It's included in stats, because many
  people reply to message, add one line of text and leaving there for
  example 10 pages of original text, which makes the quote ratio even
  higher than 90%! In times of FIDONET, there were conferences, where quote
  ratio higher than 50% was forbidden. Try to think about it when replying
  to message in mailing list where more than 300 people will download and
  read it.

  And now the stats! At first, there are TOP 10 tables (or TOP XX when using
  "-n XX" parameter). First table shows people who have written most
  messages, how much and how many percent of total message count it is. Last
  row shows the "other" - number of messages written by everyone not listed
  above and how many percent it is. Second and third tables are similar to this
  one - they also show best authors, but not by the number of messages written.
  Authors are sorted by total (or average) size of all their messages, but
  without quoting (size of message minus how much was quoted in that msg).
  Next table shows most successful subjects and how many messages with
  this subject have been posted. The other table shows most used email clients.
  The last table show people with maximal quote ratio. It's computed as sum of
  quoted text in all his/her messages divided by total size of those messages.
  Last row shows an average - sum of quoted text in all messages divided by
  total size of all messages.

  Next part of stats are some graphs. They show how much messages have been
  written during different hours of day, days of month and days of week. From
  these you can see for example when (and how much) people sleep :) or if they
  work during the working-hours or just write tons of messages...

  Next part contains info about messages which are BEST in something - message
  with max. quote ratio, longest message and some details about most successful
  subject.

  At the end, there is final summary - total number of messages, their total
  and average size and number of different authors and subjects.

* Inspiration
  I was inspired by similar program used before few years in FIDONET and Slovak
  ULTRANET. It was created by Ivan Friedlander.

* MIME (Multipurpose Internet Mail Extensions)
  What is it? Original implementation email permitted only 7bit ASCII messages.
  But during the time, there was need to send international or even binary
  files. MIME defines how can these be encoded into 7bit form suitable for
  emailing and how to decode it back to human readable form.

  In email message, you can have MIME-encoded text (body of message), but also
  some headers - for example subject and From field. MLS tries to find out if
  subject lines are MIME-encoded and if so, it tries to decode it, to present
  it to you in human-readable form.

  You can read more about MIME in RFC 1521 and 1522.
/****************************************************************************/
