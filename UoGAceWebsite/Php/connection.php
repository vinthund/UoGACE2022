<?php

$dbhost = "localhost";
$dbuser = "root";
$dbpass = "ACE@gre2052";
$dbname = "acewebsite";

if(!$con = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname));
{
    die("failed to connect!");
}