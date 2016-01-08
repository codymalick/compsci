<!DOCTYPE html>
<html>
<link rel="stylesheet" type="text/css" href="style.css">
	<title></title>
	</head>
	<div id="banner">
		<h1>Student Information Center</h1>
	</div>
	<br>
	<body>
	<div id="side">
		<ul>
			<li>
				<a href="index.php">Home</a>
			</li>
			<br>
			<li>
				<a href="student_information.html">Submit a Record</a>
			</li>
			<br>
			<li>
				<a href="student_list.php">Student List</a>
			</li>
			<br>
		</ul>
	</div>
	<?php 
		$dbhost = 'oniddb.cws.oregonstate.edu';
		$dbname = 'malickc-db';
		$dbuser = 'malickc-db';
		$dbpass = 'Jz8QJFUt65lTYY16';

		$mysql_handle = mysql_connect($dbhost, $dbuser, $dbpass)
	    or die("Error connecting to database server");

		mysql_select_db($dbname, $mysql_handle)
	    or die("Error selecting database: $dbname");

	    if(isset($_GET["student_id"])) {
	    	$query = "select * from STUDENT where STUDENTID = " . $_GET["student_id"];
	    	$result = mysql_query($query, $mysql_handle);
	    }

	    $stud = mysql_fetch_assoc($result);

	echo '<fieldset id="formbox">' .
			'<legend>Student Information Update Form</legend>' .
			'Please update your information' .
		'<form method="post" action="update.php">' .
			'<div id="form">' .
			'<p><label>Student ID:</label> <input id="textbox" type="text" name="studentID" value ="' . $stud['STUDENTID'] . '"  required><br></p>' .
			'<p><label>First Name:</label> <input id="textbox" type="text" name="fName" value ="' . $stud['FNAME'] . '" required><br></p>' .
			'<p><label>Last Name:</label> <input id="textbox" type="text" name="lName" value ="' . $stud['LNAME'] . '"  required><br></p>' .
			'<p><label>Email Address:</label> <input id="textbox" type="email" name="email" value ="' . $stud['EMAIL'] . '"  required><br></p>' .
			'<p><label>Date of Birth:</label> <input id="textbox" type="number" name="dob" maxlength = "4" value ="' . $stud['DOB'] . '"  required><br></p>' .
			'Major: <br>' .
				'<input type="radio" name="major" value="CS"> CS <br>' .
				'<input type="radio" name="major" value="ECE"> ECE <br>' .
				'<input type="radio" name="major" value="Other"> Other <br>' .
			'<br>' .
			'Courses: <br>' .
				'<input type="checkbox" name="class" value="CS161"> CS 161 <br>' .
				'<input type="checkbox" name="class" value="CS162"> CS 162 <br>' .
				'<input type="checkbox" name="class" value="CS261"> CS 261 <br>' .
				'<input type="checkbox" name="class" value="MTH251"> MTH 251 <br>' .
				'<input type="checkbox" name="class" value="ECE111"> ECE 111 <br>' .
		'<input type="submit"/>' .
	'</fieldset>'
	;






	?>




</html>