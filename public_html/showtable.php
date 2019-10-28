<?php
include_once('./databases.php');
?>

<html>
<head>
	<link rel="stylesheet" href="css/table.css">
	<link rel="stylesheet" href="css/a.css">
	<META HTTP-EQUIV="refresh" CONTENT="30" charset="utf-8">
</head>
<body>
	<table class="altrowstable" id="alternatecolor">
	<?php
	$pagenum = isset($_GET['page']) ? $_GET['page'] : 1;
	$result = select_data($pagenum);
	$resCount = count_data()[0][0];
	$len = count($result);
	if($len <= 0)
	{
		echo "<h1>검색 결과가 존재하지 않습니다.</h1>";
		return;
	}
	?>
	<thead>
		<th>Seq ID</th>
		<th>Thread ID</th>
		<th>Random String</th>
		<th>Save Time</th>
	</thead>
	<tbody>
		<?php
		for($i = 0; $i < $len; $i++)
		{
			?>
			<tr>
				<td width=150><?php echo $result[$i][0]?></td>
				<td width=150><?php echo $result[$i][1]?></td>
				<td width=150><?php echo $result[$i][2]?></td>
				<td width=150><?php echo $result[$i][3]?></td>
			</tr>
			<?php
		}
		?>
	</tbody>
	</table>
	<div>
	<?php
	$getURL = "/showtable.php?";
	$list = 10; //페이지에 보여 줄 리스트 개수
	$b_pagenum_list = 10;   //블럭에 나타날 페이지 번호 개수
	$block = ceil($pagenum/$b_pagenum_list); //현재 리스트의 블럭을 구함
	$b_start_page = ( ($block - 1) * $b_pagenum_list ) + 1; //현재 블럭에서 시작페이지 번호
	$b_end_page = $b_start_page + $b_pagenum_list - 1; //현재 블럭에서 마지막 페이지 번호
	$total_page = ceil( $resCount / $list );
	if ($b_end_page > $total_page){
		$b_end_page = $total_page;
	}

	//페이지번호가 1보다 작거나 같다면 링크없이 문자만 출력
	if($pagenum <= 1){?> 
	<font size=2  color=red>처음</font>
	
	<?php
	} //1보다 크다면 링크 걸린 채로 출력
	else{ ?> 
		<font size=2 ><a class="ablue" href="<?=$getURL?>page=1">처음</a></font>
	<?php
	} //block이 1보다 작거나 같으면 표시X
	if($block <=1){
	?>
		<font> </font>
	<?php
	}
	else{ //block이 1보다 크다면 이전 링크를 보여줌
	?>
		<font size=2><a class="ablue" href="<?=$getURL?>page=<?=$b_start_page-1?>">이전</a></font>
	<?php
	}
	
	for($j = $b_start_page; $j <=$b_end_page; $j++)
	{
		if($pagenum == $j) //pagenum 와 j 가 같으면 현재 페이지 이므로 링크걸지 않고 그냥 현재페이지만 출력
		{?>
			<font size=2 class = "askin ared on"><?=$j?></font>
		
		<?php
		}
		else{ //서로 다르다면 현재 페이지를 제외한 나머지 페이지번호를 링크를 달아 출력하기
		?>
			<font size=2><a class="askin ablue" href="<?=$getURL?>page=<?=$j?>"><?=$j?></a></font>
		<?php
		}
	}
	if($block >= $total_block) //block 과 총 block 갯수와 값이 같다면 다음링크버튼이 필요없어서 보여주지 않는다
	{ ?>
		<font> </font>
	<?php
	}
	else{ //그게 아니라면 다음 링크버튼을 걸어 보여줌
	?>
		<font size=2><a class = "askin ablue" href="<?=$getURL?>page=<?=$b_end_page+1?>">다음</a></font>
	<?php
	}
	if($pagenum >= $total_page)//페이지 넘버와 총 페이지가 같다면 마지막 페이지이므로 링크없이 마지막만 빨간색으로 출력
	{?>
		<font size=2  color=red>마지막</font>
	<?php
	}
	else{ // 그게 아니라면 페이지변수를 total_page로 주어서 마지막으로 가는 링크버튼을 출력
	?>
		<font size=2><a class = "ablue" href="<?=$getURL?>page=<?=$total_page?>">마지막</a></font>
	</div>
	<?php
	}
	?>

</body>
</html>