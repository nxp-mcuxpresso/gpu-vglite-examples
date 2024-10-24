
* How to generate new graphics assets ?
  gpu-vglite-toolkit.sh ClockAnalogOrange.svg clock_analog.h
  gpu-vglite-toolkit.sh HourNeedle.svg hour_needle.h
  gpu-vglite-toolkit.sh MinuteNeedle.svg minute_needle.h

* How to try clock dial with radial gradient
  gpu-vglite-toolkit.sh ClockAnalogRadial.svg clock_analog.h

  Replace following in clock_freertos.c
    UI_LAYER_DATA(ClockAnalogOrange),
  with
    UI_LAYER_DATA(ClockAnalogRadial),

* How to try stroke in minute needle
  gpu-vglite-toolkit.sh MinuteNeedleStroke.svg minute_needle.h

  Replace following in clock_freertos.c
    UI_LAYER_DATA(MinuteNeedle),
  with
    UI_LAYER_DATA(MinuteNeedleStroke),
