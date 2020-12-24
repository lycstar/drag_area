import 'package:drag_area/drag_area_channel.dart';
import 'package:flutter/material.dart';

class DragArea extends StatelessWidget {
  final Widget child;

  DragArea({@required this.child});

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTapDown: (detail) {
        DragAreaChannel.instance.dragWindow();
      },
      child: child,
    );
  }
}
