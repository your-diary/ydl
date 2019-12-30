import sys
import json

input_fd = sys.stdin
json_dict = json.load(input_fd)

l = json_dict['items']
for i in l:
    print(i['id'])

