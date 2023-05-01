<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Casa Inteligente Tabla de datos</title>
    <style>
		table, th, td {
            border: 1px solid black;
            border-collapse: collapse;
		}
		th, td {
            padding: 10px;
            text-align: left;
		}
	</style>
</head>
<body>
    <h1>Tabla de datos de sensores</h1>
	<?php
		if(isset($_GET['json'])) {
			$json = $_GET['json'];
			$data = json_decode($json);
            if(is_array($data)){
                echo "<table>";
                echo "<tr><th>Sensor</th><th>Datos</th></tr>";
                foreach($data as $obj) {
                    //nombre: nombre del sensor | datos: datos del sensor en json
                    if(isset($obj->nombre) && isset($obj->datos)) {
                        $nombre = $obj->nombre;
                        $datos = $obj->datos;
                        
                        echo "<tr><td>$nombre</td><td>";
                        foreach($datos as $dato) {
                            //tipoDeDato: que se esta midiento | resultado: que se recibio
                            if(isset($dato->tipoDeDato) && isset($dato->resultado)) {
                                $tipoDeDato = $dato->tipoDeDato;
                                $resultado = $dato->$resultado;
                                echo "$tipoDeDato: $resultado<br/>";
                            }
                        }
                        echo "</td></tr>";
                        echo "</table>";
                    } else {
                        echo "Please provide the JSON data required.";
                    }
                }
            }
		} else {
			echo "Please provide JSON data.";
		}
	?>
</body>
</html>