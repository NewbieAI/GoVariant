<?php
require_once("include/database_control.php");
require_once("include/response.php");


$post = array();

$i = 1;
while (array_key_exists("query_$i", $_POST)) {
  $post[$_POST["query_$i"]] = $_POST["field_$i"];
  $i++;
}

/*
  "type"=>"host",
  "authentication_code"=>370403801,
  "size"=>"19x19",
  "host_color"=>"random",
  "timer"=>"10:30",
  "spectator_on"=>"On",
  
  
  "type"=>"join",
  "authentication_code"=>924672402,
  "game_id"=>1,
  
  "type"=>"login",
  "username"=>$username,
  "password"=>$password
  
  "type"=>"logout",
  "authentication_code"=>808863202
  
  
*/

//  "type"=>"update",
//  "authentication_code"=>263685001

showme($post);
handle_post_request($post);

?>
