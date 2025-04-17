<?php
function getServerHostname()                {  return "invaders"; }
function getServerUser()                    {  return "seir";}
function getServerPass()                    {  return "seir";}
function getKeyDir()                        {  return "../keys/"; }
function getKeySufix()                      {  return "-pub_k-SEIR.pem"; }
function getRemoteDir()                     {  return "/home/seir/SEIR-simulator/"; }
function getRemoteExec()                    {  return getRemoteDir() . "script-SEIR.sh "; }
function getPublicKeyFileName($user_md5)    {  return getKeyDir() . $user_md5 . getKeySufix(); }    

function auth_rsa($message, $signature_encoded, $user){
	$user_md5 = md5($user);
        $public_key_file = getPublicKeyFileName($user_md5);
	$publicKeyPem = file_get_contents($public_key_file);
	$signature = base64_decode($signature_encoded);
	return openssl_verify($message,$signature,$publicKeyPem,OPENSSL_ALGO_SHA256);
}

?>
