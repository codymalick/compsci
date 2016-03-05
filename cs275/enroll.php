<!DOCTYPE html>
<html>
<link rel="stylesheet" type="text/css" href="style.css">
<head>
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
		<div id="main">
			<fieldset>
			<legend>Student List</legend>
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
			    	$query1 = "select * from STUDENT where STUDENTID = " . $_GET["student_id"];
			    	$result = mysql_query($query1, $mysql_handle);
			    }

			    $stud = mysql_fetch_assoc($result);

			    echo $stud["FNAME"] . " " . $stud["LNAME"] . " Enrollment Record";

				$query = "SELECT * FROM COURSE";
				$students = mysql_query($query);
				echo "<table style='width = 50%' border='1px'>
						<th>Name</th>
						<th>Description</th>
						<th>Credits</th>
						<th>Term</th>
				";

				$enroll_query = "INSERT INTO Enrollment (" . $stud["STUDENTID"] . ', ' . $row["COURSEID"] . ", " . $_POST['term'] . ")";

				echo $enroll_query;


				while($row = mysql_fetch_array($students)) {
					$date = substr($row["DOB"], 0,10);
					//echo $date->date('Y', $row["DOB"]);

					echo'<tr>' .
						 	'<td>' .
								$row['NAME'] .
							'</td>' .
							'<td>' .
								$row['DESC'] .
							'<td>' .
								$row['CREDITS'] .
							'</td>' .
							'<td>' .
								'<select name="term">'.
								'<option value="SP"/>SP'.
								'<option value="SU"/>SU'.
								'<option value="FA"/>FA'.
								'<option value="WN"/>WN'.
								'</select>' .

							'</td>' .
							'<td>' .
								"<a href='enroll.php?student_id=" . $enroll_query . "'>Enroll</a>" .
							'</td>' .
						'</tr>';
				}
				echo "</table>";
				


				mysql_close($mysql_handle);

			?>
		</fieldset>
		</div>	
	</body>
</html>