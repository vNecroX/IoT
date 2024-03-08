<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Casa Inteligente Tabla de datos</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="datatable-container">
        <h1>Tabla de datos de sensores</h1>
        <?php
            if(isset($_GET['data'])) {
                $json = $_GET['data'];
                $data = json_decode($json);
                if(is_array($data)){
                    echo "<table class='datatable'><thead>";
                    echo "<tr class='table-titles'><th><h3>Sensor</h3></th><th><h3>Datos</h3></th></tr></thead><tbody>";
                    foreach($data as $obj) {
                        //nombre: nombre del sensor | datos: datos del sensor en json
                        if(isset($obj->nombre) && isset($obj->datos)) {
                            $nombre = $obj->nombre;
                            $datos = $obj->datos;
                            
                            echo "<tr><td>$nombre</td><td>";
                            if(is_array($datos)){
                                foreach($datos as $dato) {
                                    //tipoDeDato: que se esta midiendo | resultado: que se recibio
                                    if(isset($dato->tipoDeDato) && isset($dato->resultado)) {
                                        $tipoDeDato = $dato->tipoDeDato;
                                        $resultado = $dato->resultado;
                                        echo "$tipoDeDato: $resultado<br/>";
                                    }
                                }
                            }
                            echo "</td></tr>";
                        } else {
                            echo "Please provide the JSON data required.";
                        }
                    }
                    echo "</table>";
                }
            } else {
                echo "Please provide JSON data.";
            }
        ?>
    </div>
    <a class="config-button" href="configuracion.php">Configuración</a>
</body>
</html>