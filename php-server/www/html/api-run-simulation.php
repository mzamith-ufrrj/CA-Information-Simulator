<?php
// Replace this with the actual public key of the client
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, X-SIGNATURE");
header("Content-Type: text/html");
//header('Content-Type: application/json');
include "system-lib.php";

$data = json_decode(file_get_contents("php://input"), true);
$ok = auth_rsa($data["message"], $data["signature"], $data["user"]);
$is_auth = ($ok === 1);
/*if ($ok === 1) {
    echo "Authenticated successfully!";
} elseif ($ok === 0) {
    echo "Invalid signature.";
} else {
    echo "Error: " . openssl_error_string();
}*/
//echo 'by Marcelo Zamith - all your bases are belong to us!' . PHP_EOL;
if (!$is_auth){
	$json_msg_out = [
	  "status" => "auth_error",
	  "data" => "???"
	];
	echo json_encode($json_msg_out);
	exit;
} 
$json_msg_out = [
   "status" => "auth_ok",
   "data" => "Marcelo",
   "user" => md5($data["user"])
];
//echo $data["user"];
//echo md5($data["user"]);
echo json_encode($json_msg_out);
?>
