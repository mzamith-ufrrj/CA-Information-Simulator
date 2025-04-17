<?php
// Replace this with the actual public key of the client
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, X-SIGNATURE");
header("Content-Type: text/html");
//header('Content-Type: application/json');
include "system-lib.php";

$data = json_decode(file_get_contents("php://input"), true);
//var_dump($data);
$ok = auth_rsa($data["message"], $data["signature"], $data["user"]);
$is_auth = ($ok === 1);
if (!$is_auth){
	$json_msg_out = [
	  "status" => "ERROR auth",
	  "data" => "???"
	];
	echo json_encode($json_msg_out);
	exit;
} 
$params = $data["params"];
//var_dump($params);
//echo $params["simulationName"];

$simulation_id = uniqid("SEIR-S-", true) ;
$nodes = $params["numNodes"];
$power = $params["power"];
$min_c = $params["minConnected"];
$json_params = [
    "simulation-name" => $simulation_id,
    "instance" => "filename.net",
    "verbose" => $params["verbose"],
    "cell-x" => $params["cellsX"],
    "cell-y" => $params["cellsY"],
    "infected" => $params["infected"],
    "timesteps" => $params["timesteps"],
    "infected_init_order" => $params["infectedOrder"],
    "alpha-rumor-mi" => $params["alphaMi"],
    "alpha-rumor-sig" => $params["alphaSig"],
    "gamma-rumor-mi" => $params["gammaMi"],
    "gamma-rumor-sig" => $params["gammaSig"]

];

$string_config_file = [
	"global-params" => $json_params
];
// Create json config file on webserver
$json_config_file = json_encode($string_config_file, JSON_PRETTY_PRINT);
$filename = "config-" . $simulation_id . ".json";
$local_filename_send = sys_get_temp_dir() . DIRECTORY_SEPARATOR . $filename;
$remote_filename_send = getRemoteDir() . $filename ;


// Save to file
$status_save = file_put_contents($local_filename_send, $json_config_file);
//SSH COPY
$connection = ssh2_connect(getServerHostname(), 22);
if (!$connection) {
    $json_msg_out = [
        "status" => "OK auth",
        "status_tmp_json_w" => $status_save,
        "local_file_send" => $local_filename_send,
        "status_ssh" => "SSH CONNECTION ERROR",
        "user" => md5($data["user"])
    ];
    echo json_encode($json_msg_out);
    return;
}

if (!ssh2_auth_password($connection, getServerUser(), getServerPass())) {
    $json_msg_out = [
        "status" => "ERROR ssh_auth",
        "status_tmp_json_w" => $status_save,
        "local_file_send" => $local_filename_send,
        "status_ssh" => "SSH AUTHENTICATION ERROR",
        "user" => md5($data["user"])
    ];
    echo json_encode($json_msg_out);
    return;
}

if (!ssh2_scp_send($connection, $local_filename_send, $remote_filename_send)){
    $json_msg_out = [
        "status" => "ERROR ssh_send",
        "status_tmp_json_w" => $status_save,
        "local_file_send" => $local_filename_send,
        "status_ssh2_send" => "SSH SCP ERROR",
        "user" => md5($data["user"])
    ];
    echo json_encode($json_msg_out);
    return;
}

$str_call = getRemoteExec() . $nodes . " " . $power . " " . $min_c ." " . $filename . " " . $simulation_id;
$stream = ssh2_exec($connection, $str_call);
stream_set_blocking($stream, true);
$output = stream_get_contents($stream);
fclose($stream);

//Remote to webserver copy
$remote_filename_recv = getRemoteDir() . $simulation_id . DIRECTORY_SEPARATOR . "statistic." . $simulation_id . ".csv" ;
$local_filename_recv = sys_get_temp_dir() . DIRECTORY_SEPARATOR . "statistic." . $simulation_id . ".csv" ;;

if (!ssh2_scp_recv($connection, $remote_filename_recv, $local_filename_recv)){
    $json_msg_out = [
        "status" => "ERROR ssh_recv",
        "status_tmp_json_w" => $status_save,
        "local_file_send" => $local_filename_send,
        "remote_file_send" => $remote_filename_send,
        "status_ssh2_send" => "SSH OK",
        "status_ssh2" => $str_call, 
        "local_file_recv" => $local_filename_recv,
        "remove_file_recv" => $remote_filename_recv,
        "status_ssh2_recv" => "SCP RECV ERROR",
        "user" => md5($data["user"])
    ];

    echo json_encode($json_msg_out);
    return;
}

$str_csv = file_get_contents($local_filename_recv);
$json_msg_out = [
        "status" => "OK simulation",
        "status_tmp_json_w" => $status_save,
        "local_file_send" => $local_filename_send,
        "remote_file_send" => $remote_filename_send,
        "status_ssh2_send" => "SSH OK",
        "status_ssh2" => $str_call, 
        "local_file_recv" => $local_filename_recv,
        "remove_file_recv" => $remote_filename_recv,
	"status_ssh2_recv" => "SSH OK",
	"data_simulation" => $str_csv,
        "user" => md5($data["user"])
    ];

echo json_encode($json_msg_out);
?>
