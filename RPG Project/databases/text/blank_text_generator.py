import json

temp_dict = {}

num = 255

for index in range(0, num+1):
	temp_dict[index] = {"text": "", "GOTO": "ENDA"}

j = json.dumps(temp_dict, indent=4)

with open("text.json", "w") as file:
	file.write(j)
