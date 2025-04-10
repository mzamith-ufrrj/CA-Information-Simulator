<?php
  /*
   * Global variables used in all system
   */
//  $filename_users  = "/opt/php-db/users.md5";
//  $public_key_file = "/opt/keys/public_key.pem";
function auth_rsa(){
	$publicKeyPem = file_get_contents("/opt/keys/public_key-Model-SEIR-2048.pem");
	$data = json_decode(file_get_contents("php://input"), true);
	$message = $data["message"];
	$signature = base64_decode($data["signature"]);

	$ok = openssl_verify(
	    $message,
	    $signature,
	    $publicKeyPem,
	    OPENSSL_ALGO_SHA256
	);
	return $ok;

}



/*
   function authentic(){
        global $public_key_file; 
	if (!file_exists($public_key_file)) {
        	return[0,0];
    	}
	$publicKey = file_get_contents($public_key_file); // Server's public key
        //echo $publicKey . '<hr>';

	$headers = getallheaders();
	//var_dump($headers);
	$signature = base64_decode($headers['X-SIGNATURE'] ?? '');
	//echo "Debug x-signature: " . $headers["X-SIGNATURE"] . PHP_EOL;
        $payload = file_get_contents("php://input"); // The JSON payload sent by the client
        //echo "PAYLOAD: " . $payload . PHP_EOL;
        // Verify signature using public key
        $verification = openssl_verify($payload, $signature, $publicKey, OPENSSL_ALGO_SHA256);
        return [$verification, $payload];
        //return $verification;
   }

   function findUser($user){
      global $filename_users;
      $ret = FALSE;

      $fileHandle = fopen($filename_users, "r");
      while ((($line = fgets($fileHandle)) !== false) && (!$ret)){
	$aux = substr($line, 0, -1);
	echo "> " . $line . " == " . $user . PHP_EOL;
	echo "> " . ($line == $user) . PHP_EOL;
      	if ($aux == $user)
	   $ret = TRUE;
      }

      fclose($fileHandle);
      return $ret;
   }
 */
function httpNotFound(){
      http_response_code(404);
      header('Content-type: text/html');
      echo '<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">';
      echo '<html><head><title>404 Not Found</title></head><body>';
      echo '<h1>Not Found</h1>';
      echo '<p>The requested URL was not found on this server.</p> </body></html>';
      die("Page not found!");
}  
?>
