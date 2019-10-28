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
	<h1>iframe start</h1>
	<iframe src="pgtest.php" class="myFrame" scrolling="no" frameborder=0 framespacing=0 marginheight=0 marginwidth=0 scrolling=no vspace=0></iframe>
	<!-- <iframe src="pgtest.php" class="myFrame" scrolling="no" onLoad="setIFrameHeight(this)"></iframe> -->
	<script type="text/javascript">
	$('.myFrame').on('load', function(){ //iframe 컨텐츠가 로드 된 후에 호출됩니다.
		var obj = $(this).get(0);
		if(obj.width == "")
		{
			obj.width = obj.contentDocument.body.scrollWidth + 30;
			obj.height = obj.contentDocument.body.scrollHeight + 30;
			$('.myFrame').attr('class','loadFrame');
		}
	});


	</script>

	<!--
	inline frame의 크기 지정
	width = "60%" height = "500px"
	
	inline frame의 두께 지정
	frameborder="0"
	
	inline frame의 여백 지정
	marginwidth="0" marginheight="0"
	
	inline frame내에서 스크롤바 사용 지정
	scrolling="yes"
	scrolling="no" 지정 시 스크롤바 생성X

	-->
	<h1>iframe end</h1>
</body>
</html>