<?php
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // Obtener los valores de configuración desde el formulario
    $temperaturaIdeal = $_POST["temperaturaIdeal"];
    $puertaPrincipal = isset($_POST["puertaPrincipal"]) ? $_POST["puertaPrincipal"] : "";
    $salon1 = isset($_POST["salon1"]) ? $_POST["salon1"] : "";
    $salon2 = isset($_POST["salon2"]) ? $_POST["salon2"] : "";
    $luminosidadEncender = $_POST["luminosidadEncender"];
    $luminosidadApagar = $_POST["luminosidadApagar"];
    $ventilador = isset($_POST["ventilador"]) ? $_POST["ventilador"] : "";

    // Crear un array con los valores de configuración
    $configuracion = [
        "temperaturaIdeal" => $temperaturaIdeal,
        "puertaPrincipal" => $puertaPrincipal,
        "salon1" => $salon1,
        "salon2" => $salon2,
        "luminosidadEncender" => $luminosidadEncender,
        "luminosidadApagar" => $luminosidadApagar,
        "ventilador" => $ventilador
    ];

    // Convertir el array a formato JSON
    $jsonConfiguracion = json_encode($configuracion, JSON_PRETTY_PRINT);

    // Guardar el JSON en un archivo
    file_put_contents("configuracion.json", $jsonConfiguracion);
}

// Leer el archivo JSON de configuración (si existe)
$configuracion = [];
if (file_exists("configuracion.json")) {
    $jsonConfiguracion = file_get_contents("configuracion.json");
    $configuracion = json_decode($jsonConfiguracion, true);
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuración</title>
    <link rel="stylesheet" href="css/style.css">
    <script src="minjs/jquery-3.6.0.min.js"></script>
    <script>
        let checkBoxChanged;
        let editingNumberInputs;
        // Función para actualizar los datos en la página
        function actualizarDatos() {
            $.ajax({
                url: 'get_data.php', // Archivo PHP que obtiene los datos de la base de datos
                type: 'GET',
                dataType: 'json',
                success: function(response) {
                    var configuracion = response["configuracion"];
                    // Actualizar los datos de configuración en la tabla
                    if(!editingNumberInputs){
                        $("#input-temperatura").val(configuracion.temperaturaIdeal);
                        $("#input-luminosidad-apagar").val(configuracion.luminosidadApagar);
                        $("#input-luminosidad-encender").val(configuracion.luminosidadEncender);
                    }
                    if(!checkBoxChanged){
                        $("input[name=puertaPrincipal][value=" + configuracion.puertaPrincipal + "]").prop("checked", true);
                        $("input[name=salon1][value=" + configuracion.salon1 + "]").prop("checked", true);
                        $("input[name=salon2][value=" + configuracion.salon2 + "]").prop("checked", true);
                        $("input[name=ventilador][value=" + configuracion.ventilador + "]").prop("checked", true);
                    }
                    // Verificar si la distancia es menor a 12 cm
                    var distancia = parseFloat(response["Sensor de ultrasonido"]);
                    if (distancia < 12 && puedeMostrarNotificacion) {
                        // Mostrar una notificación de que hay una persona en la puerta
                        mostrarNotificacion("¡Hay una persona en la puerta!");
    
                        // Desactivar la posibilidad de mostrar la notificación durante un minuto
                        puedeMostrarNotificacion = false;
                        setTimeout(function() {
                            puedeMostrarNotificacion = true;
                        }, 20000); // 20000 ms = 20 segundos
                    }
                },
                error: function(xhr, status, error) {
                    console.log('Error al obtener los datos: ' + error);
                }
            });
        }

        // Función para mostrar una notificación
        function mostrarNotificacion(mensaje) {
            window.alert(mensaje);
        }

        // Función para actualizar los datos automáticamente en intervalos regulares
        function iniciarActualizacionAutomatica() {
            // Llamar a la función "actualizarDatos" cada un intervalo de tiempo
            puedeMostrarNotificacion = true;
            setInterval(actualizarDatos, 1000);
        }

        // Iniciar la actualización automática al cargar la página
        $(document).ready(function() {
            checkBoxChanged = false;
            editingNumberInputs = false;
            const numberInputs = document.querySelectorAll('input[type="number"]');
            const radioButtons = document.querySelectorAll('input[type="radio"]');
            radioButtons.forEach(function(radioButton) {
                radioButton.addEventListener('change', function() {
                    editingRadioButtons = true;
                });
            });
            numberInputs.forEach(function(numberInput) {
                numberInput.addEventListener('input', function() {
                    editingNumberInputs = true;
                });
            });
            iniciarActualizacionAutomatica();
        });
    </script>
</head>
<body>
    <div class="datatable-container">
        <h1>Configuración</h1>
        <form method="post" action="<?php echo $_SERVER["PHP_SELF"]; ?>">
            <table class="datatable">
                <thead>
                    <tr class="table-titles">
                        <th><h3>Configuración</h3></th>
                        <th><h3>Valor</h3></th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>Temperatura Ideal</td>
                        <td>
                            <input type="number" step="0.01" min="-40" max="60" placeholder="Temperatura" id="input-temperatura" name="temperaturaIdeal" value="<?php echo isset($configuracion["temperaturaIdeal"]) ? $configuracion["temperaturaIdeal"] : ""; ?>">
                        </td>
                    </tr>
                    <tr>
                        <td>Encender/Apagar Luces - Puerta Principal</td>
                        <td>
                            <input type="radio" id="btn-puerta-principal-on" name="puertaPrincipal" value="on" <?php echo isset($configuracion["puertaPrincipal"]) && $configuracion["puertaPrincipal"] === "on" ? "checked" : ""; ?>>
                            <label for="btn-puerta-principal-on">Encender</label>
                            <input type="radio" id="btn-puerta-principal-off" name="puertaPrincipal" value="off" <?php echo isset($configuracion["puertaPrincipal"]) && $configuracion["puertaPrincipal"] === "off" ? "checked" : ""; ?>>
                            <label for="btn-puerta-principal-off">Apagar</label>
                        </td>
                    </tr>
                    <tr>
                        <td>Encender/Apagar Luces - Salón 1</td>
                        <td>
                            <input type="radio" id="btn-salon1-on" name="salon1" value="on" <?php echo isset($configuracion["salon1"]) && $configuracion["salon1"] === "on" ? "checked" : ""; ?>>
                            <label for="btn-salon1-on">Encender</label>
                            <input type="radio" id="btn-salon1-off" name="salon1" value="off" <?php echo isset($configuracion["salon1"]) && $configuracion["salon1"] === "off" ? "checked" : ""; ?>>
                            <label for="btn-salon1-off">Apagar</label>
                        </td>
                    </tr>
                    <tr>
                        <td>Encender/Apagar Luces - Salón 2</td>
                        <td>
                            <input type="radio" id="btn-salon2-on" name="salon2" value="on" <?php echo isset($configuracion["salon2"]) && $configuracion["salon2"] === "on" ? "checked" : ""; ?>>
                            <label for="btn-salon2-on">Encender</label>
                            <input type="radio" id="btn-salon2-off" name="salon2" value="off" <?php echo isset($configuracion["salon2"]) && $configuracion["salon2"] === "off" ? "checked" : ""; ?>>
                            <label for="btn-salon2-off">Apagar</label>
                        </td>
                    </tr>
                    <tr>
                        <td>Nivel de Luminosidad para Encender las Luces</td>
                        <td>
                            <input type="number" step="1" min="0" max="2500" placeholder="Luminosidad" id="input-luminosidad-encender" name="luminosidadEncender" value="<?php echo isset($configuracion["luminosidadEncender"]) ? $configuracion["luminosidadEncender"] : ""; ?>">
                        </td>
                    </tr>
                    <tr>
                        <td>Nivel de Luminosidad para Apagar las Luces</td>
                        <td>
                            <input type="number" step="1" min="0" max="2500" placeholder="Luminosidad" id="input-luminosidad-apagar" name="luminosidadApagar" value="<?php echo isset($configuracion["luminosidadApagar"]) ? $configuracion["luminosidadApagar"] : ""; ?>">
                        </td>
                    </tr>
                    <tr>
                        <td>Encender/Apagar Ventilador</td>
                        <td>
                            <input type="radio" id="btn-ventilador-on" name="ventilador" value="on" <?php echo isset($configuracion["ventilador"]) && $configuracion["ventilador"] === "on" ? "checked" : ""; ?>>
                            <label for="btn-ventilador-on">Encender</label>
                            <input type="radio" id="btn-ventilador-off" name="ventilador" value="off" <?php echo isset($configuracion["ventilador"]) && $configuracion["ventilador"] === "off" ? "checked" : ""; ?>>
                            <label for="btn-ventilador-off">Apagar</label>
                        </td>
                    </tr>
                </tbody>
            </table>
            <button type="submit" class="button">Guardar Configuración</button>
        </form>
        <div class="button"><a class="button" href="index.php">Home</a></div>
        <a class="button" href="charts.php">Charts</a>
    </div>
</body>
</html>