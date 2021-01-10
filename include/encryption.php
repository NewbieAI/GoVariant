<?php
// placeholder encoding and decoding algorithm.
// this will be completed after all other features
// are working properly.

function encode($message){
        return "###$message";
}

function decode($encoded){
        return substr($encoded, 3);
}


?>