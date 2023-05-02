<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="refresh" content="30">
    <title>Casa Inteligente Tabla de datos</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="datatable-container">
        <h1>Tabla de datos de sensores</h1>
        <?php
            $servername = "localhost";
            $username = "id20686621_codemasterx";//"id20686621_codemasterx" //"root"
            $password = "~Md]>IFxaJyOL4P1";//~Md]>IFxaJyOL4P1 //""
            $dbname = "id20686621_ceti_sensordata";//"id20686621_ceti_sensordata" //"ceti_sensordata"

            // Create connection
            $conn = new mysqli($servername, $username, $password, $dbname);

            // Check connection
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            }

            // Retrieve data from "sensores" table
            $sql = "SELECT * FROM sensores";
            $result = $conn->query($sql);

            if ($result->num_rows > 0) {
                echo "<table class='datatable'><thead>";
                echo "<tr class='table-titles'><th><h3>Sensor</h3></th><th><h3>Datos</h3></th></tr></thead><tbody>";
                while($row = $result->fetch_assoc()) {
                    $sensor_id = $row["id"];
                    $nombre = $row["nombre"];

                    echo "<tr><td>$nombre</td><td>";

                    // Retrieve data from "datos" table for the current sensor
                    $sql2 = "SELECT * FROM datos WHERE sensor_id = $sensor_id";
                    $result2 = $conn->query($sql2);

                    if ($result2->num_rows > 0) {
                        while($row2 = $result2->fetch_assoc()) {
                            $tipoDeDato = $row2["tipoDeDato"];
                            $resultado = $row2["resultado"];
                            echo "$tipoDeDato: $resultado<br/>";
                        }
                    } else {
                        echo "No data available.";
                    }

                    echo "</td></tr>";
                }
                echo "</table>";
            } else {
                echo "No data available.";
            }

            // Close connection
            $conn->close();
        ?>
    </div>
</body>
</html>