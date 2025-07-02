# Quantum Mechanical Keyboard Firmware for K10 Max
For original QMK project, see: [![Current Version](https://img.shields.io/github/tag/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/tags)

For Keychron forks, see: (https://github.com/Keychron/qmk_firmware)

This is my custom firmware for the Keychron K10 Max keyboard. It includes:

* Custom lighting effect of a gradient that differs based on Layer0-1 or 2-3
* Custom lighting effect that ripples out from key presses
* Custom macro system to allow for TOGGLE, HOLD, or ONCE type macros

The main purpose of this was to allow for the different types of macros, as Via/ViaL only allow for the most basic macros.

All custom code is contained within keyboards/keychron/k10_max/ansi/rgb/keymaps/k0nker.

Changes may have been made to the keyboard.json, info.json, config.h, and rule.mk files in the other keychron/k10_max folders in order to save space in the compiled firmware or enable RGB features.

This repo is made public for others to use and learn from as they wish, with no guarantees. Use at your own risk.
