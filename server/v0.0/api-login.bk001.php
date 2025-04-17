<?php
include "system-lib.php";

    list($verification, $payload) = authentic();
//    echo $verification . '<hr>';
    if (!$verification == 1){
      httpNotFound();
    }
    echo '[ok]' . PHP_EOL;
//    echo $user_md5 . '<hr>' . $pass_md5 . '<hr><hr>';
    $json_var = json_decode($payload, true);
//    var_dump($json_var);
    $user_md5 = $json_var['user'];
    if (!findUser($user_md5)){
	    echo 'Invalid user !' . PHP_EOL;
	
    }
//    $pass_md5 = $json_var['passwd'];
    echo $user_md5 .  PHP_EOL ;
?>
