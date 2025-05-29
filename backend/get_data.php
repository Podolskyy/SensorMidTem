<?php
// Allow requests from any origin (for cross-origin access, e.g., Flutter app)
header("Access-Control-Allow-Origin: *");

// Set response content type to JSON
header("Content-Type: application/json");

// Create a connection to the MySQL database
$conn = new mysqli("localhost", "root", "82052103", "sensordatadb");

// Check if the connection failed
if ($conn->connect_error) {
    // Return a JSON error message and stop script
    die(json_encode(["error" => "Connection failed"]));
}

// SQL query to get the latest 20 readings from the dhtreadings table
$sql = "SELECT * FROM dhtreadings ORDER BY timestamp DESC LIMIT 20";
$result = $conn->query($sql);

// Prepare an array to hold the results
$data = [];

// Loop through the query result and store each row in the data array
while ($row = $result->fetch_assoc()) {
    $data[] = $row;
}

// Output the array as a JSON response
echo json_encode($data);

// Close the database connection
$conn->close();
?>
