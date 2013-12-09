/****************************************************************************
    MailListStat - print useful statistics on email messages
    language header file
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

/*
** Program: MailListStat (MLS) - language include file
** IT - contributed by Fabio Ferrero <ferrero@interim.it>
** DE - contributed by Albert Marencin <marencin@nextra.sk>
** FR - contributed by Albert Marencin <marencin@nextra.sk>
**                 and Francois Massonneau <francois@arvernes.com>
** ES - contributed by Luis Llorente <luisllorente@luisllorente.com>
** SR - contributed by Urke MMI <urke@gmx.net>
** BR - contributed by Elias Andrade <eliasandrade@lol.com.br>
**
** define string constants in form: char langXXX[NUM_LANG][size]
** use: printf("%s", langXXX[lang]);
** where lang=0|1|2|3|4|5|6|7
** 0=SK Slovak
** 1=EN English
** 2=IT Italiano
** 3=FR Francais
** 4=DE Deutsch
** 5=ES Spanish
** 6=SR Serbian
** 7=BR Portugues
*/

#ifndef	_MLS_LANG_H
#define	_MLS_LANG_H	1

#define NUM_LANG 8  /* Number of languages (+1). Used for arrays sizes */

// you can modify sizes (the second []), but the text must still fit

char langTOPpeople[NUM_LANG][80]={
     "Tabulka ludi, ktori napisali najviac sprav:",
     "People who have written most messages:",
     "Tabella di chi ha scritto piu' messaggi:",
     "Personnes ayant �crit le plus de messages:",
     "Absender mit meisten Nachrichten:",
     "Personas que han escrito m�s mensajes:",
     "Autori sa najvi�e napisanih poruka:",
     "Pessoas que escrevem mais mensagens:"
};

char langTOPsubj[NUM_LANG][80]={
     "Tabulka najuspesnejsich subjectov:",
     "Table showing the most successful subjects:",
     "Tabella degli oggetti piu' utilizzati:",
     "Les sujets suscitant le plus d'int�r�t:",
     "Am meisten benuetzte Betreffs:",
     "Asuntos m�s utilizados:",
     "Diskusije sa najvi�e u�esnika:",
     "Tabela mostrando os assuntos mais utilizados:"
};

char langTOPmail[NUM_LANG][80]={
     "Tabulka najpouzivanejsich mailovych klientov:",
     "Most used email clients:",
     "Programmi di posta piu' utilizzati:",
     "Les clients d'Email les plus utilis�s:",
     "Am meisten benuetzte Klienten:",
     "Clientes de correo m�s utilizados:",
     "Najzastupljeniji klijenti:",
     "Clientes de correio mais utilizados:"
};

char langTOPquote[NUM_LANG][80]={
     "Ludia, ktori najviac quotovali:",
     "Table of maximal quoting:",
     "Tabella di chi ha quotato di piu':",
     "Ceux ayant repris le plus de lignes d'anciens messages:",
     "Absender die am meisten gequotet haben:",
     "Personas que m�s han citado:",
     "Autori sa najvi�e citiranog teksta:",
     "Pessoas que mais s�o citadas:"
};

char langTOPauthorSizeT[NUM_LANG][80]={
     "Najlepsi autori, podla celkovej dlzky ich sprav (bez quotovania):",
     "Best authors, by total size of their messages (w/o quoting):",
     "Chi ha scritto di piu', per dimensione totale dei messaggi (senza quote):",
     "Meilleurs auteurs, par longueur totale de leurs messages (ss quoter):",
     "Beste Autoren, nach Gesamtgroesse der Nachrichten (ohne Quotation):",
     "Mejores autores, por tama�o total de sus mensajes (sin citar):",
     "Najbolji autori, prema ukupnoj veli�ini autorskih poruka (bez citata):",
     "Melhores autores, por tamanho total de suas mensagens (Sem citar):"
};

char langTOPauthorSizeA[NUM_LANG][80]={
     "Najlepsi autori, podla priemernej dlzky ich sprav (bez quotovania):",
     "Best authors, by average size of their message (w/o quoting):",
     "Chi ha scritto di piu', per dimensione media dei messaggi (senza quote):",
     "Meilleurs auteurs, par longueur moyenne de leurs messages (ss quoter):",
     "Beste Autoren, nach Durchschnittsgroesse der Nachrichten (ohne Quotation):",
     "Mejores autores, por tama�o medio de su mensaje (sin citar):",
     "Najbolji autori, prema prose�noj veli�ini autorskih poruka (bez citata):",
     "Melhores autores, por tamanho m�dio de suas mensagens (Sem citar):"
};

char langGAuthor[NUM_LANG][10]={
     "-Autor--",
     "-Author-",
     "-Autore-",
     "-Auteur-",
     "Absender",
     "-Autor--",
     "-Autor--",
     "-Autor--"
};
// this is for HTML (without --)
char langGAuthorHtml[NUM_LANG][10]={
     "Autor",
     "Author",
     "Autore",
     "Auteur",
     "Absender",
     "Autor",
     "Autor",
     "Autor"
};

char langGMess[NUM_LANG][10]={
     "-Sprav",
     "--Msg-",
     "--Msg-",
     "--Msg-",
     "-Nachr",
     "--Msg-",
     "Poruka",
     "--Msg-"
};
// this is for HTML (without --)
char langGMessHtml[NUM_LANG][10]={
     "Sprav",
     "Msg",
     "Msg",
     "Msg",
     "Nachr",
     "Msg",
     "Poruka",
     "Msg"
};

char langDOW[NUM_LANG][30]={
     "Po  Ut  Str Stv Pi  So  Ne ",
     "Mon Tue Wed Thu Fri Sat Sun",
     "Lun Mar Mer Gio Ven Sab Dom",
     "Lun Mar Mer Jeu Ven Sam Dim",
     "Mo  Di  Mi  Do  Fr  Sa  So ",
     "Lun Mar Mie Jue Vie Sab Dom",
     "Pon Uto Sre �et Pet Sub Ned",
     "Seg Ter Qua Qui Sex Sab Dom"
};

char langMON[NUM_LANG][50]={
     "Jan Feb Mar Apr Maj Jun Jul Aug Sep Okt Nov Dec",//SK - ok
     "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec",//EN - ok
     "Gen Feb Mar Apr Mag Giu Lug Ago Set Ott Nov Dic",//IT - ok
     "Jan F�v Mar Avr Mai Jun Jui Ao� Sep Oct Nov D�c",//FR - ok
     "Jan Feb Mar Apr Mai Jun Jul Aug Sep Okt Nov Dez",//DE - ok
     "Ene Feb Mar Abr May Jun Jul Ago Sep Oct Nov Dic",//ES - ok
     "Jan Feb Mar Apr Maj Jun Jul Avg Sep Okt Nov Dec",//SR - ok
     "Jan Fev Mar Abr Mai Jun Jul Ago Set Out Nov Dez" //BR - ok
};

char langTit1[NUM_LANG][30]={
     "Statistika od",
     "Statistics from",
     "Statistica dal",
     "Statistiques du",
     "Statistik von",
     "Estad�sticas del",
     "Statistika za period od",
     "Estat�sticas de"
};

char langTit2[NUM_LANG][5]={
     "do",
     "to",
     "al",
     "au",
     "bis",
     "al",
     "do",
     "�"
};

char langTit3[NUM_LANG][5]={
     "pre",
     "for",
     "per",
     "pour",
     "fuer",
     "por",
     "za",
     "de"
};

char langGhour[NUM_LANG][80]={
     "Prehlad, v ktorom case bolo napisanych najviac sprav:",
     "Graph showing number of messages written during hours of day:",
     "Grafico del numero di messaggi scritti nelle ore del giorno",
     "Graphique montrant le nbre de msgs �crits durant les heures de la journ�e:",
     "Die Zeit, zu der die meisten Nachrichten geschrieben worden:",
     "N�mero de mensajes escritos a distintas horas del d�a:",
     "Grafik broja kreiranih poruka po satima u toku dana:",
     "Gr�fico mostrando n�meros de mensagens escritas durante as horas do dia:"
};

char langGday[NUM_LANG][80]={
     "Pocet sprav napisanych za jednotlive dni v mesiaci:",
     "Graph showing number of messages written during days of month:",
     "Grafico del numero di messaggi scritti nei giorni del mese",
     "Graphique montrant le nbre de msgs �crits durant les jours du mois:",
     "Zahl der Nachrichten die in Tagen des Monats geschrieben worden:",
     "N�mero de mensajes escritos a lo largo del mes:",
     "Grafik broja kreiranih poruka po danima u toku meseca:",
     "Gr�fico mostrando n�meros de mensagens escritas durante os dias do m�s:"
};

char langGdow[NUM_LANG][80]={
     "Pocet sprav napisanych za jednotlive dni v tyzdni:",
     "Graph showing number of messages written during days of week:",
     "Grafico del numero di messaggi scritti nei giorni della settimana",
     "Graphique montrant le nbre de msgs �crits durant les jours de la semaine:",
     "Zahl der Nachrichten die in den Wochentagen geschrieben worden:",
     "N�mero de mensajes escritos a lo largo de la semana:",
     "Grafik broja kreiranih poruka kreiranih po danima u toku nedelje:",
     "Gr�fico mostrando n�meros de mensagens escritas durante dias da semana:"
};

char langGmon[NUM_LANG][80]={
     "Pocet sprav napisanych za jednotlive mesiace:",
     "Graph showing number of messages written during months of year:",
     "Grafico del numero di messaggi scritti nei mesi del'anno:",
     "Graphique montrant le nbre de msgs �crits durant les mois de l'ann�e:",
     "Graphik mit Zahl der in den jeweiligen Monaten geschriebenen Nachrichten:",
     "Gr�fico mostrando el n�mero de mensajes escritos en los distintos meses del a�o:",
     "Grafik broja kreiranih poruka po mesecima u toku godine:",
     "Gr�fico monstrando o n�mero de mensagens escritas durante os meses do ano:"
};

char langWarn[NUM_LANG][20]={
     "Upozornenie",
     "Warning",
     "Attenzione",
     "Attention",
     "Achtung",
     "Atenci�n",
     "Napomena",
     "Aten��o"
};

// means "other people"
char langOstI[NUM_LANG][20]={
     "ostatni",
     "other",
     "altro",
     "autres",
     "andere",
     "otros",
     "ostali",
     "outras"
};

// means "other messages" - if it is different in your language
char langOstE[NUM_LANG][20]={
     "ostatne",
     "other",
     "altro",
     "Autres",
     "andere",
     "otros",
     "druge",
     "outras"
};

char langAvg[NUM_LANG][20]={
     "priemer",
     "average",
     "media",
     "moyenne",
     "Durchschnitt",
     "media",
     "prose�no",
     "m�dia" 
};

char langCount[NUM_LANG][60]={
     "sprav nebolo zaratanych",
     "message(s) not counted",
     "messaggi non contati",
     "messages non compt�s",
     "nicht gezaehlte Nachrichten",
     "mensajes sin contar",
     "poruka nije ura�unato",
     "mensagens n�o contadas"
};

char langMaxQuot[NUM_LANG][60]={
     "Najviac quotoval (a najmenej napisal):",
     "Maximal quoting:",
     "Messaggio piu' quotato:",
     "Maximum de citation d'anciens messages:",
     "Meist gequotete Nachricht:",
     "Mensaje m�s citado:",
     "Autor sa najvi�e citiranog (a najmanje autorskog) teksta:",
     "Mensagens mais citadas:"
};

char langMaxLeng[NUM_LANG][60]={
     "Najdlhsia sprava:",
     "Longest message:",
     "Messaggio piu' lungo:",
     "Message le plus long:",
     "Laengste Nachricht:",
     "Mensaje m�s largo:",
     "Najdu�a poruka:",
     "Maiores mensagens:"
};

char langMaxSubj[NUM_LANG][60]={
     "Najuspesnejsi subject:",
     "Most successful subject:",
     "Oggetto piu' utilizzato:",
     "Sujet ayant le plus de succ�s:",
     "Meist benuetzter Betreff:",
     "Asunto m�s utilizado:",
     "Diskusija sa najvi�e u�esnika:",
     "Assuntos mais utilizados:"
};

char langEndStat[NUM_LANG][60]={
     "Zaverecny prehlad:",
     "Final summary:",
     "Riassunto finale:",
     "R�sum� final:",
     "Enduebersicht:",
     "Resumen final:",
     "Sumarna statistika:",
     "Resumo final:"
};

// means "unknown authors/mailer"
char langUnknown[NUM_LANG][40]={
     "(neznamy)",
     "(unknown)",
     "(sconosciuto)",
     "(inconnu)",
     "(unbekannt)",
     "(desconocido)",
     "(nepoznato)",
     "(desconhecido)"
};

// these should be equally long in your language
// (because of alignment of output)
char langTotMess[NUM_LANG][80]={
     "Celkovy pocet sprav:                 ",
     "Total number of messages:            ",
     "Numero totale di messaggi:       ",
     "Nombre total de messages:           ",
     "Gesamtzahl der Nachrichten:     ",
     "N�mero total de mensajes:         ",
     "Ukupan broj poruka:                  ",
     "N�mero total de mensagens:          "
};
char langTotAuth[NUM_LANG][80]={
     "Celkovy pocet zucastnenych pisatelov:",
     "Total number of different authors:   ",
     "Numero totale di autori diversi: ",
     "Nombre total d'auteurs diff�rents:  ",
     "Gesamtzahl beteiligter Absender:",
     "N�mero total de autores distintos:",
     "Ukupan broj razli�itih autora:       ",
     "N�mero total de diferentes autores: "
};
char langTotSubj[NUM_LANG][80]={
     "Celkovy pocet roznych subjectov:     ",
     "Total number of different subjects:  ",
     "Numero totale di oggetti diversi:",
     "Nombre total de sujets diff�rents:  ",
     "Gesamtzahl der Betreffs:        ",
     "N�mero total de asuntos distintos:",
     "Ukupan broj razli�itih tema:         ",
     "N�mero total de diferentes assuntos:"
};
char langTotLeng[NUM_LANG][80]={
     "Celkova dlzka sprav (bez headerov):  ",
     "Total size of messages (w/o headers):",
     "Lunghezza totale (senza headers):",
     "Taille totale des msgs (ss ent�tes):",
     "Gesamtlaenge (ohne Kopfzeilen): ",
     "Tama�o total (sin cabeceras):     ",
     "Ukupna du�ina poruka (bez zaglavlja):" ,
     "Tamanho total (Sem cabe�alhos):     "
};
char langAvgLeng[NUM_LANG][80]={
     "Priemerna dlzka jednej spravy:       ",
     "Average size of a message:           ",
     "Lunghezza media di un messaggio: ",
     "Taille moyenne d'un message:        ",
     "Durchschnittliche Laenge:       ",
     "Tama�o medio del mensaje:         ",
     "Prose�na du�ina poruka:              ",
     "Tamanho m�dio da mensagem:          "
};

// these also
char langAuth[NUM_LANG][40]={
     "Autor      :",
     "Author     :",
     "Autore      :",
     "Auteur        :",
     "Absender   :",
     "Autor         :",
     "Autor          :",
     "Autor       :"
};
char langSubj[NUM_LANG][40]={
     "Subject    :",
     "Subject    :",
     "Oggetto     :",
     "Sujet         :",
     "Betreff    :",
     "Asunto        :",
     "Tema           :",
     "Assunto     :"
};
char langDate[NUM_LANG][40]={
     "Datum      :",
     "Date       :",
     "Data        :",
     "Date          :",
     "Datum      :",
     "Fecha         :",
     "Datum          :",
     "Data        :"
};
char langRate[NUM_LANG][40]={
     "Quotovane  :",
     "Quote ratio:",
     "Quotato     :",
     "Ratio Citation:",
     "Quotationen:",
     "Ratio citaci�n:",
     "Citat          :",
     "Cita��es    :"
};
char langSize[NUM_LANG][40]={
     "Dlzka      :",
     "Size       :",
     "Dimensione  :",
     "Taille        :",
     "Laenge     :",
     "Tama�o        :",
     "Veli�ina       :",
     "Tamanho     :"
};
char langNumb[NUM_LANG][40]={
     "Pocet sprav:",
     "No. of msgs:",
     "Num. di msgs:",
     "Nbre des msgs :",
     "Zahl der N.:",
     "N�m. de msgs  :",
     "Broj poruka    :",
     "N�m. de msgs:"
};
char langSSiz[NUM_LANG][40]={
     "Ich dlzka  :",
     "Total size :",
     "Dim. totale :",
     "Taille totale :",
     "Gesamtl.   :",
     "Tam. total    :",
     "Ukupna veli�ina:",
     "Tam. total  :"
};


char langByte[NUM_LANG][10]={
     "bytov",
     "bytes",
     "bytes",
     "octets",
     "bytes",
     "bytes",
     "bajta",
     "bytes"
};

char langHour[NUM_LANG][6]={
     "hod. ",
     "hour ",
     "ora  ",
     "heure",
     "uhr  ",
     "hora ",
     "sati ",
     "hora "
};

char langDay[NUM_LANG][5]={
     "den ",
     "day ",
     "gg  ",
     "jour",
     "Tag ",
     "d�a ",
     "dan ",
     "dia "
};

// not longer than 7 chars to fit on screen
// (it's on the right side of graphs)
char langMess[NUM_LANG][10]={
     "sprav",
     "msgs",
     "msgs",
     "msgs.",
     "Nachr.",
     "msgs",
     "poruka",
     "msgs"
};

char langPercent[NUM_LANG][10]={
     "-Percent-",
     "-Percent-",
     "-Percent-",
     "-Percent-",
     "-Percent-",
     "-Percent-",
     "-Procenat",
     "-Porcento-"
};
// this is for HTML (without --)
char langPercentHtml[NUM_LANG][10]={
     "Percent",
     "Percent",
     "Percent",
     "Percent",
     "Percent",
     "Percent",
     "Procenat",
     "Porcento"
};

char langSubject[NUM_LANG][10]={
     "-Subject-",
     "-Subject-",
     "-Oggetto-",
     "--Sujet--",
     "-Betreff-",
     "-Asunto--",
     "---Tema--",
     "-Assunto-"
};
// this is for HTML (without --)
char langSubjectHtml[NUM_LANG][10]={
     "Subject",
     "Subject",
     "Oggetto",
     "Sujet",
     "Betreff",
     "Asunto",
     "Tema",
     "Assunto"
};

char langMailer[NUM_LANG][10]={
     "-Mailer--",
     "-Mailer--",
     "-Mailer--",
     "-Mailer--",
     "-Mailer--",
     "-Mailer--",
     "-Klijent-",
     "-Mailer--"
};
// this is for HTML (without --)
char langMailerHtml[NUM_LANG][10]={
     "Mailer",
     "Mailer",
     "Mailer",
     "Mailer",
     "Mailer",
     "Mailer",
     "Klijent",
     "Mailer"
};


// charset to use in html output
char langCharset[NUM_LANG][16]={
     "iso-8859-1",
     "iso-8859-1",
     "iso-8859-1",
     "iso-8859-1",
     "iso-8859-1",
     "iso-8859-1",
     "iso-8859-2",
     "iso-8859-1"
};

#endif /* mls_lang.h */
//************//
//**** END ***//
//************//
