<?php
include_once('./databases.php');
//header("Content-Type: text/html; charset=UTF-8");
//

?>
<html>
<head>
	<!-- <meta charset="utf-8" /> -->
	<title>HOMEPAGE</title>
	<style>
		.timecenter{ text-align: center; }
	</style>
	<META HTTP-EQUIV="refresh" CONTENT="0.97" charset="utf-8"> <!--10초마다 페이지 자동 갱신-->
	
</head>
<body>
	<h3 class="timecenter" width="100%" height="100px"> iframe창 <br/><br/><br/>
	<span><?php get_time(); ?></span>
	</h3>
</body>

</html>