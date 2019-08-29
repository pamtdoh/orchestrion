#include <Wire.h>

constexpr int MAX_COMPONENTS = 8;
constexpr int CMD_LEN = 4;
constexpr int DATA_LEN = 3;


class Component {
public:
    virtual void note_on(byte note) = 0;
    virtual void note_off(byte note) = 0;

    static void map(byte compId, Component* component) {
        compMap[compId] = component;
    }
    static Component* get(byte compId) {
        return compMap[compId];
    }
private:
    static Component* compMap[];
};

Component* Component::compMap[MAX_COMPONENTS] = {0};


/* commands */
void note_off(byte* data) {
    byte compId = data[0];
    byte note_val = data[1];
    Component::get(compId)->note_off(note_val);
}

void note_on(byte* data) {
    byte compId = data[0];
    byte note_val = data[1];
    Component::get(compId)->note_on(note_val);
}

void (*cmds[])(byte* data) = {note_off, note_on};
