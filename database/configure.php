<?php

$data = array();
$data["name"] = "3201083_userinfo";
$data["user"] = "3201083_userinfo";
$data["password"] = "eNywlA2uiDNJe5K4G8ShyTPVaC8jdak4";
$data["host"] = "pdb42.awardspace.net";
$data["port"] = 3306;


print("executing configure.php");
print("<pre>".print_r($data, true)."</pre>");


$file = fopen(__dir__."/info.txt", "w+");
$bytes = fwrite($file, serialize($data));
print("$bytes bytes written");
fclose($file);


?>