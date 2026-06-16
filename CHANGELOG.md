# Changelog for CAN IO Board Hardware

Put all development notes/changelog stuff here. Try to format something like below to minimize git conflicts:

```
## <date, or commit hash>

<notes>

### Added

### Changed

### Removed

## <older date/hash>

...
```

This format is adopted from [keepachangelog.com](https://keepachangelog.com).

## 28a5f6d

### Added

- Finished the board.
    - Dimensions are 100mm\*100mm, with 3mm fillet on corners.
    - The two bottom PTHs are 4.25mm away from the bottom and side edges, and the top PTHs are 25mm away from the top.
    - WAGO Connectors are spaced 3.371mm from each other and are spaced evenly about the center of the board, 2.68mm away from the top edge.
    - When on, 12V is supplied to the LEFT side of the WAGO connector when facing it. 12V at the port above the small silkscreen arrow.
    - Relays are Panasonic SPST-NO relays with no internal flyback diode. Coil voltage and current consumption is 12V 30mA, so 360mW per relay.
    - High current output passes through solder bridges, 1mm traces (1oz outer copper, 2.4A current capacity), relays (5A), and WAGO 2601 connector. The weak link is the solder bridge which can handle 2A per channel.
    - The input power screw terminal is capable of 15A continuous. High current pins have either 1mm traces to/from or 2 spokes of 0.5mm thermal relief.
    - Stackup when ordering *MUST BE* JLC's 04161H-7628 with TG135 core, 1.6mm thickness 4 layer, for trace impedance purposes. CAN traces are matched to 123 Ohms as per KiCAD's calculator (plus JLC's fab parameters), have 60 + 60 termination (with capacitor), and 5mm spaced via fence at 4x the spacing. Kinda overkill but it makes sure there are no issues with noise at all.

## 91d809d

> [!NOTE]
> Since we are branching into two approaches (high side switch IC and relay), *this is the last commit*
> where the development of the PCB will occur together. From here, the high current output part of
> the schematic, as well as PCB layout and routing, will be done separately. Once the designs are
> complete, the finished BOM and Gerbers for each will be brought back to this branch.

### Added

- Finished the PCB board setup in PCB editor. Relevant calculations, and notes:
    - The board will have a 2oz (70um) outer copper thickness, and 1oz (35um) inner copper thickness.
    - The stackup is shown below
    - For the high current outputs, default traces (for HighPower netclass) have been set to 2mm width, which at 70um thickness will comfortably support 5A with 10 degree C temp increase. Still, copper pours *perforated with vias* is prefered. And in that case a pad must have at least 2 spokes from the surrounding copper pour.
    - USB and CAN have differential pairs, which require 90 and 120 differential impedance respectively. For the USB diff pair, trace width and spacing is 0.2mm and 0.36mm, as per KiCAD's calculator set to JLC's fab settings. However, the 0.1mm prepreg between the top copper layer and the first ground pour is too small to support CAN's 120ohm impedance. Thus, *THERE SHOULD BE NO GROUND POUR BENEATH THE CAN LINES DIFF PAIR*. The CANH and CANL lines trace width and spacing is 1mm and 0.82mm respectively, and factors in that the *closest copper pour* (power plane in our case) is 1.42mm away as per JLC's spec. This results in $Z_{odd}$ of 60 ohms and $Z_{even}$ of 100 ohms; still not ideal but acceptable. Again, *REMOVE GROUND COPPER POUR FROM BENEATH CANH AND CANL!!!*
    - Netclass assignments have NOT been done yet; once the schematic is complete and imported, please ***first*** complete this netclass assignment before starting any routing.
- Next steps is finishing the high current drive portion of the schematic, re-annotating, running ERC check, exporting BOM and importing into PCB editor. From there, PCB layout and routing.

| Layer | Comment |
|:-----:|:-------:|
| Signal + GND | High speed signals and diff pairs, surrounded by copper pour |
| GND | Full ground plane, for referencing, etc. |
| Power | Split plane with 5v and 3v3 as necessary. Don't route over splits |
| Signal + Power | Low speed signals + 12V power plane |

## 421d505

### Changed

- Replaced the buck converters AP63205WU and 203WU with drop in replacements 205QWU and 203QWU

## 4b08f0e

Basically replaced everything which was unavailable. High side mosfet driver/relay based output driver yet to be added in schematic. Also, buck converters need to be replaced.

### Added

- ECMF02-2AMX6 footprint

### Changed

- Replaced the G0B1KET6 with KET6N in CubeMX IOC and schematic
- Replaced the 14 pin USB C connector with 16 pin.
- Replaced ISOW1044 with ISO1042 + isolated DC-DC converter, followed by LC filter and LDO.

## 62c16c2

### Added

- Added an initial BOM csv, with all the manufacturer names and part numbers listed out. A few are missing, either because they are out of stock or undecided; they will have to be resolved before the PCB is complete and the order is placed.

## 1cf1787

### Changed

- Made the 20k to 2 10ks, so we don't have to order just one 20k.
- Need to change USB connector from 14 to 16 pin
- Need to change from G0B1KET6 to KET6N
- Need to find replacement for ISOW1044
- Need to make footprint for the USB and CAN filter ICs
- Need to decide upon a CAN bus connector
    - Once all of that is done, setup PCB editor with 2oz outer layers, stackup of signal + gnd -> gnd -> pwr -> signal + pwr, with 12V going to relays on the bottom 2oz layer. All 12V vias must be laid out in a grid.
    - Then setup netclasses, trace width and via sizes, differential pair sizes, etc.
    - Finally the rest of the layout and routing.

## a1869ab

### Added

- Finished the connectors sheet.
    - GPIO connector is just a normal 5 pin header for now, can change later.
    - SWD is connected via both TC2030, and also a 3 pin header, with NRST, SWDIO, and SWCLK
    - CAN bus lines to be connected via DT connector
    - Added USB-C, with USB peripheral in CubeMX, for printf and other usb stuff.
    - High and low current outputs will be delivered via WAGO connectors
    - Finally, added a Wakeup and NRST push button.

## 10131ee

### Added

- This design was mainly done by Kostadin. I believe he followed the example schematic from the datasheet. The current design was simply ported over from his schematic, and a few issues resolved.
- The reason behind the 12V RWM is simply that we're *expecting* that much drift on the IC's input anyway. Whether it's the actual CAN wires throughout the car, or simply by the max input of the CAN transceiver IC, we expect that the signal-to-ground voltage may exceed 3.75V. Since an input of beyond 12V (not differential, idk what happens then) isn't desirable, that becomes the diode's RWM.

## fb34b0c

### Added

- Finished the relay driver schematics. Used two TPL7407LA load driver ICs.
- The requirement was 2 *high current* channels (300mA, powering a relay), and 3 *low current* channels, providing 1.2A directly.
- The current configuration has one high current channel, one doubled up low current channel, and then another half-doubled up low current channel per IC. This evenly splits the current draw, if all channels were to be on simultaneously.
- The high current channels then drive the coils of two relay sockets.

## e61d195

### Added

- Finished the power schematic. However, there's some confusion about the post-smps filter... The IC's datasheet suggests using a 4.7uH inductor and two 22uF capacitors in parallel (making 44uF). This circuit resonates at 11kHz, and is basically a stop-band after that. Our buck IC has a switching frequency of 100kHz, which is wayyy beyond 11kHz; and so it (and all harmonics) should be attenuated. However, I've had bad experiences with this in the past (inadequate filtering, following similar procedure), and so I want another set of eyes on this.

## 5c744d9

### Added

- In previous commits:
    - an IOC, [here](https://github.com/NJIT-Solar-Car/can-io-board/blob/5c744d9fd663344a5dfaa119cfbe4434d60c49f4/Software/Software.ioc).
    - Created hierarchical sheets backbone.
    - Initial documentation, app notes, etc.
- Created a bus for the STM, and individual buses for each peripheral. The idea is to make each peripheral modular and within its own hierarchical sheet if possible. Object-oriented schematics if you want to put it that way.
- Added LEDs for the heartbeat and fault outputs
- Added 5 push buttons. The schematic for these came directly from Kostadin's implementation of an RC debounce circuit with the button. I changed the values a little so that the time constant is 10ms, as a [TI App Note](https://github.com/NJIT-Solar-Car/can-io-board/blob/5c744d9fd663344a5dfaa119cfbe4434d60c49f4/docs/ApplicationNotes/Debounce_a_Switch.pdf) suggested.

## 1c9e212

first commit

### Added

- Added readme, changelog, and documentation for all the major ics being used.
