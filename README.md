# LED Jacket

## Display
Amazon: https://www.amazon.de/dp/B078JBBPXK?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1

Az Delivery: https://www.az-delivery.de/en/products/1-77-zoll-spi-tft-display

#### Pin Connections
| Display | ESP32 Wroom Devkit |
| ----- | ----- |
| 1 GND | GND |
| 2 VCC (3.3 - 5V) | 3V3 |
| 3 SCL/SCK | 18 VSPI_CLK |
| 4 SDA/MISO | 23 VSPI_MOSI |
| 5 RES | GPIO4 |
| 6 RS | GPIO0 |
| 7 CS | GPIO5 |
| 8 LEDA | 3V3 |

## LED Strip WS2812B
Amazon: https://www.amazon.de/dp/B01CDTEJR0?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1

Watch out for power usage, my power-bank has max output of 1A, so LED strip shouldn't draw more. Use PWM to manage that.

#### Pin Connections
| WS2812B | ESP32 Wroom Devkit |
| ----- | ----- |
| GND | GND |
| VCC 5V | 5V |
| Din 1 | GPIO15 |
| Din 2 | GPIO22 |