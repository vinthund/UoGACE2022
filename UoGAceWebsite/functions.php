<?php

function check_login($con)
{
    if(isset($_SESSION['user']))
    {
        $id = $_SESSION['user'];
        $query = "SELECT * from Login where user = '$id' limit 1";
        result = mysqli_query($con, $query)
        if($result && mysqli_num_rows($result) > 0)
        {
            $user_data = mysqli_fetch_assoc($result);
            return $user_data;

        }
    }
    //redirect to login page
    header ("Location: login.php");
    die;
}