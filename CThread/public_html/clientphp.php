#!/usr/bin/php -q
<?php
define("_IP", "10.0.0.95");
define("_PORT", "3301");

$sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($sock, _IP, _PORT);
msg("socket connect to "._IP.":"._PORT."\n");
$patterns = ['/\n/', '/\r/'];
while(1)
{
	msg("Enter command time or quit : ");
	
	// 사용자의 명령어를 입력받습니다.
	$stdin = preg_replace($patterns, "", read_data());
	$stdin = substr($stdin, 0, 4);
	
	// time 또는 quit 메시지 말고는 무시 합니다.
	if($stdin == "time" || $stdin == "quit")
	{
		msg("Input command : ".$stdin."\n");
	}
	else
	{
		msg("invalid command (not send) : ".$stdin."\n");
		continue;
	}

	socket_write($sock, $stdin);
	$sMsg = socket_read($sock, 4096);
	if(substr($sMsg, 0, 4) == 'quit')
	{
		socket_close($sock);
		exit;
	}
	else
	{
		msg("recived data : ".$sMsg."\n");
	}
}

// 표준입력을 받아 값을 리턴하는 함수
function read_data()
{
	$in = fopen("php://stdin", "r");
	$in_string = fgets($in, 255); fclose($in);
	return $in_string;
}
// 로그를 출력합니다. 디버그용
function msg($msg)
{
	echo "CLIENT >> ".$msg;
}
?>