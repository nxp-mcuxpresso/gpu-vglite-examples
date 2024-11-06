* What is a general step to generate graphics assets from SVG file ?
  gpu-vglite-toolkit.sh ClockAnalogOrange.svg clock_analog.h

* Quick steps to generate graphics assets ?
  To generate a clock basic dial with fixed color hour needle and minute needle.
  ./generate_assets.sh basic

  To generate a clock with linear gradient based dial with fixed color hour needle and minute needle.
  ./generate_assets.sh advanced

NOTE:
  generate_assets.sh is a helper script which will let you generate header.
  You will need to set TOOLKIT_PATH and OUTPUT_PATH correctly in generate_assets.sh properly.

