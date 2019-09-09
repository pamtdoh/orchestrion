class Command:
    opcodes = {
        'note_off': 0,
        'note_on': 1
    }

    def __init__(self, type, data):
        self.type = type
        self.data = data

    def to_bytes(self, comp_id):
        return bytes([
            comp_id << 4 | Command.opcodes[self.type],
            self.data
        ])

    def __repr__(self):
        return f'<{self.type}: {self.data}>'
