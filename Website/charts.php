<?php
// Step 1: Parse the CSV file in PHP
$dataHumedad = [];
$dataLuz = [];
$dataUltrasonido = [];
$file = fopen('sensor_data.csv', 'r');
$headers = fgetcsv($file); // Skip the header row
$TotalAmounOfRows = 0;

while (($row = fgetcsv($file)) !== false) {
    $TotalAmounOfRows += 1;
    date_default_timezone_set('America/Mexico_City');
    $timestamp = date('Y-m-d H:i:s', intval($row[0]));
    $sensor = $row[1];
    $jsonData = json_decode($row[2], true); // Convert the JSON string to an associative array
    
    // Extract specific data values from $jsonData and organize them as needed
    // Store the data in a suitable format
    if($sensor === "Sensor de Humedad"){
        $dataHumedad[] = [
            'timestamp' => $timestamp,
            'sensor' => $sensor,
            // [{""tipoDeDato"":""Humedad"",""resultado"":""45.00%""},{""tipoDeDato"":""Temperatura"",""resultado"":""28.20°C""}]
            'humedad' => str_replace("%","",$jsonData[0]["resultado"]),
            'temperatura' => str_replace("°C","",$jsonData[1]["resultado"])
        ];
        $jsonDataHumedad = json_encode($dataHumedad);
    }
    if($sensor === "Sensor de luz"){
        $dataLuz[] = [
            'timestamp' => $timestamp,
            'sensor' => $sensor,
            // [{""tipoDeDato"":""Luminosidad"",""resultado"":""2611 lumenes""}]
            'lumenes' => str_replace(" lumenes","",$jsonData[0]["resultado"])
        ];
        $jsonDataLuz = json_encode($dataLuz);
    }
    if($sensor === "Sensor de ultrasonido"){
        $dataUltrasonido[] = [
            'timestamp' => $timestamp,
            'sensor' => $sensor,
            // [{""tipoDeDato"":""Distancia"",""resultado"":""654.04cm""}]
            'distancia' => str_replace("cm","",$jsonData[0]["resultado"])
        ];
        $jsonDataUltrasonido = json_encode($dataUltrasonido);
    }
}

fclose($file);
?>

<!DOCTYPE html>
<html>
<head>
    <script src="minjs/jquery-3.6.0.min.js"></script>
    <script src='minjs/Chart.min.js'></script>
    <script>
        function setCharts(){
            let xValues = <?php echo $jsonDataHumedad; ?>.map(row => row.timestamp);

            new Chart("ChartHumedad", {
                type: "line",
                data: {
                    labels: xValues,
                    datasets: [{ 
                        label: "Humedad",
                        data: <?php echo $jsonDataHumedad; ?>.map(row => row.humedad),
                        borderColor: "red",
                        fill: false
                    }, { 
                        label: "Temperatura",
                        data: <?php echo $jsonDataHumedad; ?>.map(row => row.temperatura),
                        borderColor: "green",
                        fill: false
                    }]
                },
                options: {
                    title: {
                        display: true,
                        text: "Sensor de Humedad"
                    },
                    legend: {display: true},
                    responsive: true,
                    maintainAspectRatio: true
                }
            });

            xValues = <?php echo $jsonDataLuz; ?>.map(row => row.timestamp);
            new Chart("ChartLuz", {
                type: "line",
                data: {
                    labels: xValues,
                    datasets: [{ 
                        label: "Luz",
                        data: <?php echo $jsonDataLuz; ?>.map(row => row.lumenes),
                        borderColor: "red",
                        fill: false
                    }]
                },
                options: {
                    title: {
                        display: true,
                        text: "Sensor de Luz"
                    },
                    legend: {display: true},
                    responsive: true,
                    maintainAspectRatio: true
                }
            });
            
            xValues = <?php echo $jsonDataUltrasonido; ?>.map(row => row.timestamp);
            new Chart("ChartUltrasonido", {
                type: "line",
                data: {
                    labels: xValues,
                    datasets: [{ 
                        label: "Ultrasonido",
                        data: <?php echo $jsonDataUltrasonido; ?>.map(row => row.distancia),
                        borderColor: "red",
                        fill: false
                    }]
                },
                options: {
                    title: {
                        display: true,
                        text: "Sensor de Ultrasonido"
                    },
                    legend: {display: true},
                    responsive: true,
                    maintainAspectRatio: true
                }
            });
        }

        $(document).ready(function() {
            var charts = document.querySelectorAll("[id^='Chart']");
            charts.forEach(function(chart) {
                chart.style.width = document.documentElement.clientWidth + 'px';
                chart.style.height = document.documentElement.clientWidth*.4 + 'px';
                chart.addEventListener('click', function() {
                    console.log(chart.style.width);
                    if(chart.style.width === document.documentElement.clientWidth + 'px'){
                        chart.style.width = '<?php echo $TotalAmounOfRows*5; ?>px';
                        chart.style.height = '<?php echo $TotalAmounOfRows*5*.4; ?>px';
                    }else{
                        chart.style.width = document.documentElement.clientWidth + 'px';
                        chart.style.height = document.documentElement.clientWidth*.4 + 'px';
                    }
                    setCharts();
                });
            });
            var chartsDivs = document.querySelectorAll("[id^='chart']");
            chartsDivs.forEach(function(chart) {
                chart.style.width = document.documentElement.clientWidth + 'px';
                chart.style.height = document.documentElement.clientWidth*.4 + 'px';
                chart.style.marginBottom = "10%";
                chart.addEventListener('click', function() {
                    console.log(chart.style.width);
                    if(chart.style.width === document.documentElement.clientWidth + 'px'){
                        chart.style.width = '<?php echo $TotalAmounOfRows*5; ?>px';
                        chart.style.height = '<?php echo $TotalAmounOfRows*5*.4; ?>px';
                        chart.style.marginBottom = "50%";
                    }else{
                        chart.style.width = document.documentElement.clientWidth + 'px';
                        chart.style.height = document.documentElement.clientWidth*.4 + 'px';
                        chart.style.marginBottom = "10%";
                    }
                    setCharts();
                });
            });
        });
    </script>
</head>
<body>
    <div id="chartHumedad" style="margin-bottom: 10%;">
        <canvas id="ChartHumedad""></canvas>
    </div>
    <div id="chartLuz" style="margin-bottom: 10%;">
        <canvas id="ChartLuz""></canvas>
    </div>
    <div id="chartUltrasonido" style="margin-bottom: 10%;">
        <canvas id="ChartUltrasonido"></canvas>
    </div>
    <script>
        setCharts();
    </script>

</body>
</html>