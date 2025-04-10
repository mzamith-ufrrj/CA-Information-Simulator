<?php
function getPublicKeyFileName($user_md5){
    $public_key_dir = "/opt/keys/";
    $public_key_sufix = "-pub_k-SEIR.pem";

    return $public_key_dir . $user_md5 . $public_key_sufix;

}    
function auth_rsa($message, $signature_encoded, $user){
	$user_md5 = md5($user);
        $public_key_file = getPublicKeyFileName($user_md5);
	$publicKeyPem = file_get_contents($public_key_file);
	$signature = base64_decode($signature_encoded);
	return openssl_verify($message,$signature,$publicKeyPem,OPENSSL_ALGO_SHA256);
}

?>
