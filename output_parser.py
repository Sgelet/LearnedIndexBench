import sys

import matplotlib.pyplot as plt
import numpy as np

trans = {
    "PGM" : ["PGM", "blue", '-'],
    "IDX" : ["IDX", "orange", ':'],
    "OPT" : ["IDX", "orange", '-'],
    "NOSTORE" : ["NOSTORE", "", '']
}
def parse_linecount(file):
    res = {
        "name": file.name,
        "model": file.name.split('_')[-2][1:],
        "elem": [],
        "val": [],
        "size" : 0
    }

    for line in file:
        line = line.rstrip()
        row = line.split(' ')

        if row[0].isdigit():
            res["elem"].append(int(row[0]))
            res["val"].append(int(row[-1]))
            continue

        if "size" in line or "sixe" in line:
            res["size"] = int(row[-1])
            continue

    return res


# Parsing is stupid, added manually
ratios = ("0.0", "0.25", "0.5", "0.75", "1.0", "ADV")
run_data = {
    'IDX' : [],
    'PGM' : []
}

def parse_run(file):
    res = {
        "name": file.name,
        "model": file.name.split('_')[-2],
        "elem": [file.name.split('_')[-1].removesuffix(".run")],
        "val": [],
        "size" : 0
    }
    for line in file:
        line = line.rstrip()
        row = line.split(' ')

        if "Updates" in row[0]:
            run_data[res["model"].upper()].append(float(row[-1]))
            return res

    return res



def make_plot(data):
    plt.tight_layout()
    ls = ['-','--','-.',(0,(3,2,1,2,1,2)),':']
    cl = ["orange","blue"]
    fig, ax = plt.subplots()
    filename = "default"
    if "linecount" in data[0]["name"]:
        filename = "linecount"
        for d in data:
            if d["model"] == "NOSTORE" or d["model"] == "IDX":
                continue
            ax.plot(d["elem"],d["val"],label = trans[d["model"]][0],c=trans[d["model"]][1],ls=trans[d["model"]][2])
        ax.set(xlabel="No. of elements",ylabel="No. of lines",title="Line count on LINES data")#,xlim=[49000,5e6+1000])#,ylim=[0,900])
    elif ".run" in data[0]["name"]:
        filename = "updates"
        width = 0.25
        multiplier = 0
        x = np.arange(len(ratios))
        for model,val in run_data.items():
            offset = width*multiplier
            rects = ax.bar(x+offset, val, width, label=model,color=cl[multiplier])
            #ax.bar_label(rects, padding = 3)
            multiplier += 1
        ax.set(ylabel="Microseconds/operation",xlabel="Query ratio (wrt. updates)",title="Dynamic scenario on UNIF data")
        ax.set_xticks(x+width,ratios)

    ax.legend(loc="upper right")
    ax.legend()
    fig.savefig(filename+".png", dpi=300.)

def main():
    # Parse
    data = []
    for path in sys.stdin:
        file = open(path[:-1])
        if file.name.__contains__("linecount"):
            data += [parse_linecount(file)]
        elif file.name.__contains__("run"):
            data += [parse_run(file)]


    make_plot(data)


if __name__ == "__main__":
    main()