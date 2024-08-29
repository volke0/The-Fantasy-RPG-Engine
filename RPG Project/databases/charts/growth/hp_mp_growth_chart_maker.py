import json
import random

def generate_MP_Growth_Chart(start_lvl, max_mp, base_mp):
    #Generate MP Growth Chart
    temp_mp_list = []
    total_mp = base_mp
    max_lvl = 99 + 1
    base_gain = 4
    num = 0
    bonus = 0

    for lvl_num in range(start_lvl, max_lvl):
        diff_mp = max_mp - total_mp
        lvl_diff = max_lvl - lvl_num

        gain = round(base_gain + bonus)

        if (total_mp + (gain * lvl_diff) <= max_mp):
            bonus += .49

        if (total_mp + (gain * lvl_diff) > max_mp):
            bonus -= .55

        mp_gain = round(gain + bonus)
        if (lvl_num == start_lvl):
            mp_gain = 0
        if (lvl_num == 99):
            mp_gain = diff_mp
        total_mp += mp_gain

        temp_mp_dict = {"lvl": lvl_num, "mp_gain": mp_gain, "total_mp": total_mp}
        temp_mp_list.append(temp_mp_dict)

    return temp_mp_list
    

def generate_HP_Growth_Chart(start_lvl, max_hp, base_hp):
    #Generate HP Growth Chart
    max_lvl = 99 + 1
    total_hp = base_hp
    temp_hp_list = []
    for lvl_num in range(start_lvl, max_lvl):
        if lvl_num != max_lvl:
            diff_hp = max_hp - total_hp
            hp_per_lvl = round(diff_hp/(max_lvl - lvl_num))

            if lvl_num <=30:
                hp_per_lvl = hp_per_lvl - (hp_per_lvl*.65)

            if lvl_num < 50:
                hp_per_lvl = hp_per_lvl - (hp_per_lvl*.45)

            hp_per_lvl = round(hp_per_lvl - random.randrange(1, (int)(hp_per_lvl*.65)))

            if lvl_num >= 70:
                hp_per_lvl = round(diff_hp/(max_lvl - lvl_num))
                hp_per_lvl = round(hp_per_lvl - random.randrange(1, (int)(hp_per_lvl*.15)))

            if lvl_num == 99:
                hp_per_lvl = diff_hp

            if (lvl_num == start_lvl):
                hp_per_lvl = 0

            total_hp += hp_per_lvl
            temp_hp_dict = {"lvl": lvl_num, "hp_gain": hp_per_lvl, "total_hp": total_hp}
            temp_hp_list.append(temp_hp_dict)

    return temp_hp_list

class_name = str(input("Insert the name of the class (must already exist in the classes folder): "))
f = open(f"../../classes/{class_name}.json")

class_growth_dict = json.load(f)

f.close()

max_hp = class_growth_dict["max_stats"]["hp"]
max_mp = class_growth_dict["max_stats"]["mp"]

name = str(input("Insert the name of the unit (must already exist in the classes folder): "))
f = open(f"../../units/{name}.json")

unit_base_stats = json.load(f)
unit_base_stats = unit_base_stats["stats"]

f.close()

start_lvl = unit_base_stats["lvl"]
base_hp = unit_base_stats["hp"]
base_mp = unit_base_stats["mp"]

lvl_num = start_lvl + 1

temp_hp_list = []
temp_mp_list = []


temp_hp_list = generate_HP_Growth_Chart(start_lvl, max_hp, base_hp)
temp_mp_list = generate_MP_Growth_Chart(start_lvl, max_mp, base_mp)

class_growth_json_dict = {"hp": temp_hp_list, "mp": temp_mp_list}

class_growth_json = json.dumps(class_growth_json_dict, indent=4)

with open(f"{name}.json", "w") as output_file:
    output_file.write(class_growth_json) 




