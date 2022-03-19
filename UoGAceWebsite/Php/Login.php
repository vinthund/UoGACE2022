<?php 
session_start();
    $_SESSION;
    include("connection.php");
    include("functions.php");

    if($_SERVER['REQUEST_METHOD'] == POST) 
    {
        $user_name = $_POST('user');
        $user_password = $_POST('password');
        
        if(!empty($user_name) && !empty($user_password) && !is_numeric($user_name))
        {
            //read from database
            $query = "SELECT * from Login where user = '$user_name' limit 1";
            
            if ($result)
            {
                if($result && mysqli_num_rows($result) > 0)
                {
                    $user_data = mysqli_fetch_assoc($result);
                    if($user_data['password'] == $password)
                    {
                        $_SESSION['user'] = $user_data['id'];
                        header("Location: index.php");
                        die;
                    }
                }
            }
            echo "wrong username or password!";
        }
        else
        {
            echo "wrong username or password!";
        }
    }
    

?>
<!DOCTYPE html>
<html>
    <head>
        <title>Login</title>
    </head>
    <body>
        <a href="logout.php">Logout</a>

        <h1>This is the Login page</h1>
        <div id="box">
            <form method="post">
                <input type ="text" name="username"><br>
                <input type="password" name="password"><br>
                <input type="submit" value="Login"><br>
                <a href=""> CLICK TO SIGN UP</a>
            </form>
        </div>

    </body>

</html>