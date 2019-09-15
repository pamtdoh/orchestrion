#include <Wire.h>

constexpr int MAX_COMPONENTS = 16;
constexpr int CMD_BUFFER_LEN = 64;
constexpr int CMD_LEN = 2;


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


class CommandQueue {
public:
    void push(byte* cmd) {
        if (front == last && front != -1) return;
        if (front == -1) {
            front = 0;
            last = 0;
        }
        for (int i = 0; i < CMD_LEN; i++) {
            buffer[last + i] = cmd[i];
        }
        last = (last + CMD_LEN) % CMD_BUFFER_LEN;
    }

    void pop(byte* cmd) {
        if (front == -1) return;
        for (int i = 0; i < CMD_LEN; i++) {
            cmd[i] = buffer[front + i];
        }
        front = (front + CMD_LEN) % CMD_BUFFER_LEN;
        if (front == last) {
            front = -1;
            last = -1;
        }
    }

    bool available() {
        return front != -1;
    }

private:
    byte buffer[CMD_BUFFER_LEN];
    int front = -1;
    int last = -1;
};

/* commands */
void note_off(byte comp_id, byte note_val) {
    Component::get(comp_id)->note_off(note_val);
}

void note_on(byte comp_id, byte note_val) {
    Component::get(comp_id)->note_on(note_val);
}

void (*cmds[])(byte comp_id, byte data) = {note_off, note_on};
