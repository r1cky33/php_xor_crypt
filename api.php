<?php
/**
 *    DB-Access
 */
$config['mysql_hostname'] = 'localhost';
$config['mysql_username'] = 'root';
$config['sql_password'] = '';
$config['mysql_database'] = 'core_members';

include_once $_SERVER["DOCUMENT_ROOT"]."/api/helper.php"; //helper file
//include_once $_SERVER["DOCUMENT_ROOT"]."/plugins/sql.php"; //sQL-MaNAgEr

//mysql_conn
$link = mysqli_connect($config['mysql_hostname'], $config['mysql_username'], "", $config['mysql_database'] );

if (!$link) {
    echo "Error: Unable to connect to MySQL." . PHP_EOL;
    exit;
}

//param shit
if(isset($_GET['type']) && isset($_GET['id']) && isset($_GET['pw'])){
    $type = $_GET['type'];
    $id = $_GET['id'];
    $pw = $_GET['pw'];

    if($type == "1"){
        //mysql_query
        $query = $link->prepare("SELECT * FROM users WHERE name = ?");
        $query->bind_param("s", $_GET['id']);
        $query->execute();

        $result = $query->get_result();
        if(!$result) exit('No rows');

        //handle query
        if($row = $result->fetch_assoc()){
            //echo "name: " . $row["name"] . " password: " . $row["password"] . "<br>";

            if(password_verify($pw, $row['password'])){
                $hex = get_file_hex("sample.dll");
                $crypted = xorEncrypt($hex, "mysecretkey");
                exit(bin2hex($crypted));
            }
        }
    }
}
else{
    exit("No credentials");
}

//cleanup
$query->close();
mysqli_close($link);
exit();
?>