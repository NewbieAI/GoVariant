<?php


require($_SERVER["DOCUMENT_ROOT"]."/include/reset_funcs.php");
$password = file_get_contents("masterpassword.txt");


//echo $_POST["password"]."\n";
//echo $password."\n";

if ($_POST["password"] == $password)
{
        
        resetDirectory($_SERVER["DOCUMENT_ROOT"].'/data');
        generateFiles();
        echo __dir__." reseting...";
        
        
} else {

        echo "Forbidden. Use by site owner only";
        
}


?>