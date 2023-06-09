<?php
// Configuración de la base de datos
$servername = "localhost";
$username = "id20878984_codemasterx";//"id20686621_codemasterx" //"root"
$password = "OjT9Fz+E0m|9szJ=";//~Md]>IFxaJyOL4P1 //""
$dbname = "id20878984_ceti_sensordata";//"id20686621_ceti_sensordata" //"ceti_sensordata"

// Crear conexión a la base de datos
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar si hay error en la conexión
if ($conn->connect_error) {
    die("Error de conexión a la base de datos: " . $conn->connect_error);
}

// Consultar la última lectura del sensor de ultrasonido en la base de datos
$sql = "SELECT resultado FROM datos WHERE tipoDeDato = 'Distancia' ORDER BY sensor_id DESC LIMIT 1";
$result = $conn->query($sql);

$distanciaMedida = 0; // Valor predeterminado si no se encuentra ninguna lectura en la base de datos

if ($result->num_rows > 0) {
    // Obtener el resultado de la consulta
    $row = $result->fetch_assoc();
    $distanciaMedida = obtenerDistancia($row['resultado']);
}

$configuracion = [];
if (file_exists("configuracion.json")) {
    $jsonConfiguracion = file_get_contents("configuracion.json");
    $configuracion = json_decode($jsonConfiguracion, true);
}

// Función para extraer la distancia de la cadena de resultado
function obtenerDistancia($resultado) {
    // Buscar la posición del texto "cm"
    $pos = strpos($resultado, "cm");

    if ($pos !== false) {
        // Extraer la distancia como una subcadena antes del texto "cm"
        $distancia = substr($resultado, 0, $pos);

        // Convertir la distancia a un número decimal
        $distancia = floatval($distancia);

        return $distancia;
    }

    return 0; // Valor predeterminado si no se encuentra la distancia en la cadena de resultado
}

// Crear un arreglo con los datos que se enviarán como respuesta
$response = array(
    'Sensor de ultrasonido' => $distanciaMedida,
    'configuracion' => $configuracion
);

// Convertir el arreglo a formato JSON
$jsonResponse = json_encode($response);

// Enviar la respuesta al cliente
header('Content-Type: application/json');
echo $jsonResponse;

// Cerrar la conexión a la base de datos
$conn->close();
?>