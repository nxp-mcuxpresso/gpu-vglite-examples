# GPU VGLite Examples

## Overview

This directory contains a collection of sample applications demonstrating the
**VGLite GPU** hardware-accelerated 2D graphics rendering capabilities in **Zephyr**.

VGLite is a lightweight 2D vector graphics library optimized for embedded systems
with GPU acceleration support on **NXP i.MX RT series microcontrollers**.

---

## Available Samples

### `clock`
Displays an animated clock with moving hands, showcasing real-time graphics
updates and smooth animation capabilities.
- For documentation and application code, refer to the [clock sample](./clock)

### `cube`
Renders a 3D cube with isometric projection, demonstrating VGLite’s ability
to create 3D-like graphics using 2D transformations with smooth animation.
- For documentation and application code, refer to the [cube sample](./cube)

### `decompress_etc2`
Shows **ETC2 texture decompression and rendering**, demonstrating VGLite’s
support for compressed texture formats.
- For documentation and application code, refer to the [decompress_etc2 sample](./decompress_etc2)

### `tiger`
Renders the classic *tiger demo* — a complex polygon vector graphic showcasing
advanced path rendering and smooth curves.
- For documentation and application code, refer to the [tiger sample](./tiger)

### `tile`
Demonstrates tiled image rendering using blit operations combined with polygon
vector graphics on a black buffer.
- For documentation and application code, refer to the [tile sample](./tile)

### `toolkit`
Demonstrates **SVG (Scalable Vector Graphics)** rendering capabilities using the
VGLite toolkit, including path rendering and transformations.
- For documentation and application code, refer to the [toolkit sample](./toolkit)

### `vector`
Demonstrates polygon vector graphics rendering with high-quality anti-aliasing
and smooth edges on a blue buffer.
- For documentation and application code, refer to the [vector sample](./vector)

---

## Features Demonstrated

Across all samples, the following VGLite features are demonstrated:

- **Vector Graphics Rendering** – High-quality path rendering with anti-aliasing
- **Image Blitting** – Fast image copy and transformation operations
- **Transformations** – Rotation, scaling, translation, and matrix operations
- **Color Formats** – Support for multiple pixel formats and color spaces
- **Texture Compression** – ETC2 compressed texture support
- **Animation** – Real-time graphics updates and smooth animations
- **3D Projection** – Isometric and perspective-style transformations
- **SVG Support** – Scalable Vector Graphics rendering

---

## Getting Started

Each sample includes detailed documentation in its respective directory with
complete build instructions, board setup, and configuration options.

Refer to individual sample `README` files for:

- Hardware requirements and supported boards
- Display panel configuration
- Build and flash instructions
- Board-specific settings
- Troubleshooting information

---

## References

- **NXP i.MX RT Series**
  https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/i-mx-rt-crossover-mcus:IMX-RT-SERIES

- **VGLite API reference**
  https://www.nxp.com/docs/en/reference-manual/IMXRTVGLITEAPIRM.pdf

- **VGLite GitHub Repository**
  https://github.com/nxp-mcuxpresso/gpu-vglite

