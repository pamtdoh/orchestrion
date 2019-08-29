import sys
from mido import MidiFile
from serial import Serial
from time import time_ns


def note_octave(number):
    octave = number // 12 - 1

    note_symbols = ('C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B')
    note = note_symbols[number % 12]

    return note + str(octave)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: ./player.py <port> <filename>')
        sys.exit(1)

    port = sys.argv[1]
    filename = sys.argv[2]

    try:
        midi_file = MidiFile(filename)
    except OSError:
        print(f'Could not open {filename}')
        sys.exit(2)

    start = time_ns()

    # channel_comp = {
    #     0: [0, 3],
    #     1: [1, 4],
    #     2: [5],
    #     3: [6],
    #     9: [2]
    # }

    channel_comp = {
        0: [3, 8, 9],
        1: [4, 10, 11],
        2: [5],
        3: [6],
        9: [2, 7]
    }

    with Serial(port, 115200) as ser:
        for message in midi_file.play():
            print(message)
            if message.type in ['note_on', 'note_off']:
                op = b'\x01' if message.type == 'note_on' else b'\x00'
                comp_ids = channel_comp.get(message.channel)
                if comp_ids is not None:
                    for comp_id in comp_ids:
                        ser.write(op + bytes([comp_id, message.note]) + b'\x00')
                        print(f'{comp_id}: {message.type} {message.channel} {message.note}')

    print(time_ns() - start)
