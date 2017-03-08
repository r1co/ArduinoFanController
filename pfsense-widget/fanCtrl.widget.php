<div>

<div>

<?php
	$items = [];
	$itemsPerRow=5;

	$itemsStart = 30;
	$itemsEnd = 100;
	$itemsStep = 10;

	for ($x = $itemsStart; $x <= $itemsEnd; $x=$x+$itemsStep) {
 		$items[] = $x;
	}


	print('<table id="" class="table table-striped table-hover">');
	print("<thead><tr>");
	for($x = 0; $x<$itemsPerRow;$x++){
		if($x==0){
			print("<th></th>");
		}else{
			print("<th></th>");
		}
	}
	print("</tr></thead>");
	print("<tbody>");
	for($x = 0; $x<count($items);$x=$x+$itemsPerRow){
		print("<tr>");
		for($y=0;$y<$itemsPerRow;$y++){
			print("<td><a
href='?action=set&value=".$items[$x+$y]."'>".$items[$x+$y]."</a></td>");
		}
		print("</tr>");
	}

	print("</tbody></table>");
?>
</div>

<?php

function sendSerialCommand(){
	$cmd = 'echo "set#'.$_GET['value'].'" > /tmp/fanController/input';
	$output = shell_exec($cmd);
	print "<pre>cmd:</pre>";
	print "<pre>$cmd</pre>";
}


if ($_GET['action']) {
   if( $_GET['action'] == "set" ){
	if ($_GET['value']) {
		$output = sendSerialCommand();
	}else{
		print("Value missing");
	}
   }
}
 ?>

</div>
