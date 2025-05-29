<?php
$servername = "localhost"; //server
$username = "root"; //username
$password = "82052103"; //password
$dbname = "sensordatadb";  //database

// Connect to database
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Support both POST and GET
$temperature = $_POST['temperature'] ?? $_GET['temperature'] ?? null;
$humidity = $_POST['humidity'] ?? $_GET['humidity'] ?? null;

// Validate data
if ($temperature !== null && $humidity !== null) {
    $stmt = $conn->prepare("INSERT INTO DHTReadings (temperature, humidity) VALUES (?, ?)");
    $stmt->bind_param("dd", $temperature, $humidity);
    
    if ($stmt->execute()) {
        echo "Success: Data inserted";
    } else {
        echo "Insert error: " . $stmt->error;
    }

    $stmt->close();
} else {
    echo "Error: Missing temperature or humidity";
}

$conn->close();
?>
