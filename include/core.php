<?php
// core server logic
$path = $_SERVER["DOCUMENT_ROOT"]."/data/";

function assign_authentication($user){
  // generates an authentication code to be
  // assigned to a client application.
  
  global $path;
  
  // lock the file for writing
  $file_ids = @fopen($path."user_id.txt", "r");
  
  if (!@flock($file_ids, LOCK_EX)) {
    return 0;
  }
  
  $ids = unserialize(file_get_contents($path."user_id.txt"));
  
  if (count($ids)) {
  
    $file_write = @fopen($path."user_id.tmp", "w+");
    
    if (!@flock($file_write, LOCK_EX)) {
      @flock($file_ids, LOCK_UN);
      return 0;
    }
    
    $val = $ids["max_capacity"] * random_int(1000000, 9999999);
    $user["id"] = array_shift($ids["available"]);
    $authentication_code = $user["id"] + $val;
    @fwrite($file_write, serialize($ids));
    fclose($file_write);
    
    copy($path."user_id.tmp", $path."user_id.txt");
    @flock($file_write, LOCK_UN);
    
  } else {
    $authentication_code = 0;
  }
  
  @flock($file_ids, LOCK_UN);
  
  if ($authentication_code) {
    // successful assignment, add user to active user list
    $user["location"] = 0;
    add_user(0, "$authentication_code", $user);
  }
  //print("$authentication_code<br>");
  
  return $authentication_code;
}

function remove_authentication($authentication_code){

  global $path;
  $user = authenticate($authentication_code);
  if ($user) {
  
    $file_ids = @fopen($path."user_id.txt", "r");
    $file_write = @fopen($path."user_id.tmp", "w+");
    
    if (@flock($file_ids, LOCK_EX) && @flock($file_write, LOCK_EX)) {
    
      $ids = unserialize(file_get_contents($path."user_id.txt"));
      array_push($ids["available"], $user["id"]);
      remove_user(0, $authentication_code);
      fwrite($file_write, serialize($ids));
      copy($path."user_id.tmp", $path."user_id.txt");
      
      flock($file_write, LOCK_UN);
      flock($file_ids, LOCK_UN);
      

      
    } else {
      @flock($file_write, LOCK_UN);
      @flock($file_ids, LOCK_UN);
      return false;
    }
    
    fclose($file_write);
    fclose($file_ids);
    return true;
  }
  
  return false;
}

function authenticate($authentication_code){
  // authenticate the user
  global $path;
  $file_users = @fopen($path."active_users.txt", "r");
  if (!@flock($file_users, LOCK_SH)) {
    return false;
  }
  $users = unserialize(file_get_contents($path."active_users.txt"));
  flock($file_users, LOCK_UN);
  fclose($file_users);
  
  if (array_key_exists($authentication_code, $users)){
    return $users[$authentication_code];
  } else {
    return false;
  }

}

function assign_game(){
  global $path;
  $file_ids = @fopen($path."game_id.txt", "r");
  if (!@flock($file_ids, LOCK_EX)) {
    return 0;
  }
  
  $ids = unserialize(file_get_contents($path."game_id.txt"));
  if (count($ids["available"])) {
  
    // allocates a new game id
    $game_id = array_shift($ids["available"]);
    
    $file_write = @fopen($path."game_id.tmp", "w+");
    @flock($file_write, LOCK_UN);
    
    fwrite($file_write, serialize($ids));
    copy($path."game_id.tmp", $path."game_id.txt");
    
    flock($file_write, LOCK_UN);
    fclose($file_write);
    flock($file_ids, LOCK_UN);
    fclose($file_ids);
    return $game_id;
    
  }
  
  flock($file_ids, LOCK_UN);
  fclose($file_ids);
  return 0;

}

function initialize_game(
  $loc,
  $hostname,
  $size,
  $host_color,
  $timer,
  $spectator_on
) {
  // adds game to the list of active games
  // initialize parameters.
  global $path;
  
  $arr = array(
    "status" => "Starting",
    "host" => $hostname,
    "opponent" => "",
    "size" => $size,
    "host_color" => $host_color,
    "timer" => $timer,
    "spectator_on" => intval($spectator_on)? "On" : "Off",
    "player_count" => 1,
    "score_black" => -1,
    "score_white" => -1,
    "time_black" => 0,
    "time_white" => 0,
  );
    
  if (!update_info($loc, $arr)) {
    echo("update_info failed");
    return false;
  }
    
  // reset games, don't need to worry about existing locks
  $file_move = @fopen($path."game/moves_$loc.tmp", "w+");
  fwrite($file_move, serialize(array()));
  copy($path."game/moves_$loc.tmp", $path."game/moves_$loc.txt");
  fclose($file_move);
    
  $file_message = @fopen($path."game/messages_$loc.tmp", "w+");
  fwrite($file_message, serialize(array()));
  copy($path."game/moves_$loc.tmp", $path."game/messages_$loc.txt");
  fclose($file_message);
  
  
  $file_games = @fopen($path."active_games.txt", "r");
  if (!@flock($file_games, LOCK_EX)) {
    return false;
  }
  $file_write = @fopen($path."active_games.tmp", "w+");
  if (!@flock($file_write, LOCK_EX)){
    @flock($file_games, LOCK_UN);
    return false;
  }
  
  $games = unserialize(file_get_contents($path."active_games.txt"));
  array_push($games, $loc);
  fwrite($file_write, serialize($games));
  copy($path."active_games.tmp", $path."active_games.txt");
  
  @flock($file_write, LOCK_UN);
  @flock($file_games, LOCK_UN);
  
  fclose($file_write);
  fclose($file_games);
    
  return true;  
}

function close_game($loc){
  global $path;
  
  $file_games = @fopen($path."active_games.txt", "r");
  $file_write = @fopen($path."active_games.tmp", "w+");
  
  if (!@flock($file_games, LOCK_EX)) {
    return false;
  }
  if (!@flock($file_write, LOCK_EX)) {
    flock($file_games, LOCK_UN);
    return false;
  }
  
  $games = unserialize(file_get_contents($path."active_games.txt"));
  $index = array_search($loc, $games);
  unset($games[$index]);
    
  fwrite($file_write, serialize($games));
  copy($path."active_games.tmp", $path."active_games.txt");
  
  @flock($file_write);
  @flock($file_games);
  
  fclose($file_write);
  fclose($file_games);
  
  $file_id = @fopen($path."game_id.txt", "r");
  $file_write = @fopen($path."game_id.tmp", "w+");
  
  if (!@flock($file_id, LOCK_EX)) {
    return false;
  }
  if (!@flock($file_write, LOCK_EX)) {
    flock($file_id, LOCK_UN);
    return false;
  }
  
  $ids = unserialize(file_get_contents($path."game_id.txt"));
  array_push($ids["available"], $loc);
    
  fwrite($file_write, serialize($ids));
  copy($path."game_id.tmp", $path."game_id.txt");
  
  @flock($file_write);
  @flock($file_id);
  
  fclose($file_write);
  fclose($file_id);
  
  return true;
}

function get_info($loc){
  // read only operation
  
  global $path;
  $filename = $path."game/info_$loc.txt";
  $file_info = @fopen($filename, "r");
  if (!@flock($file_info, LOCK_SH)) {
    return false;
  }
  
  $info = unserialize(file_get_contents($filename));
  @flock($file_info, LOCK_UN);
  fclose($file_info);
  
  return $info;
}


function update_info($loc, $array, $from = ""){
  // updates game info at $loc with key value pair
  //showme($array);
  global $path;
  $filename = $path."game/info_$loc.txt";
  $writename = $path."game/info_$loc.tmp";
  

  $file_info = @fopen($filename, "r");
  $file_write = @fopen($writename, "w+");
  
  if (!@flock($file_info, LOCK_EX)) {
    return false;
  }
  
  if (!@flock($file_write, LOCK_EX)) {
    flock($file_info, LOCK_UN);
    return false;
  }
  
  $data = unserialize(file_get_contents($filename));

  
  switch($from) {
    case "join":
      if ($data["host"] == "" && $data["opponent"] != "") {
        @flock($file_write, LOCK_UN);
        @flock($file_info, LOCK_UN);
  
        fclose($file_write);
        fclose($file_info);
        return false;
      }
      break;
      
    case "leave":
      if (array_key_exists("host", $array)) {
        $array["host"] = $data["opponent"];
      }
      break;
      
  }
  
  foreach ($array as $key=>$value) {
    $data[$key] = $value;
  }
  
  fwrite($file_write, serialize($data));
  copy($writename, $filename);
  
  @flock($file_write, LOCK_UN);
  @flock($file_info, LOCK_UN);
  
  fclose($file_write);
  fclose($file_info);
  
  return true;
}

function add_user($loc, $key, $value, $is_host = false){
  // puts a user at specificed location
  global $path;
  if ($loc == 0) {
    $filename = $path."active_users.txt";
    $writename = $path."active_users.tmp";
  } else {
    $filename = $path."game/players_$loc.txt";
    $writename = $path."game/players_$loc.tmp";
  }
  

  $file_users = @fopen($filename, "r");
  $file_write = @fopen($writename, "w+");
  
  if (!@flock($file_users, LOCK_EX)) {
    return false;
  }
  
  if (!@flock($file_write, LOCK_EX)) {
    flock($file_users, LOCK_UN);
    return false;
  }
  
  $data = unserialize(file_get_contents($filename));
  
  //showme($data);
  if ($loc > 0 && $is_host == false && count($data) == 0) {
    // if attemptint to add user to an empty game
    // return false unless the function is called by user_host.
    @flock($file_write, LOCK_UN);
    @flock($file_users, LOCK_UN);
    fclose($file_write);
    fclose($file_users);
    return false;
  }
  
  $data[$key] = $value;
  $player_count = count($data);
  //showme($data);
  fwrite($file_write, serialize($data));
  copy($writename, $filename);

  
  fclose($file_write);
  fclose($file_users);
  
  if ($loc > 0 && $player_count > 1) {
    update_info($lock, array("player_count"=>$player_count));
  }
  
  return true;
}

function remove_user($loc, $key){
  global $path;
  if ($loc == 0) {
    $filename = $path."active_users.txt";
    $writename = $path."active_users.tmp";
  } else {
    $filename = $path."game/players_$loc.txt";
    $writename = $path."game/players_$loc.tmp";
  }
  

  $file_users = @fopen($filename, "r");
  $file_write = @fopen($writename, "w+");
  
  
  if (!@flock($file_users, LOCK_EX)) {
    return false;
  }
  if (!@flock($file_write, LOCK_EX)) {
    flock($file_users, LOCK_UN);
    return false;
  }
  
  $data = unserialize(file_get_contents($filename));
  $array = array();
  
  if ($loc > 0 && $data[$key]["role"] != "spectator") {
    $array[$data[$key]["role"]] = "";
  }
  if ($data[$key]["role"] == "host") {
    foreach($data as $id=>$player) {
      if ($data[$id]["role"] == "opponent") {
        $data[$id]["role"] == "host";
      }
    }
  }
  
  unset($data[$key]);
  $player_count = count($data);
  $array["player_count"] = $player_count;
  
  fwrite($file_write, serialize($data));
  copy($writename, $filename);

  
  @flock($file_write, LOCK_UN);
  @flock($file_users, LOCK_UN);
  
  
  fclose($file_write);
  fclose($file_users);
  
  if ($loc) {
    if ($player_count) {
      // decrement player_count in info file
      update_info($loc, $array, "leave");
    } else {
      // closes game if last player leaves
      close_game($loc);
    }
  }
  
}

function change_location($authentication_code, $loc) {
  // changes the location of a player
  global $path;
  $file_users = @fopen($path."active_users.txt", "r");
  $file_write = @fopen($path."active_users.tmp", "w+");
  
  if (!@flock($file_users, LOCK_EX)) {
    return false;
  }
  
  if (!@flock($file_write, LOCK_EX)) {
    @flock($file_write, LOCK_EX);
    return false;
  }
  
  $users = unserialize(file_get_contents($path."active_users.txt"));
  $users["$authentication_code"]["location"] = $loc;
  fwrite($file_write, serialize($users));
  copy($path."active_users.tmp", $path."active_users.txt");
  
  @flock($file_write, LOCK_UN);
  @flock($file_users, LOCK_UN);
  
  
  fclose($file_users);
  fclose($file_write);
  
}

function post_action($loc, $action){
  global $path;
  
  $filename = $path."game/moves_$loc.txt";
  $writename = $path."game/moves_$loc.tmp";
  
  $file_moves = @fopen($filename, "r");
  $file_write = @fopen($writename, "w+");
  
  if (!@flock($file_moves, LOCK_EX)) {
    return false;
  }
  
  if (!@flock($file_write, LOCK_EX)) {
    flock($file_moves, LOCK_UN);
    return false;
  }
  
  $moves = unserialize(file_get_contents($filename));
  array_push($moves, $action);
  fwrite($file_write, serialize($moves));
  copy($writename, $filename);
  
  @flock($file_write);
  @flock($file_moves);
  
  fclose($file_write);
  fclose($file_moves);

  return true;
}

function post_message($loc, $message){
  global $path;
  if ($loc == 0) {
    $filename = $path."messages.txt";
    $writename = $path."messages.tmp";
  } else {
    $filename = $path."game/messages_$loc.txt";
    $writename = $path."game/messages_$loc.tmp";
  }

  $file_message = @fopen($filename, "r");
  $file_write = @fopen($writename, "w+");
  
  if (!@flock($file_message, LOCK_EX)) {
    return false;
  }
  
  if (!@flock($file_write, LOCK_EX)) {
    flock($file_message, LOCK_EX);
    return false;
  }
  
  $data = unserialize(file_get_contents($filename));
  array_push($data, $message);
  //showme($data);
  fwrite($file_write, serialize($data));
  copy($writename, $filename);

  @flock($file_write, LOCK_UN);
  @flock($file_message, LOCK_UN);
  
  fclose($file_write);
  fclose($file_message);
  return true;
}

function echo_lobby_info($stamp) {
  
  global $path;
  $sep = strpos($stamp, ":");
  $message_start = intval(substr($stamp, $sep + 1));
  
  $file_games = @fopen($path."active_games.txt", "r");
  @flock($file_games, LOCK_SH);
  $games = unserialize(file_get_contents($path."active_games.txt"));
  @flock($file_games, LOCK_UN);
  fclose($file_games);
  
  $game_list = "games=".count($games)."\0";
  foreach($games as $game_id) {
    $info = get_info($game_id);
    $game_list .= sprintf(
      "%d\r%d\r%s\r%s\r%s\r%s\r%s\r%s\r%s\r\0",
      //args
      $game_id,
      $info["player_count"],
      $info["host"],
      $info["opponent"],
      $info["size"],
      $info["host_color"],
      $info["timer"],
      $info["status"],
      $info["spectator_on"]
    );
  }
  
  
  $file_users = @fopen($path."active_users.txt", "r");
  @flock($file_users, LOCK_SH);
  $users = unserialize(file_get_contents($path."active_users.txt"));
  @flock($file_users, LOCK_UN);
  fclose($file_users);
  
  $user_count = 0;
  
  foreach($users as $user){
    if ($user["location"] == 0) {
      $user_count++;
      $user_list .= sprintf(
        "%s\r%d\r%d\r%s\r\0",
        $user["username"],
        $user["wins"],
        $user["losses"],
        $user["country"]
      );
    }
  }
  
  
  $file_message = @fopen($path."messages.txt", "r");
  @flock($file_message, LOCK_SH);
  $messages = unserialize(file_get_contents($path."messages.txt"));
  @flock($file_message, LOCK_UN);
  fclose($file_message);
  
  $new_messages = count($messages) - $message_start;
  $message_list = "messages=$new_messages\0";
  
  for ($i = 0; $i < $new_messages; $i++){
    $message_list .= $messages[$message_start + $i]."\0";
  }
  
  echo(sprintf("8=1\0stamp=0:%d\0", count($messages)));
  echo($game_list);
  echo("users=$user_count\0$user_list");
  echo($message_list);
  
  
}

function echo_game_info($loc, $stamp){
  
  global $path;
  $sep = strpos($stamp, ":");
  $move_start = intval(substr($stamp, 0, $sep));
  $message_start = intval(substr($stamp, $sep + 1));
  
  $info = get_info($loc);
  $control_info = sprintf(
    // [host]\r[opponent]\r[player_black]\r[player_white]\r\0
    "%s\r%s\r%s\r%s\r\0",
    $info["host"],
    $info["opponent"],
    $info["player_black"],
    $info["player_white"]
  );
  // I am not sure what to do with this yet. but doesn't hurt to
  // get the information incase I need it.
  
  
  // list of players must be provided
  $file_player = @fopen($path."game/players_$loc.txt", "r");
  @flock($file_player, LOCK_SH);
  $players = unserialize(file_get_contents($path."game/players_$loc.txt"));
  @flock($file_player, LOCK_UN);
  fclose($file_player);
  
  $player_list = "players=".count($players)."\0";
  foreach ($players as $player){
    $player_list .= sprintf(
      "%s\r%d\r%d\r%s\r\0",
      $player["username"],
      $player["wins"],
      $player["losses"],
      $player["country"]
    );
  }
  
  
  // list of moves must be provided
  $file_moves = @fopen($path."game/moves_$loc.txt", "r");
  @flock($file_moves, LOCK_SH);
  $moves = unserialize(file_get_contents($path."game/moves_$loc.txt"));
  @flock($file_moves, LOCK_UN);
  fclose($file_moves);
  
  $new_moves = count($moves) - $move_start;
  $move_list = "moves=$new_moves\0";
  for ($i = 0; $i < $new_moves; $i++) {
    $move_list .= $moves[$move_start + $i]."\0";
  }
  
  
  
  
  $file_message = @fopen($path."game/messages_$loc.txt", "r");
  @flock($file_message, LOCK_SH);
  $messages = unserialize(file_get_contents($path."game/messages_$loc.txt"));
  @flock($file_message, LOCK_UN);
  fclose($file_message);
  
  $new_messages = count($messages) - $message_start;
  $message_list = "messages=$new_messages\0";
  for ($i = 0; $i < $new_messages; $i++){
    $message_list .= $messages[$message_start + $i]."\0";
  }
  
  echo(sprintf("8=1\0stamp=%d:%d\0", count($moves), count($messages)));
  echo($control_info);
  echo($player_list);
  echo($move_list);
  echo($message_list);
  
}



function showme($var){
// test function
  print("<br><pre>");
  print_r($var);
  print("</pre><br>");
}
?>