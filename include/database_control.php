<?php

function db_getInstance(){
        $path = $_SERVER["DOCUMENT_ROOT"]."/database/info.txt";
        $data = unserialize(file_get_contents($path));
        $db = new mysqli(
                $data["host"],
                $data["user"],
                $data["password"],
                $data["name"],
                $data["port"]
        );
        
        return $db;
}

function db_register($db_instance, $username, $pw_encrypted, $country){

        // generate query string;
        $query = 
        "INSERT INTO 
        `users`(`username`, `password`, `wins`, `losses`, `country`)
        VALUES(?, ?, '0', '0', ?)";
        
        $statement = $db_instance->prepare($query);
        $statement->bind_param("sss", $username, $pw_encrypted, $country);
        $result = $statement->execute();
        $statement->close();
        
        return $result;
}

function db_update($db_instance, $username, $win, $loss){
        
        $query = 
        "UPDATE 
        `users` 
        SET 
        `wins`=?,`losses`=? 
        WHERE 
        `username`=?";
        
        $statement = $db_instance->prepare($query);
        $statement->bind_param("iis", $query, $win, $loss, $username);
        $result = $statement->execute();
        $statement->close();
        
        return $result;
}

function db_select($db_instance, $username, &$data){

        $query = 
        "SELECT *
        FROM 
        `users`
        WHERE
        `username` = ?";
        
        $statement = $db_instance->prepare($query);
        $statement->bind_param("s", $username);
        $statement->bind_result($data[0], $data[1], $data[2], $data[3], $data[4]);
        $result = $statement->execute();
        
        if ($result) {
                return $statement->fetch();
        }

        $statement->close();
        return false;
}

function db_disconnect($db_instance){
        $db_instance->close();
}

function db_test(){

        $db_instance = db_getInstance();
        $result = db_select($db_instance, "fakename");
        $string = print_r($result->fetch_row(), true);
        print("<pre>$string</pre>");
        db_disconnect($db_instance);
}



//db_test();

?>