import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:async';
import 'package:fl_chart/fl_chart.dart';

class SensorChart extends StatefulWidget {
  @override
  _SensorChartState createState() => _SensorChartState();
}

class _SensorChartState extends State<SensorChart> {
  List<FlSpot> temperatureData = [];
  List<FlSpot> humidityData = [];
  List<String> timeLabels = [];
  Timer? refreshTimer;

  @override
  void initState() {
    super.initState();
    fetchSensorData();
    refreshTimer = Timer.periodic(Duration(seconds: 10), (timer) {
      fetchSensorData();
    });
  }

  @override
  void dispose() {
    refreshTimer?.cancel();
    super.dispose();
  }

  Future<void> fetchSensorData() async {
    final url = Uri.parse("http://192.168.1.3:8080/ESP32/get_data.php");
    try {
      final response = await http.get(url);
      if (response.statusCode == 200) {
        final List<dynamic> data = json.decode(response.body).reversed.toList();
        List<FlSpot> tempSpots = [];
        List<FlSpot> humSpots = [];
        List<String> labels = [];

        int dataCount = data.length;
        for (int i = 0; i < dataCount; i++) {
          final point = data[i];
          final double temp = double.tryParse(point["temperature"] ?? "") ?? 0;
          final double hum = double.tryParse(point["humidity"] ?? "") ?? 0;

          tempSpots.add(FlSpot(i.toDouble(), temp));
          humSpots.add(FlSpot(i.toDouble(), hum));
          labels.add(point["timestamp"].toString().substring(11, 16));
        }

        setState(() {
          temperatureData = tempSpots;
          humidityData = humSpots;
          timeLabels = labels;
        });
      } else {
        throw Exception("HTTP ${response.statusCode}");
      }
    } catch (e) {
      print("Error fetching data: $e");
    }
  }

  LineChartData buildChart(
      List<FlSpot> spots, String title, Color color, double yMin, double yMax) {
    return LineChartData(
      backgroundColor: Colors.white,
      minY: yMin,
      maxY: yMax,
      gridData: FlGridData(show: true, drawVerticalLine: true),
      titlesData: FlTitlesData(
        bottomTitles: AxisTitles(
          sideTitles: SideTitles(
            showTitles: true,
            interval: 2,
            getTitlesWidget: (value, _) {
              int index = value.toInt();
              if (index >= 0 && index < timeLabels.length) {
                return Padding(
                  padding: const EdgeInsets.only(top: 8.0),
                  child:
                      Text(timeLabels[index], style: TextStyle(fontSize: 10)),
                );
              }
              return Text('');
            },
          ),
        ),
        leftTitles: AxisTitles(
          sideTitles: SideTitles(
            showTitles: true,
            reservedSize: 40,
            getTitlesWidget: (value, _) => Padding(
              padding: const EdgeInsets.only(right: 8.0),
              child: Text(value.toStringAsFixed(0),
                  style: TextStyle(fontSize: 10)),
            ),
          ),
        ),
        rightTitles: AxisTitles(
          sideTitles: SideTitles(showTitles: false),
        ),
        topTitles: AxisTitles(
          sideTitles: SideTitles(showTitles: false),
        ),
      ),
      borderData: FlBorderData(
        show: true,
        border: Border.all(color: Colors.grey.shade300),
      ),
      lineBarsData: [
        LineChartBarData(
          spots: spots,
          isCurved: true,
          color: color,
          barWidth: 3,
          belowBarData: BarAreaData(
            show: true,
            color: color.withOpacity(0.2),
          ),
          dotData: FlDotData(show: true),
        ),
      ],
      lineTouchData: LineTouchData(enabled: true),
    );
  }

  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 2,
      child: Scaffold(
          appBar: AppBar(
            title: Text("Environmental Sensor Charts"),
            backgroundColor: Colors.teal,
            actions: [
              IconButton(
                icon: Icon(Icons.refresh),
                onPressed: fetchSensorData,
              ),
            ],
            bottom: TabBar(
              indicatorColor: Colors.white,
              tabs: [
                Tab(icon: Icon(Icons.thermostat), text: "Temperature"),
                Tab(icon: Icon(Icons.water_drop), text: "Humidity"),
              ],
            ),
          ),
          body: StreamBuilder(
            stream: Stream.periodic(Duration(seconds: 10))
                .asyncMap((_) => fetchSensorData()),
            builder: (context, snapshot) {
              return TabBarView(
                children: [
                  Padding(
                    padding: const EdgeInsets.all(16.0),
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text("Temperature (°C)",
                            style: TextStyle(
                                fontSize: 16, fontWeight: FontWeight.bold)),
                        SizedBox(height: 10),
                        Expanded(
                            child: LineChart(buildChart(temperatureData,
                                "Temperature", Colors.orange, 0, 50)))
                      ],
                    ),
                  ),
                  Padding(
                    padding: const EdgeInsets.all(16.0),
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text("Humidity (%)",
                            style: TextStyle(
                                fontSize: 16, fontWeight: FontWeight.bold)),
                        SizedBox(height: 10),
                        Expanded(
                            child: LineChart(buildChart(
                                humidityData, "Humidity", Colors.blue, 0, 100)))
                      ],
                    ),
                  ),
                ],
              );
            },
          )),
    );
  }
}
