<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>RFID</title>
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 1.22" />
	
<script>
	function Reload()
	{
		window.location.reload();
	}
</script>
</head>

<body>
To use properly this demo, first set the path to the LOG file.

By default the log file is in <strong>/home/rfid/</strong> folder.

<table  width="50%">
	<thead>
		<tr>
			<th align="left">NAME</th>
			<th align="left">DATE</th>
		</tr>
	</thead>
<tr>
<?php
$handle = fopen("/home/rfid/LOG", "r");
while($userdata = fscanf($handle, "%s\t\t%s %s %s %s %s\n"))
{
	list($aa,$bb, $cc, $dd, $ee, $ff) = $userdata;
	echo '</tr><tr>';
	echo '<td>'.$aa.'</td><td>'.$bb.' '.$cc.' '.$dd.' '.$ee.' '.$ff.'</td>';
}
?>
</tr>
</table>

<input type = button value="Refresh" onClick = Reload()>
	
</body>

</html>
