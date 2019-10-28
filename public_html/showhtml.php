<?php
function loadHTML()
{?>


<html>
<head>
	<meta charset="utf=8"/>
	<title>iframetest</title>
	<script src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
	<style>
		body{ text-align: center; }
	</style>
</head>
<body>
	<h1>MONITORAPP RANDOM STRING CREATER</h1>
	<h3><a href="javascript:reloadiframe()">refresh!</a></h3>
	<iframe src="showtable.php", id = "tableframe" class="myFrame" scrolling="no" frameborder=0 framespacing=0 marginheight=0 marginwidth=0 scrolling=no vspace=0></iframe>
	<!-- <iframe src="pgtest.php" class="myFrame" scrolling="no" onLoad="setIFrameHeight(this)"></iframe> -->
	<script type="text/javascript">
	$('.myFrame').on('load', function(){ //iframe 컨텐츠가 로드 된 후에 호출됩니다.
		var obj = $(this).get(0);
		if(obj.width == "")
		{
			//obj.width = obj.contentDocument.body.scrollWidth + 30;
			obj.width = 850;
			obj.height = obj.contentDocument.body.scrollHeight + 30;
			$('.myFrame').attr('class','loadFrame');
		}
	});
	function reloadiframe()
	{
		document.getElementById("tableframe").contentDocument.location.reload(true);
	}
	</script>

</body>
</html>


<?php } ?>