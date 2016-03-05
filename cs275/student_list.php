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


				$query = "SELECT * FROM STUDENT";
				$students = mysql_query($query);
				echo "<table style='width = 50%' border='1px'>
						<th>Name</th>
						<th>Student ID</th>
						<th>Email</th>
						<th>Major</th>
						<th>Date of Birth</th>
				";
				while($row = mysql_fetch_array($students)) {
					$date = substr($row["DOB"], 0,10);
					//echo $date->date('Y', $row["DOB"]);

					echo'<tr>' .
						 	'<td>' .
								$row['FNAME'] . " " . $row['LNAME'] .
							'</td>' .
							'<td>' .
								$row['STUDENTID'] .
							'<td>' .
								$row['EMAIL'] .
							'</td>' .
							'<td>' .
								$row['MAJOR'] .
							'</td>' .
							'<td>' .
								$date .
							'</td>' .
							'<td>' .
								"<a href='student_edit.php?student_id=" . $row["STUDENTID"] . "'>Edit</a>" .
							'</td>' .
							'<td>' .
								"<a href='enroll.php?student_id=" . $row["STUDENTID"] . "'>Enrollment</a>" .
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