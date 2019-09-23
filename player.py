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

comp_config = 'config/comp.ini'
slave_config = 'config/slave.ini'
instrument_config = 'config/default.ini'
settle_time = 0.0001

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: ./player.py <filename>')
        sys.exit(1)
    try:
        filename = sys.argv[1]
        midi_file = MidiFile(filename)
    except OSError:
        print(f'Could not open {filename}')
        sys.exit(2)

    # Begin building components, slave, and instrument mapping from config
    comp_dict = parse_component(comp_config)
    slave_dict = parse_slave(slave_config)
    instrument_dict = parse_instrument(instrument_config)

    for comp in comp_dict.values():
        slave = slave_dict[comp.slave_name]
        slave.add_component(comp)
        comp.set_slave(slave)

    for instrument in instrument_dict.values():
        instrument.add_comps(comp_dict)

    # Then, read and process midi file
    # Currently supports only `note_on`, `note_off`, and
    # `program_change` messages
    midi_file = MidiFile(filename)
    groups = group_messages(
        [msg for msg in midi_file
         if msg.type in ['note_on', 'note_off', 'program_change']]
    )

    # Setting initial `channel to prog num` mapping and `instrument to channel` mapping
    # Will be changed when playing through the midi file
    channels = playing_channels(midi_file)
    channel_map = {channel: 0 for channel in channels}
    instrument_map = InstrumentMap(instrument_dict, channel_map)

    def run_prog_change(messages):
        update_channel_map(channel_map, messages)
        instrument_map.build()
        print(channel_map)
        print(instrument_map.mapping)

    def run_note(messages):
        for message in messages:
            for inst in instrument_map.get(message.channel):
                if message.type == 'note_on':
                    inst.note_on(message.note)
                else:
                    inst.note_off(message.note)
        for slave in slave_dict.values():
            if slave.cmd_available():
                slave.build_stream()
                for attempts in range(3):
                    try:
                        print(f'{time.time() - start_time:7.3f}, {slave.name}: {slave.cmd_stream}')
                        slave.send_cmd_stream()
                        time.sleep(settle_time)
                    except OSError as e:
                        print(f'Send stream fail (attempt {attempts}): {e}')
                slave.empty_commands()

    # Playing the midi
    grp_count = len(groups)
    grp_index = 0
    start_time = time.time()
    next_time = time.time()
    while grp_index < grp_count:
        # Wait
        if time.time() < next_time:
            continue

        # Get current message group, update for next iter
        grp = groups[grp_index]
        grp_index += 1
        next_time += grp.time

        # Play current message group
        if grp.type == 'program_change':
            run_prog_change(grp.messages)
        elif grp.type == 'note':
            run_note(grp.messages)
        print(f'wait: {grp.time}s')
