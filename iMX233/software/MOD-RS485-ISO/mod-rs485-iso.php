<?php
/*
 * mod-rs485-iso.php
 * 
 * Copyright 2012 OLIMEX Ltd<>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
	
	
<?php
$addr = "0xA0";
$ch1 = "unchecked";
$ch2 = "unchecked";
$r = "1";
$w = "2";
$cmd = "0";

if(isset($_POST["submit"]))
{
	$addr = $_POST["txt1"];
	if(empty($addr))
	{
		echo "Address field is empty";
	}
	else
	{
		if(isset($_POST['ch1']))
		{
			$ch1 = $_POST['ch1'];
			
			if($ch1 == 'Read')
			{
				$ch1  = "checked";
			}
		}
		
		if(isset($_POST['ch2']))
		{
			$ch2 = $_POST['ch2'];
			
			if($ch2 == 'Write')
			{
				$ch2  = "checked";
			}
		}
		
		if($ch1 == "checked")
		{
			$cmd = $r;
		}
		if($ch2 == "checked")
		{
			$cmd += $w;
		}
		
		echo "<p>I2C Address is :".$addr."</p>";
		echo "<p>Command is: ./mod-rs485-iso -c " . $addr . " " . $cmd ." </p>";
		passthru ("./mod-rs485-iso -c ".$addr." ".$cmd);
		echo "<hr>";
	}	
}

?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>untitled</title>
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 1.22" />
</head>

<body>
	<form method="post" action="mod-rd485-iso.php">
	Address:  <input type="text" size="12" name="txt1"><br />
	<input type="checkbox" value="Read" name="ch1" <?php print $ch1 ?> >Read<br />
	<input type="checkbox" value="Write" name="ch2" <?php print $ch2 ?> >Write<br />
	
	<input type="submit" value="submit" name="submit"><br />
	</form>
	
</body>

</html>
