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

if(isset($_GET['data'])) {
    $json = $_GET['data'];
    $data = json_decode($json);
    if(is_array($data)){
        foreach($data as $obj) {
            //nombre: nombre del sensor | datos: datos del sensor en json
            if(isset($obj->nombre) && isset($obj->datos)) {
                $nombre = $conn->real_escape_string($obj->nombre);
                $datos = $conn->real_escape_string(json_encode($obj->datos));

                // Insert data into "sensores" table
                $sql = "INSERT INTO sensores (nombre) VALUES ('$nombre')";
                if ($conn->query($sql) === TRUE) {
                    $sensor_id = $conn->insert_id;

                    // Insert data into "datos" table
                    if(is_array($obj->datos)){
                        foreach($obj->datos as $dato) {
                            //tipoDeDato: que se esta midiendo | resultado: que se recibio
                            if(isset($dato->tipoDeDato) && isset($dato->resultado)) {
                                $tipoDeDato = $conn->real_escape_string($dato->tipoDeDato);
                                $resultado = $conn->real_escape_string($dato->resultado);

                                $sql = "INSERT INTO datos (sensor_id, tipoDeDato, resultado) VALUES ('$sensor_id', '$tipoDeDato', '$resultado')";
                                if ($conn->query($sql) !== TRUE) {
                                    echo "Error: " . $sql . "<br>" . $conn->error;
                                }else{
                                    echo "{'response':'OK'}";
                                }
                            }
                        }
                    }
                } else {
                    echo "Error: " . $sql . "<br>" . $conn->error;
                }
            } else {
                echo "Please provide the JSON data required.";
            }
        }
    }
} else {
    echo "Please provide JSON data.";
}

// Close connection
$conn->close();
?>