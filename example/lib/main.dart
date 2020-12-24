import 'package:drag_area/drag_area.dart';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: PreferredSize(
          child: DragArea(
            child: AppBar(
              title: const Text('Plugin example app'),
            ),
          ),
          preferredSize: Size.fromHeight(kToolbarHeight),
        ),
        body: Center(
          child: DragArea(
            child: FlutterLogo(
              size: 100.0,
            ),
          ),
        ),
      ),
    );
  }
}
