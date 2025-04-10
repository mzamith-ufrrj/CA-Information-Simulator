<?php
   $filename = "/opt/php-date/users.md5";
   $myfile = fopen($filename, "r") or die("Unable to open file!");
   echo fread($myfile,filesize($filename));
   fclose($myfile);
?>
