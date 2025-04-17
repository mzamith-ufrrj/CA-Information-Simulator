<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, X-SIGNATURE");
header('Content-Type: application/json');
include "system-lib.php";

list($verification, $payload) = authentic();
//------------------------------------------------------------------------
$json_vars = json_decode($payload, true);
$user_md5 = $json_vars["user"] ;
$dataset = [
    ["id" => 1, "name" => "Alice", "age" => 25],
    ["id" => 2, "name" => "Bob", "age" => 30],
    ["id" => 3, "name" => "Carol", "age" => 22],
];
$output = [
    "verification" => $verification,
    "user" => $user_md5,
    "data" => $dataset
];

echo json_encode($output);

//------------------------------------------------------------------------
//echo "Marcelo Zamith" . PHP_EOL;
//    echo $verification . '<hr>';
//    if (!$verification == 1){
	    //httpNotFound();
	    //
 //     echo '[NOT AUTHENTICATED]' . PHP_EOL;
//    }else{
//      echo '[ok]' . PHP_EOL;
//    }
//    echo $user_md5 . '<hr>' . $pass_md5 . '<hr><hr>';
//    $json_var = json_decode($payload, true);
//    var_dump($json_var);
//    $user_md5 = $json_var['user'];
//    if (!findUser($user_md5)){
//	    echo 'Invalid user !' . PHP_EOL;
	
//    }
//    $pass_md5 = $json_var['passwd'];
//    echo $user_md5 .  PHP_EOL ;
?>
