<?php
// Replace this with the actual public key of the client
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, X-SIGNATURE");
header('Content-Type: application/json');
include "system-lib.php";

$data = json_decode(file_get_contents("php://input"), true);
$ok = auth_rsa($data["message"], ($data["signature"]));

var_dump($data);
echo ">>>   " . $data["user"] . ":::" ;

if ($ok === 1) {
    echo "Authenticated successfully!";
} elseif ($ok === 0) {
    echo "Invalid signature.";
} else {
    echo "Error: " . openssl_error_string();
}

?>
