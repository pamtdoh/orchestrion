from collections import defaultdict
from functools import reduce
from smbus2 import SMBus

i2c_bus = SMBus(1)


class Slave:
    def __init__(self, name, type):
        self.name = name
        self.type = type
        self.components = {}
        self.cmd_buffer = defaultdict(list)
        self.cmd_stream = b''

    @staticmethod
    def factory(section):
        type = section['type']
        if type == 'i2c':
            return I2C_Slave.from_config(section)
        else:
            raise NotImplementedError

    def add_component(self, component):
        comp_id = component.comp_id
        self.components[comp_id] = component

    def add_command(self, comp_id, command):
        self.cmd_buffer[comp_id].append(command)

    def empty_commands(self):
        self.cmd_buffer = defaultdict(list)
        self.cmd_stream = b''

    def optimize_commands(self):
        for comp_id in self.cmd_buffer:
            comp = self.components[comp_id]
            cmds = self.cmd_buffer[comp_id]

            # Only include `note_on` if comp is `fast_return`
            if comp.fast_return:
                cmds = [cmd for cmd in cmds if cmd.type == 'note_on']

            # If comp can only play single note
            # Remove `note_off` if only switching notes
            if comp.single_note:
                switching = bool([cmd.data for cmd in cmds
                                  if cmd.type == 'note_on'])
                cmds = [cmd for cmd in cmds
                        if not (cmd.type == 'note_off' and switching)]

            self.cmd_buffer[comp_id] = cmds

    def pack_stream(self):
        self.cmd_stream = reduce(
            lambda x, y: x + y,
            [reduce(
                lambda x, y: x + y,
                [cmd.to_bytes(comp_id) for cmd in cmds],
                b''
                ) for comp_id, cmds in self.cmd_buffer.items()],
            b''
        )

    def build_stream(self):
        self.optimize_commands()
        self.pack_stream()

    def __str__(self):
        return self.name

    def cmd_available(self):
        return bool(self.cmd_buffer)

    def send_cmd_stream(self):
        raise NotImplementedError


class I2C_Slave(Slave):
    MAX_BLOCK_SIZE = 32

    def __init__(self, name, type, address):
        super().__init__(name, type)
        self.address = int(address)

    @staticmethod
    def from_config(section):
        return I2C_Slave(
            section.name,
            section['type'],
            section['i2c_address']
        )

    def __repr__(self):
        return f'<Slave {self.type}:{self.address} {self.name}>'

    def send_cmd_stream(self):
        length = len(self.cmd_stream)
        for start in range(0, length, I2C_Slave.MAX_BLOCK_SIZE):
            end = start + I2C_Slave.MAX_BLOCK_SIZE
            if end > length:
                end = length
            i2c_bus.write_i2c_block_data(
                self.address,
                self.cmd_stream[start],
                self.cmd_stream[start + 1: end]
            )
