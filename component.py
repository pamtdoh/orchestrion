from command import Command


def parse_range(range_str):
    def set_range(s):
        if s.isdigit():
            return {int(s)}
        else:
            start, stop = [int(n) for n in s.split('-')]
            return set(range(start, stop + 1))

    ranges = set()
    for s in range_str.split(','):
        ranges |= set_range(s.strip())
    return ranges


class Component:
    def __init__(self, name, slave_name, comp_id, single_note=False,
                 fast_return=False, note_range='0-127'):
        self.name = name
        self.slave_name = slave_name
        self.comp_id = comp_id
        self.single_note = single_note
        self.fast_return = fast_return
        self.note_range = parse_range(note_range)
        self.notes_playing = []

    def set_slave(self, slave):
        self.slave = slave

    @staticmethod
    def from_config(section):
        return Component(
            section.name,
            section['slave'],
            section.getint('comp_id'),
            section.getboolean('single_note'),
            section.getboolean('fast_return'),
            section['note_range']
        )

    def __repr__(self):
        return f'<Component {self.slave}:{self.comp_id} {self.name}>'

    def can_play(self, note_val):
        return note_val in self.note_range and \
               note_val not in self.notes_playing and \
               not (self.single_note and self.notes_playing)

    def note_on(self, note_val):
        self.slave.add_command(self.comp_id, Command('note_on', note_val))
        self.notes_playing.append(note_val)

    def note_off(self, note_val):
        self.slave.add_command(self.comp_id, Command('note_off', note_val))
        self.notes_playing.remove(note_val)
