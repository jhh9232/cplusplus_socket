<?php

// CREATE TABLE savestr (
// seqid integer PRIMARY KEY,
// threadID varchar NOT NULL,
// randstr varchar NOT NULL,
// savetime timestamp NOT NULL
// );

try{
    $host = "127.0.0.1";
    $db = "dbrand";
    $user = "postgres";
    $pass = "1234asdf";
    $pdo = new PDO("pgsql:host=$host;dbname=$db", $user, $pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}catch(Exception $e){
    echo $e->getMessage();
}

function get_time()
{
	global $pdo;
	$query = "SELECT to_char(CURRENT_TIMESTAMP, 'YYYY-MM-DD HH24:MI:SS')";
	$stat = $pdo->prepare($query);
	$stat->execute();
	$result = $stat->fetchAll(PDO::FETCH_NUM);
	echo $result[0][0];
}

function insert_data($data)
{
	global $pdo;
	try
	{
		//insert into test(testtime) values(to_timestamp('2009-01-03 12:33:33');
		$query = "insert into savestr (seqid, threadID, randstr, savetime) values (?, ?, ?, TO_TIMESTAMP(?, 'YYYY-MM-DD HH24:MI:SS'))";
		$stat = $pdo->prepare($query);
		$stat->execute(array($data["seqid"], $data["threadID"], $data["randomstr"], $data["timestamp"]));
		$result = $stat->fetchAll(PDO::FETCH_NUM);
		return true;
		//insert into savestr (seqid, threadID, randstr, savetime) values (1, '1012343354', 'AsVDEwRT', TO_TIMESTAMP('2009-01-03 12:33:33', 'YYYY-MM-DD HH:MI:SS'));
	}
	catch(Exception $e)
	{
		echo $e->getMessage();
		return false;
	}
}

function count_data()
{
	global $pdo;
	$query = "SELECT count(*) FROM savestr";
	$stat = $pdo->prepare($query);
	$stat->execute();
	$result = $stat->fetchAll(PDO::FETCH_NUM);
	return $result;
}

function select_data()
{
	global $pdo;
	$size = func_num_args();
	try
	{
		if($size == 0)
		{
			$query = "SELECT * FROM savestr order by seqid LIMIT 10 OFFSET 0";
			$stat = $pdo->prepare($query);
			$stat->execute();
			$result = $stat->fetchAll(PDO::FETCH_NUM);
			return $result;
		}
		else if($size == 1)
		{
			$offset = (func_get_arg(0)-1)*10;
			$query = "SELECT * FROM savestr order by seqid LIMIT 10 OFFSET ?";
			$stat = $pdo->prepare($query);
			$stat->execute(array($offset));
			$result = $stat->fetchAll(PDO::FETCH_NUM);
			return $result;
		}
		else
		{
			return false;
		}
	}
	catch(Exception $e)
	{
		echo $e->getMessage();
		return false;
	}
}

?>