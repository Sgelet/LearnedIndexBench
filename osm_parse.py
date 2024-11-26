import sys
def parse(inp):
    data = inp.read(4)
    while data:
        print(int.from_bytes(data))
        data = inp.read(4)
        data = inp.read(4)

def main():
    for path in sys.stdin:
        file = open(path[:-1],mode="rb")
        parse(file)




if __name__ == "__main__":
    main()
