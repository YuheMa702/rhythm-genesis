# # src/python/beatmap_parser.py
# import json
# import librosa
# import os
# import numpy as np
# from random import choice

# def parse_audio_file(file_path):
#     try:
#         beatmap = process_song(file_path, 5, verbose=False)
#         return json.dumps(beatmap)
#     except Exception as e:
#         # In case of error, return a JSON error message.
#         return json.dumps({"error": str(e)})

# def process_song(filename, num_lanes, verbose=False):
#     # Load the audio as a waveform `y` and store the sampling rate as `sr`
#     y, sr = librosa.load(filename)
#     # Run the default beat tracker
#     tempo, beat_frames = librosa.beat.beat_track(y=y, sr=sr)

#     # Determine the onset strength of the beats
#     onset_env = librosa.onset.onset_strength(y=y, sr=sr)

#     threshold_strength = np.mean(onset_env) + 0.5 * np.std(onset_env)

#     # Get a list of frames where the onset strenght is greater than threshold
#     note_frames = np.where(onset_env > threshold_strength)[0]
#     note_times = librosa.frames_to_time(note_frames, sr=sr)
#     note_strengths = onset_env[note_frames]

#     # Calculate the average beat strength
#     beat_strengths = onset_env[beat_frames]

#     if (verbose):
#         print(list(map('Estimated tempo: {:.2f} beats per minute'.format,tempo)))
    
#     # Generate the beatmap
#     print("Generating beatmap...")
#     beatmap = build_beatmap(note_times, note_strengths, num_lanes)
#     return beatmap

# # Create a beatmap, which is a list of beatstamps
# def build_beatmap(beat_times, beat_strengths, num_lanes):
#     beatmap = []
#     for i in range(beat_strengths.size):
#         num_notes = choice([n for n in range(1, num_lanes + 1)])
#         beatstamp = create_json(beat_times[i].item(), num_notes, num_lanes)  # single note
#         beatmap.append(beatstamp)
#     return beatmap

# # Create a beatstamp, a tuple of the timestamp and the number of notes
# def create_json(beat_time, num_notes, num_lanes):
#     lanes = pick_lanes(num_notes, num_lanes)
#     beatstamp = {
#         "time": beat_time,
#         "num_notes": num_notes,
#         "lanes": lanes
#     }
#     return beatstamp

# def pick_lanes(num_notes, num_lanes):
#     lanes = []
#     for notes in range(num_notes):
#         lane = choice([i for i in range(0, num_lanes) if i not in lanes])
#         lanes.append(lane)
#     return lanes

# def clean_rhythm(note_times, tempo):
#     minimum_note_length = 0.05
#     cleaned_times = []
#     last_time = None
#     for time in note_times:
#         if last_time is None or time - last_time > minimum_note_length:
#             cleaned_times.append(time)
#             last_time = time
#     return cleaned_times

# src/python/beatmap_parser.py
import json
import librosa
import os
import numpy as np
from random import choice

def parse_audio_file(file_path):
    try:
        beatmap = process_song(file_path, 5, verbose=False)
        return json.dumps(beatmap)
    except Exception as e:
        # In case of error, return a JSON error message.
        return json.dumps({"error": str(e)})

def process_song(filename, num_lanes, verbose=False):
    # Load the audio as a waveform `y` and store the sampling rate as `sr`
    y, sr = librosa.load(filename)
    # Run the default beat tracker
    tempo, beat_frames = librosa.beat.beat_track(y=y, sr=sr)

    # Determine the onset strength of the beats
    onset_env = librosa.onset.onset_strength(y=y, sr=sr)

    threshold_strength = np.mean(onset_env) + 0.5 * np.std(onset_env)

    # Get a list of frames where the onset strenght is greater than threshold
    note_frames = np.where(onset_env > threshold_strength)[0]
    note_times = librosa.frames_to_time(note_frames, sr=sr)
    note_strengths = onset_env[note_frames]

    # Calculate the average beat strength
    beat_strengths = onset_env[beat_frames]

    if (verbose):
        print(list(map('Estimated tempo: {:.2f} beats per minute'.format,tempo)))
    
    # Generate the beatmap
    print("Generating beatmap...")
    beatmap = build_beatmap(note_times, note_strengths, num_lanes)
    return beatmap

# Create a beatmap, which is a list of beatstamps
def build_beatmap(beat_times, beat_strengths, num_lanes):
    beatmap = []
    minimum_note_length = 0.1
    last_time = None
    for i in range(beat_strengths.size):
        time = beat_times[i].item()
        if last_time is None or time - last_time > minimum_note_length:
            num_notes = choice([n for n in range(1, 3)])
            beatstamp = create_json(time, num_notes, num_lanes)
            beatmap.append(beatstamp)
            last_time = time
    return beatmap

# # Create a beatstamp, a tuple of the timestamp and the number of notes
# def create_json(beat_time, num_notes, num_lanes):
#     lanes = pick_lanes(num_notes, num_lanes)
#     beatstamp = {
#         "time": beat_time,
#         "num_notes": num_notes,
#         "lanes": lanes
#     }
#     return beatstamp

def create_json(beat_time, num_notes, num_lanes):
    lanes = pick_lanes(num_notes, num_lanes)
    beatstamp = {
        "time": round(beat_time, 4),  # Round to 4 decimal places
        "num_notes": num_notes,
        "lanes": lanes
    }
    return beatstamp


def pick_lanes(num_notes, num_lanes):
    lanes = []
    for notes in range(num_notes):
        lane = choice([i for i in range(0, num_lanes) if i not in lanes])
        lanes.append(lane)
    return lanes