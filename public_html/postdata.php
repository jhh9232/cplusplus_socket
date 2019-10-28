<?php
// if(isset($_POST))
// {
// 	foreach($_POST as $key => $value)
// 	{
// 		echo "key : ".$key.", value : ".$value."<br/>";
// 	}
// }

include_once('./databases.php');
include_once('./showhtml.php');
$data = json_decode(file_get_contents('php://input'), true);
if(isset($data))
{
	print_r($data);
	foreach($data as $dvalue)
	{
		if(insert_data($dvalue))
		{
			echo $dvalue["timestamp"];
		}
	}
}
else
{
	loadHTML();
}

?>