<?php

require_once("database_control.php");
require_once("core.php");
require_once("encryption.php");



function user_register($username, $password, $country){
  // encrypt username and password and store them
  // in the database, returns a boolean.
  $db_instance = db_getInstance();
  
  if ($db_instance->connect_error) {
    return false;
  }

  $pass_encrypted = password_hash($password, PASSWORD_DEFAULT);
  $result = db_register($db_instance, $username, $pass_encrypted, $country);
  
  if ($result){
    echo("0=1\0");
  } else {
    echo("0=0\0");
  }
       
} //OK

function user_login($username, $password){
  // match the received account login
  // information to stored info in the
  // database.
  // returns an boolean
  $db_instance = db_getInstance();
  
  if ($db_instance->connect_error) {
    return false;
  }
  
  $data = array(
    0=>"",
    1=>"",
    2=>0,
    3=>0,
    4=>"",
  );
  
  $result = db_select($db_instance, $username, $data);

  if (!$result || !password_verify($password, $data[1])){
    echo("1=0\0");
    return;
  }
  
  // add user to lobby, assign an authentication code, respond with information about the lobby
  $user = array(
    "username"=>$data[0],
    "wins"=>$data[2],
    "losses"=>$data[3],
    "country"=>$data[4],
  );
  
  $authentication_code = assign_authentication($user);
  if ($authentication_code) {
    echo("1=1\0authentication_code=$authentication_code\0");
    user_update($authentication_code, "0:0");
  } else {
    echo("1=0\0");
  }
  
}


function user_logout($authentication_code){
  // removes user from authenticated list
  // remove user from lobby/game
  if (remove_authentication($authentication_code)){
    echo("2=1\0");

  } else {
    echo("2=0\0");
  }
  
  
}


function user_host(
  $authentication_code, 
  $size,
  $host_color,
  $timer, 
  $spectator_on){
  
  // creates a game in the lobby
  // initialized with settings
  // that the host has requested.
  $user = authenticate($authentication_code);
  if ($user) {
    // $user can
    $game_id = assign_game();
    if ($game_id) {
      // a game maybe created
      
      add_user(
        $game_id,
        $user["id"],
        array(
          "username"=>$user["username"],
          "wins"=>$user["wins"],
          "losses"=>$user["losses"],
          "country"=>$user["country"],
          "role"=>"host",
        ),
        true
      );
      
      change_location($authentication_code, $game_id);
      
      initialize_game(
        $game_id,
        $user["username"],
        $size,
        $host_color,
        $timer,
        $spectator_on
      );

      echo ("3=1\0");
      user_update($authentication_code, "0:0");
      return;
      
    }
  
  }
  echo("3=0\0");

}

function user_join($authentication_code, $game_id){

  // adds a player to the game as the opponent to the host
  $user = authenticate($authentication_code);
  if ($user) {
    //check if there's a conflict
    
    if (update_info($game_id, array("opponent"=>$user["username"]), "join")) {
      if ($user["location"] == 0) {
      
        $join_success = add_user(
          $game_id,
          $user["id"],
          array(
            "username"=>$user["username"],
            "wins"=>$user["wins"],
            "losses"=>$user["losses"],
            "country"=>$user["country"],
            "role"=>"opponent",
          )
        );
        
        if (!$join_success) {
          // game is closed.
          echo("4=0\0");
          return;
        }
        
      }
      
      change_location($authentication_code, $game_id);
      echo("4=1\0");
      user_update($authentication_code, "0:0");
      return;
    } 
    
  }
  echo("4=0\0");
}

function user_spectate($authentication_code, $game_id){
  $user = authenticate($authentication_code);
  if ($user) {
  
    if ($user["location"] == 0) {
      
      // join game from lobby. if game is closed, return failure.
      $spectate_success = add_user(
        $game_id,
        $user["id"],
        array(
          "username"=>$user["username"],
          "wins"=>$user["wins"],
          "losses"=>$user["losses"],
          "country"=>$user["country"],
          "role"=>"spectator",
        )
      );
        
      if (!$spectate_success) {
        echo("5=0\0");
        return;
      }
      
      change_location($authentication_code, $game_id);
      
        
    } else {
      // current player goes into spectator mode
      update_info($game_id, array("opponent"=>""));
    }
    
    echo("5=1\0");
    user_update($authentication_code, "0:0");
    return;
      
  }
  echo("5=0\0");

}

function user_leave($authentication_code){
  // removes user from the current game
  // if there is no other user, close the game
  $user = authenticate($authentication_code);
  if ($user) {

    remove_user($user["location"], $user["id"]);
    change_location($authentication_code, 0);
    
    echo("6=1\0");
    user_update($authentication_code, "0:0");
    
  } else {
  
    echo("6=0\0");
    
  }

}

function user_play($authentication_code, $action, $stamp){
  // update the game file with the most recent move
  $user = authenticate($authentication_code);
  if ($user) {
    
    post_action($user["location"], $action);
    
    echo("9=1\0");
    user_update($authentication_code, $stamp);
  } else {
    echo("9=0\0");
  }
}

function user_message($authentication_code, $message, $stamp){
  // update the message file with the most recent information
  $user = authenticate($authentication_code);
  if ($user) {
    
    post_message($user["location"], $message);
    
    echo("10=1\0");
    user_update($authentication_code, $stamp);
  } else {
    echo("10=0\0");
  }
}

function user_score($authentication_code, $score){
  // update the players score estimate, if the score estimates
  // agree, a winner is declared.
  
  // a poor design choice was made, the function has responsibilities
  // that should have been delegated to other functions.
  
  $user = authenticate($authentication_code);
  if ($user) {
    //temp_log($user["username"]. " submits a score of $score");
    
    $info = get_info($user["location"]);
    switch($score) {
    case -1: // black unreadies
      if ($info["status"] != "started") {
        post_message($user["location"], "b".$user["username"]." cancels.");
        $info["score_black"] = -1;
      } else {
        echo("11=0\0"); // game already started, cannot cancel
        return;
      }
      break;
      
    case -2: // black readies
      $info["score_black"] = 361 * 362;
      post_message($user["location"], "b".$user["username"]." is ready.");
      if ($info["score_white"] == 361) {
        $info["status"] = "started";
        if ($info["host_color"] == "random") {
        
          if (random_int(0, 1) == 0) {
            $info["player_black"] = $user["username"];
            $info["player_white"] = ($user["username"] == $info["host"] ? $info["opponent"] : $info["host"]);
          } else {
            $info["player_white"] = $user["username"];
            $info["player_black"] = ($user["username"] == $info["host"] ? $info["opponent"] : $info["host"]);
          }
        
        } else {
        
          if ($info["host_color"] == "black") {
            $info["player_black"] = $info["host"];
            $info["player_white"] = $info["opponent"];
          } else {
            $info["player_white"] = $info["host"];
            $info["player_black"] = $info["opponent"];
          }
        
        }
        
      }
      break;
      
    case -3: // white unreadies
      if ($info["status"] != "started") {
        post_message($user["location"], "b".$user["username"]." cancels.");
        $info["score_white"] = -1;
      } else {
        echo("11=0\0"); // game already started, cannot cancel
        return;
      }
      break;
      
    case -4: // white readies
      $info["score_white"] = 361;
      post_message($user["location"], "b".$user["username"]." is ready.");
      if ($info["score_black"] == 361 * 362) {
        $info["status"] = "started";
        if ($info["host_color"] == "random") {
        
          if (random_int(0, 1) == 0) {
            $info["player_black"] = $user["username"];
            $info["player_white"] = ($user["username"] == $info["host"] ? $info["opponent"] : $info["host"]);
          } else {
            $info["player_white"] = $user["username"];
            $info["player_black"] = ($user["username"] == $info["host"] ? $info["opponent"] : $info["host"]);
          }
          
        } else {
        
          if ($info["host_color"] == "black") {
            $info["player_black"] = $info["host"];
            $info["player_white"] = $info["opponent"];
          } else {
            $info["player_white"] = $info["host"];
            $info["player_black"] = $info["opponent"];
          }
          
        }
        
      }
      break;
      
    case -5: // black resigns
      if ($info["status"] == "started") {
        $info["status"] = "finished";
        $info["score_black"] = -1;
      }
      break;
      
    case -6: // white resigns
      if ($info["status"] == "started") {
        $info["status"] = "finished";
        $info["score_white"] = -1;
      }
      break;
      
    default:
      if ($user["id"] == $info["player_black"]) {
        $info["score_black"] = $score;
        
      } else {
        $info["score_white"] = $score;

      }
      
      post_message(
        $user["location"],
        sprintf("b%s submitted a score of %d[b] : %d[w]",
          $user["username"],
          floor($score / 362),
          $score % 362
        )
      );
      
      // if scores agree, set the game to finish
      $diff1 = $info["score_black"] / 
      if () {
        $info["status"] = "finished";
      
      }
      
    }
    
    if (update_info($user["location"], $info, "score")) {
      if ($info["status"] == "started" && $score >= -4) {
        // if the current score request started the game
        // notify users that the game is started
        post_action($user["location"], -1);
        post_message($user["location"], "gGame Started");
        
      }
      if ($info["status"] == "finished") {
        // if game successfully finished, update the statistics
      
      }
      echo("11=1\0");
    } else {
      echo("11=0\0");
    }
    
  } else {
    echo("11=0\0");
  }
}




function user_update($authentication_code, $stamp = "0:0"){
  // provide updated information to a user represented by the
  // authentication code.
  
  // provide information on all events that happened after the last
  // update response to avoid re-transmitting information tha was
  // already sent.
  $user = authenticate($authentication_code);
  if ($user){
    //showme($users["allocated_ids"][$authentication_code]);
    
    if ($user["location"] == 0) {
      // return inforamtion about the lobby
      echo_lobby_info($stamp, $stamp);
    } else {
      // return information about the game
      echo_game_info($user["location"], $stamp);
    }
    
    
  } else {
    echo("8=0\0");
  }
  
}


function handle_post_request($post){
  switch ($post["type"]) {
    case "register":
      user_register($post["username"], $post["password"], $post["country"]);
      break;
    case "login":
      user_login($post["username"], $post["password"]);
      break;
    case "logout":
      user_logout($post["authentication_code"]);
      break;
    case "host":
      user_host(
        $post["authentication_code"], 
        $post["size"], 
        $post["host_color"],
        $post["timer"],
        $post["spectator_on"]
      );
      break;
    case "join":
      user_join($post["authentication_code"], intval($post["game_id"]));
      break;
    case "spectate":
      user_spectate($post["authentication_code"], intval($post["game_id"]));
      break;
    case "leave":
      user_leave($post["authentication_code"]);
      break;
    case "update":
      user_update($post["authentication_code"], $post["stamp"]);
      break;
    case "play":
      user_play($post["authentication_code"], $post["action"], $post["stamp"]);
      break;
    case "message":
      user_message($post["authentication_code"], $post["message"], $post["stamp"]);
      break;
    case "score":
      user_score($post["authentication_code"], $post["score"]);
      break;
    case "renew":
      echo("12=1\0");
      break;
  }      
}

function temp_log($string){
  $file = fopen("server_temp_log.txt", "a");
  fwrite($file, $string."\r\n");
  fclose($file);
}

?>