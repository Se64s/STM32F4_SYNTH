# STM32F4_SYNTH

Yet another poly-voice synth based on the STM32F446x platform.

## Table of Contents

1. [About the Project](#about-the-project)
1. [Project Status](#project-status)
1. [Getting Started](#getting-started)
	1. [Building](#building)
1. [Release Process](#release-process)
	1. [Versioning](#versioning)
1. [License](#license)
1. [Authors](#authors)

## About the Project

Synth project using the DSP capabilities of the Cortex-M4 platform. This project is an excuse to play with the FPU unit of the M4 families.

Main features:
* 8 voices with ADSR envelopes and detuning features.
* Configurable effects engine.
* 48kHz, 16bit stereo output.

**[Back to top](#table-of-contents)**

## Project Status

Main audio engine complete, working on the user interface.

**[Back to top](#table-of-contents)**

## Getting Started

Build system based on [CMake](https://cmake.org) and [Ninja](https://ninja-build.org).

### Getting the Source

This project is [hosted on GitHub](https://github.com/Se64s/STM32F4_SYNTH). You can clone this project directly using this command:

```
git clone https://github.com/Se64s/STM32F4_SYNTH.git
```

### Building

```
cd build

cmake -DARM_TOOLCHAIN_DIR=<PATH_ARM_TOOLCHAIN> -DCMAKE_MAKE_PROGRAM=<PATH_TO_NINJA> -GNinja ..
```

**[Back to top](#table-of-contents)**

## Release Process

### Versioning

This project uses [Semantic Versioning](http://semver.org/). For a list of available versions, see the [repository tag list](https://github.com/Se64s/STM32F4_SYNTH/tags).

**[Back to top](#table-of-contents)**

## License

This project is licensed under the MIT License License - see [LICENSE.md](LICENSE.md) file for details.

**[Back to top](#table-of-contents)**

## Authors

* **[Sebastián Del Moral](https://github.com/Se64s)** - *Initial work*

**[Back to top](#table-of-contents)**

**[Back to top](#table-of-contents)**
