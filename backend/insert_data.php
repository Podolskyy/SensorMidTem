<?php
// Database connection settings
$servername = "localhost";       // Database server address
$username   = "root";            // MySQL username
$password   = "82052103";        // MySQL password
$dbname     = "sensordatadb";    // Database name

// Establish connection to the database
$conn = new mysqli($servername, $username, $password, $dbname);

// Check if connection failed
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Support data coming from both POST and GET requests
$temperature = $_POST['temperature'] ?? $_GET['temperature'] ?? null;
$humidity    = $_POST['humidity'] ?? $_GET['humidity'] ?? null;

// Only proceed if both values are provided
if ($temperature !== null && $humidity !== null) {

    // Prepare SQL statement to prevent SQL injection
    $stmt = $conn->prepare("INSERT INTO DHTReadings (temperature, humidity) VALUES (?, ?)");

    // Bind parameters as double (float) values
    $stmt->bind_param("dd", $temperature, $humidity);
    
    // Execute the query and check for success
    if ($stmt->execute()) {
        echo "Success: Data inserted";
    } else {
        echo "Insert error: " . $stmt->error;
    }

    $stmt->close(); // Close the prepared statement
} else {
    // One or both values were missing
    echo "Error: Missing temperature or humidity";
}

$conn->close(); // Always close the DB connection
?>
