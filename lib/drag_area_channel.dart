import 'package:flutter/services.dart';

const String _dragChannelName = 'flutter/drag_area';
const String _dragWindowMethod = 'dragWindow';

class DragAreaChannel {
  DragAreaChannel._();

  final MethodChannel _platformChannel = const MethodChannel(_dragChannelName);

  static final DragAreaChannel instance = new DragAreaChannel._();

  void dragWindow() async {
    await _platformChannel.invokeMethod(_dragWindowMethod);
  }
}
