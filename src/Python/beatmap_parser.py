# src/python/beatmap_parser.py
import json
import librosa
import os
def parse_audio_file(file_path):
    # Example: use librosa to process the file (e.g., for beat detection) 
    # and return a JSON string for the beatmap.
    # For now return a static JSON for debugging purpose
    # beatmap = {
    #     "song": "example_song",
    #     "bpm": 60,
    #     "notes": [
    #         {"time": 1, "count": 2},
    #         {"time": 2, "count": 1},
    #         {"time": 3, "count": 2},
    #         {"time": 4, "count": 1},
    #         {"time": 5, "count": 2},
    #         {"time": 6, "count": 1},
    #         {"time": 7, "count": 2},
    #         {"time": 8, "count": 1},
    #         {"time": 9, "count": 2},
    #         {"time": 10, "count": 1},
    #         {"time": 11, "count": 2},
    #         {"time": 12, "count": 1},
    #         {"time": 13, "count": 2},
    #         {"time": 14, "count": 1},
    #         {"time": 15, "count": 2},
    #         {"time": 16, "count": 3},
    #         {"time": 17, "count": 1},
    #     ]
    # }
    # return json.dumps(beatmap)

    try:
        # Load the audio file
        y, sr = librosa.load(file_path, sr=None)
        
        # Extract tempo (bpm) and beat frame indices
        tempo, beat_frames = librosa.beat.beat_track(y=y, sr=sr)
        
        # Convert beat frames to time in seconds and cast to a list
        beat_times = librosa.frames_to_time(beat_frames, sr=sr).tolist()
        
        # Use the base name of the file as the song name
        song_name = os.path.basename(file_path)
        
        # Construct the beatmap dictionary with default intensity 1 for all beats.
        # Explicitly cast values to native float before rounding.
        beatmap = {
            "song": song_name,
            "bpm": round(float(tempo), 2),
            "notes": [{"time": round(float(t), 3), "intensity": 1} for t in beat_times]
        }
        
        return json.dumps(beatmap)
    except Exception as e:
        # In case of error, return a JSON error message.
        return json.dumps({"error": str(e)})