<?php

$root = $_SERVER["DOCUMENT_ROOT"];
$path = $root."/data";
$capacity = intval(file_get_contents("$root/owner/capacity.txt"));

$game_ids = array(
  "max_capacity"=>$capacity,
  "available"=>range(1, $capacity)
);
$user_ids = array(
  "max_capacity"=>$capacity,
  "available"=>range(1, $capacity)
);

$active_users = array(

);
$active_games = array(

);


$game_info = array(
  "status"=>"closed",
  "host"=>"",
  "opponent"=>"",
  "size"=>"",
  "host_color"=>"",
  "timer"=>"",
  "allow_spectator"=>0,
  "player_count"=>0,
  "player_black"=>"",
  "player_white"=>"",
  "score_black"=>-1,
  "score_white"=>-1,
);
$game_players = array(

);
$game_moves = array(

);
$game_messages = array(

);

function resetDirectory($dir){
  if (file_exists($dir)){
    chmod($dir, 0711);
    $files = glob($dir."/*");
    foreach($files as $file){
      if (is_file($file)){
        unlink($file);
      } else {
        resetDirectory($file);
      }
    }
    rmdir($dir);
  }
}

function createDefaultFile($filename, $default_content, $mode){
  $file_handle = fopen($filename, "w+");
  fwrite($file_handle, $default_content);
  fclose($file_handle);
  chmod($filename, $mode);
}

function generateFiles(){
  global $path;
  global $capacity;
  global $game_ids;
  global $user_ids;
  global $active_games;
  global $active_users;
  global $game_info;
  global $game_players;
  global $game_moves;
  global $game_messages;
  mkdir($path, 0700);
  createDefaultFile($path."/game_id.txt", serialize($game_ids), 0600);
  createDefaultFile($path."/user_id.txt", serialize($user_ids), 0600);
  createDefaultFile($path."/active_games.txt", serialize($active_games), 0600);
  createDefaultFile($path."/active_users.txt", serialize($active_users), 0600);
  createDefaultFile($path."/messages.txt", serialize($game_messages), 0600);
  mkdir($path."/game", 0700);
  for ($i = 1; $i <= $capacity/2; $i++) {
    print("creating file<br>");
    createDefaultFile($path."/game/info_$i.txt", serialize($game_info), 0600);
    createDefaultFile($path."/game/players_$i.txt", serialize($use_ids), 0600);
    createDefaultFile($path."/game/moves_$i.txt", serialize($active_games), 0600);
    createDefaultFile($path."/game/messages_$i.txt", serialize($active_users), 0600);
  }
}


?>
