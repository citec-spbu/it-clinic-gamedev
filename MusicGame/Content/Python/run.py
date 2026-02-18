import os
from pathlib import Path

# установить рабочую директорию = папка этого скрипта
os.chdir(Path(__file__).parent)
print("cwd:", os.getcwd())

import sys
sys.path.append(r"C:/Users/HP/Documents/Unreal Projects/it-clinic-gamedev/MusicGame/Content/Python/py_libs")

def run(wav_path, seq_path):
    import beatmark
    beatmark.run(wav_path)

    import clone_trigger_from_csv as ctc
    ctc.run(seq_path, r"C:\Users\HP\Documents\Unreal Projects\it-clinic-gamedev\MusicGame\Content\Python\beats_energy.csv")