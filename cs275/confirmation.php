<!DOCTYPE html>
<html>
<head>
	<title>Confirmation</title>
</head>
<link rel="stylesheet" type="text/css" href="style.css">
<div id="banner">
		<h1>Student Information Center</h1>
	</div>
	<br>
<body>
	<div id="side">
			<ul>
				<li><a href="index.php">Home</a></li>
				<br>
				<li><a href="student_information.html">Submit a Record</a></li>
				<br>
				<li><a href="student_list.php">Student List</a></li>
				<br>

			</ul>
		</div>
</body>
</html>

<?php
	// database connection information
	$dbhost = 'oniddb.cws.oregonstate.edu';
	$dbname = 'malickc-db';
	$dbuser = 'malickc-db';
	$dbpass = 'Jz8QJFUt65lTYY16';

	$mysql_handle = mysql_connect($dbhost, $dbuser, $dbpass)
    	or die("Error connecting to database server");

	mysql_select_db($dbname, $mysql_handle)
    	or die("Error selecting database: $dbname");

	//echo 'Successfully connected to database!';

	$student_id = mysql_real_escape_string($_POST['studentID']);
	$first_name = mysql_real_escape_string($_POST['fName']);
	$last_name = mysql_real_escape_string($_POST['lName']);
	$email_address = mysql_escape_string($_POST['email']);
	$dob = mysql_escape_string($_POST['dob']);
	$maj = mysql_escape_string($_POST['major']);
	

	$query="INSERT INTO STUDENT(STUDENTID, FNAME, LNAME, EMAIL, MAJOR, DOB)
	    VALUES ('$student_id','$first_name','$last_name', '$email_address', '$maj', '$dob')";
	
	$result = mysql_query($query);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}

	mysql_close($mysql_handle);

	echo '<h1>Thank you for submitting your information!</h1> <br>';
	echo 'Here is the information we have recorded:<br><br>';
	echo 'Student ID: ', $student_id,'<br>';
	echo 'First Name: ', $first_name,'<br>';
	echo 'Last Name: ', $last_name,'<br>';
	echo 'Email Adress: ', $email_address,'<br>';
	echo 'Date of Birth: ', $dob,'<br>';
	echo 'Major: ', $maj, '<br>';

	echo '<a href="index.php">Back to front page</a>';

	
?>
