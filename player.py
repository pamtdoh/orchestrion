import sys
import time
from mido import MidiFile
from midi_processing import (
    playing_channels, update_channel_map, group_messages
)
from parse import (
    parse_component, parse_slave, parse_instrument
)
from instrument import InstrumentMap

comp_config = 'comp.ini'
slave_config = 'slave.ini'
instrument_config = 'default.ini'
test_midi = 'midi_files/Portal_Still-Alive_acoccimi.mid'

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: ./player.py <filename>')
        sys.exit(1)

    try:
        # filename = sys.argv[1]
        filename = test_midi
        midi_file = MidiFile(filename)
    except OSError:
        print(f'Could not open {filename}')
        sys.exit(2)

    comp_dict = parse_component(comp_config)
    slave_dict = parse_slave(slave_config)
    instrument_dict = parse_instrument(instrument_config)

    for comp in comp_dict.values():
        slave = slave_dict[comp.slave_name]
        slave.add_component(comp)
        comp.set_slave(slave)

    for instrument in instrument_dict.values():
        instrument.add_comps(comp_dict)

    # Currently supports only `note_on`, `note_off`, and
    # `program_change` messages
    midi_file = MidiFile(filename)
    messages = [msg for msg in midi_file
                if msg.type in ['note_on', 'note_off', 'program_change']]
    messages = group_messages(messages)

    # Hold channel to prog and instrument to channel mapping
    channels = playing_channels(midi_file)
    channel_map = {channel: 0 for channel in channels}
    instrument_map = InstrumentMap(instrument_dict, channel_map)

    for grp in messages:
        if grp.type == 'program_change':
            update_channel_map(channel_map, grp.messages)
            instrument_map.build()
            print(channel_map)
            print(instrument_map.mapping)
        elif grp.type == 'note':
            for message in grp.messages:
                for inst in instrument_map.get(message.channel):
                    if message.type == 'note_on':
                        inst.note_on(message.note)
                    else:
                        inst.note_off(message.note)
            for slave in slave_dict.values():
                slave.send_commands()
            print(f'wait: {grp.time}s')
