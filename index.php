<?php

require_once("include/response.php");
header("Content-Type: text/plain");


if ($_SERVER["REQUEST_METHOD"] == "GET"){
        
        echo("Go Variant Server");

} else if ($_SERVER["REQUEST_METHOD"] == "POST"){

        //print_r($_POST);
        handle_post_request($_POST);
        
}

?>