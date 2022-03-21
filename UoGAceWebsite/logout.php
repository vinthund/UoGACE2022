<?php

session_start();
if(isset($_SESSION['user']));
{
    unset($_SEESION['user_id']);

}

header('Location:	Login.php');
die;