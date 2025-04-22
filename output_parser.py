import sys

import matplotlib.pyplot as plt
import numpy as np
models = ["idx","pgm","opt"]
trans = {
    # Models
    "PGM" : ["Logar.", "blue", '-'],
    "IDX" : ["Dynamic", "orange", ':'],
    "OPT" : ["Dynamic", "orange", '-'],
    "NOSTORE" : ["NOSTORE", "", ''],
    "pgm" : ["Logar.", "blue", '-'],
    "idx" : ["Dynamic", "orange", ':'],
    "opt" : ["Dynamic", "orange", '-'],
    # Datasets
    "berlin" : ["BERLIN"],
    "books" : ["BOOKS"],
    "drifter" : ["DRIFTER"],
    "chicago" : ["CHICAGO"],
    "italy" : ["LONGITUDE"],
    "lines" : ["LINES"],
    "ran" : ["UNIF"],
    "m4" : ["TIMESERIES"]
}
def parse_linecount(file):
    name = file.name.split('/')[-1]
    res = {
        "name": name,
        "model": name.split('_')[2],
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
del_ratios = ("0.0", "0.25", "0.5", "0.75")
run_data = {
    'IDX' : {
        '0.0' : [],
        '0.25' : [],
        '0.5' : [],
        '0.75' : []
    },
    'PGM' : {
        '0.0' : [],
        '0.25' : [],
        '0.5' : [],
        '0.75' : []
    }
}

def parse_run(file):
    name = file.name.split('/')[-1]
    filesplit = name.split('_')
    model = "missing"
    for candidate in models:
        if candidate in filesplit:
            model = candidate
    if "adv" in filesplit[-1]:
        del_ratio = del_ratios
    else:
        del_ratio = [filesplit[-1].removesuffix(".run")]
    res = {
        "name": name,
        "model": model,
        "elem": [filesplit[-1].removesuffix(".run")],
        "val": [],
        "size" : 0
    }
    for line in file:
        line = line.rstrip()
        row = line.split(' ')

        if "Updates" in row[0]:
            for r in del_ratio:
                run_data[res["model"].upper()][r].append(float(row[-1]))
            return res

    return res



def make_plot(data,ratio="0.0"):
    plt.tight_layout()
    ls = ['-','--','-.',(0,(3,2,1,2,1,2)),':']
    cl = ["orange","blue"]
    fig, ax = plt.subplots()
    name = data[0]["name"].split("_")[0]
    extension = data[0]["name"].split('.')[-1]
    filename = name
    if "linecount" in extension:
        filename += "_linecount"
        for d in data:
            if d["model"] == "NOSTORE" or d["model"] == "IDX":
                continue
            ax.plot(d["elem"],d["val"],label = trans[d["model"]][0],c=trans[d["model"]][1],ls=trans[d["model"]][2])
        ax.set(xlabel="No. of elements",ylabel="No. of lines",title="Line count on "+trans[name][0]+" data")#,xlim=[49000,5e6+1000])#,ylim=[0,900])
    elif ".run" in data[0]["name"]:
        filename += "_updates"
        width = 0.25
        multiplier = 0
        x = np.arange(len(ratios))
        for model,data in run_data.items():
            val = data[ratio]
            if len(val) < len(ratios):
                return
            offset = width*multiplier
            rects = ax.bar(x+offset, val, width, label=trans[model][0],color=cl[multiplier])
            #ax.bar_label(rects, padding = 3)
            multiplier += 1
        ax.set(ylabel="Microseconds/operation",xlabel="Query ratio (wrt. updates)",title="Dynamic scenario on "+trans[name][0]+" data, with "+str(int(float(ratio)*100))+"% prior deletions")
        ax.set_xticks(x+width,ratios)
        ax.legend(loc="upper right")
        ax.legend()
        fig.savefig(filename+ratio+".png", dpi=300.)
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


    for r in del_ratios:
        make_plot(data,r)


if __name__ == "__main__":
    main()
