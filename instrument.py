from helper import comma_split

program_range_map = {
    'piano':                range(8),
    'chromatic_percussion': range(8, 16),
    'organ':                range(16, 24),
    'guitar':               range(24, 32),
    'bass':                 range(32, 40),
    'strings':              range(40, 48),
    'ensemble':             range(48, 56),
    'brass':                range(56, 64),
    'reed':                 range(64, 72),
    'pipe':                 range(72, 80),
    'synth_lead':           range(80, 88),
    'synth_pad':            range(88, 96),
    'synth_effects':        range(96, 104),
    'ethnic':               range(104, 112),
    'percussive':           range(112, 120),
    'sound_effects':        range(120, 128)
}


def program_range(name):
    return program_range_map[name]


class Instrument:
    def __init__(self, name,
                 percussions=False, comp_names=None, programs=None):
        self.name = name
        self.percussions = percussions
        self.programs = programs
        self.comp_names = comp_names
        self.comps = []
        self.note_comp = {}

    @staticmethod
    def from_config(section):
        if section.getboolean('percussions'):
            return Instrument(
                section.name,
                True,
                comma_split(section['components']),
                None
            )
        else:
            program_lists = map(program_range,
                                comma_split(section['programs']))
            programs = []
            for program_list in program_lists:
                programs += program_list

            return Instrument(
                section.name,
                False,
                comma_split(section['components']),
                programs
            )

    def add_comps(self, comp_dict):
        for comp_name in self.comp_names:
            comp = comp_dict[comp_name]
            self.comps.append(comp)

    def note_on(self, note_val):
        for comp in self.comps:
            if comp.can_play(note_val):
                comp.note_on(note_val)
                self.note_comp[note_val] = comp
                break
        else:
            print(f'{self}: Cannot play {note_val}')

    def note_off(self, note_val):
        if note_val in self.note_comp:
            comp = self.note_comp[note_val]
            comp.note_off(note_val)
            del self.note_comp[note_val]

    def __repr__(self):
        return f'<Instrument {self.name}>'


class InstrumentMap:
    def __init__(self, inst_dict, channel_map):
        self.inst_dict = inst_dict
        self.channel_map = channel_map
        self.mapping = {channel: [] for channel in channel_map}

    def build(self, fill_all=True):
        available_inst = list(self.inst_dict.keys())
        available_channel = list(self.channel_map.keys())

        # Assign percussions
        if 9 in self.channel_map:
            valid_inst = [name for name in available_inst
                          if self.inst_dict[name].percussions]
            self.mapping[9] += [self.inst_dict[name] for name in valid_inst]
            for name in valid_inst:
                available_inst.remove(name)
            available_channel.remove(9)

        # Assign primary instruments according to free channels
        for channel in available_channel.copy():
            valid_inst = [name for name in available_inst
                          if self.channel_map[channel] in
                          self.inst_dict[name].programs]
            if valid_inst:
                first = valid_inst[0]
                self.mapping[channel].append(self.inst_dict[first])
                available_inst.remove(first)
                available_channel.remove(channel)

        # Melody fallback
        for channel in available_channel.copy():
            if not available_inst:
                break
            first = available_inst[0]
            self.mapping[channel].append(self.inst_dict[first])
            available_inst.remove(first)
            available_channel.remove(channel)

    def get(self, channel):
        if channel in self.mapping:
            return self.mapping[channel]
