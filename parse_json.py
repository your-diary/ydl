import sys
import json

input_fd = sys.stdin
json_dict = json.load(input_fd)

if __name__ == "__main__":
    print(eval("json_dict" + sys.argv[1]))

