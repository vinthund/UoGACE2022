<?php

$dbhost = "localhost";
$dbuser = "root";
$dbpass = "ACEgre2052";
$dbname = "login";

if(!$con = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname));
{
    die("failed to connect!");
}
