<?php 
session_start();
    $_SESSION;
    include("connection.php");
    include("functions.php");
    $user_data = check_login($con);



?>
<!DOCTYPE html>
<html>
    <head>
        <title>Ace Machines</title>
    </head>
    <body>
        <a href="logout.php">Logout</a>

        <h1>This is the index page</h1>

        Hello, <?php echo $user_data['user'];?>

    </body>

</html>