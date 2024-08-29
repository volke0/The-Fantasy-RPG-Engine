import os
import json
from pydub.utils import mediainfo

#directory = "./resources/music/"
directory = "../../resources/music/"
#database_dir = "./databases/music/loop_data.json"
database_dir = "./loop_data.json"
database = {}

def addToDatabase(meta_data, loop_start, loop_end, volume):
    if 'LOOPSTART' in meta_data['TAG']:
        loop_start = meta_data['TAG']['LOOPSTART']

    if 'LOOPEND' in meta_data['TAG']:
        loop_end = meta_data['TAG']['LOOPEND']
        
    if 'VOLUME' in meta_data['TAG']:
        volume = meta_data['TAG']['VOLUME']

    database[f'{name}'] = {'volume': float(volume), 'loop_start': float(loop_start), 'loop_end': float(loop_end)}

try: 
    with open(database_dir, "r") as jsonfile:
        j_file = json.load(jsonfile)
        for file in os.listdir(directory):
            name, ext = os.path.splitext(file)
            if (ext == ".ogg"):
                loop_end = 0.0
                loop_start = 0.0
                volume = 1
                if name not in j_file:
                    temp_meta_data = mediainfo(directory + f"{name}{ext}")
                    addToDatabase(temp_meta_data, loop_start, loop_end, volume)
                else:
                    temp_meta_data = mediainfo(directory + f"{name}{ext}")
                    loop_start = j_file[name]["loop_start"]
                    loop_end = j_file[name]["loop_end"]
                    volume = j_file[name]["volume"]
                    addToDatabase(temp_meta_data, loop_start, loop_end, volume)

        json_obj = json.dumps(database, indent=1)
        with open(database_dir, "w") as outputfile:
            outputfile.write(json_obj)
            jsonfile.close()

except FileNotFoundError:
    print("NOT FOUND")
    for file2 in os.listdir(directory):
        name, ext = os.path.splitext(file)
        if (ext == ".ogg"):
            temp_meta_data = mediainfo(directory + f"{name}{ext}")
            addToDatabase(temp_meta_data)


    json_obj = json.dumps(database, indent=1)

    with open(database_dir, "w") as outputfile:
        outputfile.write(json_obj)
        file2.close()




