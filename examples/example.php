<?php
/***
Short example to show usage of MailListStat PHP class
See file mls_class.php for complete overview of available options...
***/
require("mls_class.php");
$mls=new MailListStat;
$mls->title="Statistics for MOBiL (year 2002)";
$mls->default_lang="sk";
$mls->default_topX="25";
$mls->path="/usr/local/bin/mls"; // mls executable
$mls->input="/home/marki/2002.cache"; // input file
$mls->graph="dwmy"; // show graphs for Day, Week, Month and Year
$mls->is_cache=true; // input file is cache (and not MBOX)
$mls->Run();
?>
