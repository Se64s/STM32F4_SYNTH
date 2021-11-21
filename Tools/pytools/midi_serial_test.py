#!/usr/bin/env python3
""" 
File name:    midi_serial_test.py
Description:  Bunch of basic testing for midi serial interface.
Author:       Sebastián DEl Moral Gallardo
Date:         20-11-2021
"""

import serial
import rtmidi
import sys
import argparse
import logging
import time
from logging import critical, error, info, warning, debug

# Defined user midi commands
MIDI_NOTE_ON = "ON"
MIDI_NOTE_OFF = "OFF"


def parse_arguments():
    """Read arguments from a command line."""
    parser = argparse.ArgumentParser(description='Arguments get parsed via --commands')
    parser.add_argument('-v', metavar='verbosity', type=int, default=3,
        help='Verbosity of logging: 0 -critical, 1- error, 2 -warning, 3 -info, 4 -debug')

    args = parser.parse_args()
    verbose = {0: logging.CRITICAL, 1: logging.ERROR, 2: logging.WARNING, 3: logging.INFO, 4: logging.DEBUG}
    logging.basicConfig(format='%(message)s', level=verbose[args.v], stream=sys.stdout)

    return args


def gen_midi_note_on_off(on_off, channel, note, velocity):
    midi_cmd = bytearray()
    if on_off == MIDI_NOTE_ON:
        midi_cmd.append(0x90 | (0x0f & channel))
        midi_cmd.append(note & 0x7F)
        midi_cmd.append(velocity & 0x7F)
    elif on_off == MIDI_NOTE_OFF:
        midi_cmd.append(0x80 | (0x0f & channel))
        midi_cmd.append(note & 0x7F)
        midi_cmd.append(velocity & 0x7F)
    else:
        error("Not valid midi command")
        error(on_off)
    return midi_cmd


def midi_speed_test():
    """Test defined to test continuous midi transmissions"""
    base_note = 100
    base_vel = 126
    midi_ch = 1
    midi_iter = 1000
    midi_iter = 0
    sleep_time = 0.0

    info("Init MIDI speed test")

    midiOut = rtmidi.MidiOut()
    midi_ports = midiOut.get_ports()
    info(midi_ports)
    midiOut.open_port(1)

    num_cmd = 0
    for a in range(midi_iter):
        midiOut.send_message(gen_midi_note_on_off(MIDI_NOTE_ON, midi_ch, base_note, base_vel))
        num_cmd += 1
        info("Test iter: [%d/%d]" % (num_cmd, midi_iter * 2))
        time.sleep(sleep_time)

        midiOut.send_message(gen_midi_note_on_off(MIDI_NOTE_ON, midi_ch, base_note, 0))
        num_cmd += 1
        info("Test iter: [%d/%d]" % (num_cmd, midi_iter * 2))
        time.sleep(sleep_time)

    info("End MIDI speed test")

    del midiOut


def midi_voice_test():
    """Test to check voice handlig in midi driver"""

    num_voices = 8
    base_note = 100
    base_vel = 100
    sleep_time = 0.5
    midi_ch = 1

    info("Init MIDI voice test")

    midiOut = rtmidi.MidiOut()
    midi_ports = midiOut.get_ports()
    info(midi_ports)
    midiOut.open_port(1)

    for a in range(num_voices):
        note = base_note + a
        vel = base_vel
        info("Send note ON : ch %d - note x%02X - vel x%02X" % (a, note, vel))
        midiOut.send_message(gen_midi_note_on_off(MIDI_NOTE_ON, midi_ch, note, vel))
        time.sleep(sleep_time)

    for a in range(num_voices):
        note = base_note + a
        vel = 0
        info("Send note OFF: ch %d - note x%02X - vel x%02X" % (a, note, vel))
        midiOut.send_message(gen_midi_note_on_off(MIDI_NOTE_ON, midi_ch, note, vel))
        time.sleep(sleep_time)

    info("End MIDI speed test")

    del midiOut


def midi_click_test():
    """Test to check clicking sound on midi activation"""
    base_note = 100
    base_vel = 100
    midi_ch = 1
    midi_iter = 1000
    sleep_time = 0.1

    info("Init MIDI click test")

    midiOut = rtmidi.MidiOut()
    midi_ports = midiOut.get_ports()
    info(midi_ports)
    midiOut.open_port(1)

    for a in range(midi_iter):
        note = base_note
        vel = base_vel
        
        info("Send note ON : iter %d - note x%02X - vel x%02X" % (a, note, vel))
        midiOut.send_message(gen_midi_note_on_off(MIDI_NOTE_ON, midi_ch, note, vel))
        time.sleep(sleep_time)

        info("Send note OFF: iter %d - note x%02X - vel x%02X" % (a, note, vel))
        midiOut.send_message(gen_midi_note_on_off(MIDI_NOTE_OFF, midi_ch, note, vel))
        time.sleep(sleep_time)

    info("End MIDI click test")

    del midiOut


def main():
    midi_speed_test()
    # midi_voice_test()
    # midi_click_test()
    pass


if __name__ == '__main__':
    args = parse_arguments()
    main()
