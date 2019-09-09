from collections import namedtuple


def playing_channels(midi_file):
    channels = set()
    for msg in midi_file:
        if msg.type == 'note_off' or msg.type == 'note_on':
            channels.add(msg.channel)
    return channels


def update_channel_map(channel_map, prog_chg_msgs):
    """Add in program changes to `channel_map`."""
    for msg in prog_chg_msgs:
        channel_map[msg.channel] = msg.program


GroupMessage = namedtuple('GroupMessage', ['type', 'time', 'messages'])


def group_messages(msgs):
    """Groups `program_change` messages and `note_on` + `note_off`
    messages that fire at the same time.
    Returns a list of `GroupMessage`.
    """

    def group_type(msg):
        if msg.type in ['note_on', 'note_off']:
            return 'note'
        elif msg.type in ['program_change']:
            return 'program_change'

    grouped_msgs = []

    cur_msgs = []
    cur_type = None

    for msg in msgs:
        msg_type = group_type(msg)

        # for first iteration
        if not cur_type:
            cur_msgs = [msg]
            cur_type = msg_type

        # check group end conditions
        # separate and create new group if valid
        if (cur_type == 'program_change' and msg_type != cur_type) or \
           (cur_type == 'note' and (msg_type != cur_type or msg.time != 0)):
                grouped_msgs.append(
                    GroupMessage(cur_type, cur_msgs[0].time, cur_msgs)
                )
                cur_msgs = [msg]
                cur_type = msg_type
        # msg belongs to the current group
        elif msg_type == cur_type:
            cur_msgs.append(msg)

    # add last group
    grouped_msgs.append(
        GroupMessage(cur_type, cur_msgs[0].time, cur_msgs)
    )

    return grouped_msgs
