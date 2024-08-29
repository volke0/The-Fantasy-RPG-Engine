import json

name = input("Enter in the class name: ")
base = int(input("Enter in the base value for exp: "))
variation = input("Enter in the first amount of variation you want: ")
variation2 = input("Enter in the second amount of variation you want: ")

variation = int(variation)/100
variation2 = int(variation2)/100

total_exp = 0
gain = 2.50 + variation
base_pow = 2.00 + variation2
lvl_list = []

temp_dict = {"lvl": 1, "exp": 0}
lvl_list.append(temp_dict)
for i in range(2, 100):

    base = base + gain 
    total_exp = total_exp + round(pow(base, base_pow))
    temp_dict = {"lvl": i, "exp": total_exp}
    lvl_list.append(temp_dict)

exp_chart = {"EXP": lvl_list}

json_file = json.dumps(exp_chart, indent=4)

with open(f"{name}.json", "w") as file_out:
    file_out.write(json_file)

