import mido
from pynput.keyboard import Controller
import time

keyboard = Controller()

midi_note_to_key = {
    53: 'r',
    55: 't',
    57: 'y',
    59: 'u',
    60: 'i' 
}


with mido.open_input(mido.get_input_names()[0]) as port:
    for msg in port:
        if msg.type == 'note_on':
            if(msg.note in midi_note_to_key):
                keyboard.press(midi_note_to_key[msg.note])
        elif msg.type == 'note_off':
            if(msg.note in midi_note_to_key):
                keyboard.release(midi_note_to_key[msg.note])