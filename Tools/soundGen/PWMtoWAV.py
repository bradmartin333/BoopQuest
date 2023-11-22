#!/usr/bin/python
# based on : www.daniweb.com/code/snippet263775.html
import math
import wave
import struct
import re
import pyperclip

audio = []
sample_rate = 44100  # CD quality


def reset_wav():
    global audio
    audio = []
    return


def make_sounds(path):
    cpp = "std::vector<Music> sounds = {\n"

    with open(path, "r") as f:
        data = f.read()

    strings_to_remove = [" ", "\n", "\r", "void", "buzzer,"]
    for string_to_remove in strings_to_remove:
        data = re.sub(string_to_remove, "", data)

    for match in re.finditer(r"([^()]*)\(\){([^}]*)}", data):
        reset_wav()

        sound_name, raw_sound_data = match.groups()
        sound_data = raw_sound_data.split(";")

        for command in sound_data:
            if command.startswith("tone"):
                match = re.search(r".*\((\d+),(\d+)\)", command)
                append_sinewave(int(match.group(1)), int(match.group(2)))
            elif command.startswith("delay"):
                match = re.search(r".*\((\d+)\)", command)
                append_silence(int(float(match.group(1)) * 0.65))

        path = "/home/bmartin/BoopQuest/Web/res/sounds/"
        file = str(sound_name) + ".wav"
        save_wav(path + file)
        cpp = cpp + '\tLoadMusicStream("res/sounds/' + file + '"),\n'

    cpp = cpp[:-2] + "};"
    pyperclip.copy(cpp)
    return


def append_silence(duration_milliseconds):
    num_samples = duration_milliseconds * (sample_rate / 1000.0)

    for _ in range(int(num_samples)):
        audio.append(0.0)
    return


def append_sinewave(freq, duration_milliseconds, volume=0.5):
    global audio

    num_samples = duration_milliseconds * (sample_rate / 1000.0)

    for x in range(int(num_samples)):
        audio.append(volume * math.sin(2 * math.pi * freq * (x / sample_rate)))
    return


def save_wav(file_name):
    wav_file = wave.open(file_name, "w")
    nchannels = 1
    sampwidth = 2
    nframes = len(audio)
    comptype = "NONE"
    compname = "not compressed"
    wav_file.setparams((nchannels, sampwidth, sample_rate, nframes, comptype, compname))

    # WAV files here are using short, 16 bit, signed integers for the
    # sample size.  So we multiply the floating point data we have by 32767, the
    # maximum value for a short integer.  NOTE: It is theortically possible to
    # use the floating point -1.0 to 1.0 data directly in a WAV file but not
    # obvious how to do that using the wave module in python.
    for sample in audio:
        wav_file.writeframes(struct.pack("h", int(sample * 32767.0)))
    wav_file.close()
    return


make_sounds("arduinoSounds.txt")
