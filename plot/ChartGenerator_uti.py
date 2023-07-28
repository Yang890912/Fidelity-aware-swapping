import numpy as np
import math
import os
import latex
import matplotlib.pyplot as plt
import matplotlib.transforms
import matplotlib
from matplotlib.offsetbox import AnchoredOffsetbox, TextArea, HPacker, VPacker

class ChartGenerator:
    # data檔名 Y軸名稱 X軸名稱 Y軸要除多少(10的多少次方) Y軸起始座標 Y軸終止座標 Y軸座標間的間隔
    def __init__(self, dataName, Xlabel, Ylabel, Xpow, Ypow, Ystart, Yend, Yinterval):
        filename = '../data/ans/' + dataName

        if not os.path.exists(filename):
            print("file doesn't exist")
            return
        
        with open(filename, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            
        print("start generate", filename)
        
        
        # Ydiv, Ystart, Yend, Yinterval

        # color = [
        #     "#000000",
        #     "#006400",
        #     "#FF1493",
        #     "#7FFF00",   
        #     "#900321",
        # ]
        color = [
            "#FF0000",
            "#00FF00",   
            "#0000FF",
            "#000000",
            "#900321",
        ]
        # matplotlib.rcParams['text.usetex'] = True

        fontsize = 26
        Xlabel_fontsize = fontsize
        Ylabel_fontsize = fontsize
        Xticks_fontsize = fontsize
        Yticks_fontsize = fontsize
            
        # fig, ax = plt.subplots(figsize=(8, 6), dpi=600) 
        
        andy_theme = {
        # "axes.grid": True,
        # "grid.linestyle": "--",
        # "legend.framealpha": 1,
        # "legend.facecolor": "white",
        # "legend.shadow": True,
        # "legend.fontsize": 14,
        # "legend.title_fontsize": 16,
        "xtick.labelsize": 20,
        "ytick.labelsize": 20,
        "axes.labelsize": 20,
        "axes.titlesize": 20,
        "mathtext.fontset": "custom",
        "font.family": "Times New Roman",
        "mathtext.default": "default",
        "mathtext.it": "Times New Roman:italic",
        "mathtext.cal": "Times New Roman:italic",
        # "mathtext.fontset": "regular",
        # "figure.autolayout": True,
        "text.usetex": True,
        # "figure.dpi": 800
        }
        
        matplotlib.rcParams.update(andy_theme)
        # fig, ax1 = plt.subplots(figsize = (6, 4.5), dpi = 600)
        fig, ax1 = plt.subplots(figsize = (8, 6), dpi = 600)
        # ax1.spines['top'].set_position(('axes', 0.5)
        # ax1.spines['right'].set_linewidth(1.5)
        # ax1.spines['bottom'].set_linewidth(1.5)
        # ax1.spines['left'].set_linewidth(1.5)
        ax1.tick_params(direction = "in")
        ax1.tick_params(bottom = True, top = True, left = True, right = True)
        ax1.tick_params(pad = 20)

        ##data start##
        x = []
        _y = []
        numOfData = 0

        for line in lines:
            line = line.replace('\n','')
            if line[-1] == ' ':
                line = line[0 : -1]
            data = line.split(' ')
            numOfline = len(data)
            numOfData += 1

            for i in range(numOfline):
                if i == 0:
                    x.append(data[i])
                else:
                    if Ylabel.endswith("(\\%)"):
                        _y.append(str(float(data[i]) * 100))        
                    else:
                        _y.append(data[i])
        
        numOfAlgo = len(_y) // numOfData

        y = [[] for _ in range(numOfAlgo)]
        for i in range(numOfData * numOfAlgo):
            y[i % numOfAlgo].append(_y[i])

        # print(x)
        # print(y)

        maxData = 0
        minData = math.inf

        Ydiv = float(10 ** Ypow)
        Xdiv = float(10 ** Xpow)
        
        for i in range(numOfData):
            x[i] = float(x[i]) / Xdiv

        for i in range(numOfAlgo):
            for j in range(numOfData):
                y[i][j] = float(y[i][j]) / Ydiv
                maxData = max(maxData, y[i][j])
                minData = min(minData, y[i][j])

        marker = ['x', 'v', 'o', '^', '.']

        Per = [0, 2, 1, 3, 4]
        # for i in range(numOfAlgo - 1, -1, -1):
        for _ in range(numOfAlgo):
            i = Per[_]
            if i == 1:
                continue
            ax1.plot(x, y[i], color = color[i], lw = 2.5, linestyle = "-", marker = marker[i], markersize = 15, markerfacecolor = "none", markeredgewidth = 2.5, zorder = -_)
        # plt.show()

        plt.xticks(fontsize = Xticks_fontsize)
        plt.yticks(fontsize = Yticks_fontsize)
        
        Per_AlgoName = ["NM", "NM_LP", "FLTO", "Nesting", "Linear"]
        AlgoName = Per_AlgoName[0:1] + Per_AlgoName[2:]

        leg = plt.legend(
            AlgoName,
            loc = 10,
            bbox_to_anchor = (0.4, 1.2),
            prop = {"size": fontsize - 5, "family": "Times New Roman"},
            frameon = "False",
            labelspacing = 0.2,
            handletextpad = 0.2,
            handlelength = 1,
            columnspacing = 0.8,
            ncol = 2,
            facecolor = "None",
        )

        leg.get_frame().set_linewidth(0.0)
        # Ylabel += self.genMultiName(Ypow)
        Xlabel += self.genMultiName(Xpow)
        plt.subplots_adjust(top = 0.81)
        plt.subplots_adjust(left = 0.22)
        plt.subplots_adjust(right = 0.975)
        plt.subplots_adjust(bottom = 0.23)

        plt.yticks(np.arange(Ystart, Yend + Yinterval, step = Yinterval), fontsize = Yticks_fontsize)
        plt.xticks(x)
        plt.ylabel(Ylabel, fontsize = Ylabel_fontsize)
        plt.xlabel(Xlabel, fontsize = Xlabel_fontsize, labelpad = 500)
        ax1.yaxis.set_label_coords(-0.2, 0.5)
        ax1.xaxis.set_label_coords(0.45, -0.27)
        # plt.show()
        # plt.tight_layout()
        pdfName = dataName[0:-4].replace('#', '')
        plt.savefig('../data/pdf/Fie_{}.eps'.format(pdfName)) 
        # plt.savefig('../data/pdf/{}.jpg'.format(pdfName)) 
        # Xlabel = Xlabel.replace(' (%)','')
        # Xlabel = Xlabel.replace('# ','')
        # Ylabel = Ylabel.replace('# ','')
        plt.close()

    def genMultiName(self, multiple):
        if multiple == 0:
            return str()
        else:
            return "($" + "10" + "^{" + str(multiple) + "}" + "$)"

if __name__ == "__main__":
    # data檔名 Y軸名稱 X軸名稱 Y軸要除多少(10的多少次方) Y軸起始座標 Y軸終止座標 Y軸座標間的間隔
    # ChartGenerator("numOfnodes_waitingTime.txt", "need #round", "#Request of a round", 0, 0, 25, 5)
    Xlabels = ["num_nodes", "request_cnt", "time_limit", "avg_memory", "tao"]
    # Ylabels = ["fidelity_gain", "succ_request_cnt", "utilization"]
    Ylabels = ["utilization"]
    
    LabelsName = {}
    LabelsName["num_nodes"] = "\\#Nodes"
    LabelsName["request_cnt"] = "\\#Request"
    LabelsName["time_limit"] = "$|T|$"
    LabelsName["avg_memory"] = "Average Memory"
    LabelsName["tao"] = "$\\it{\\tau}$"
    LabelsName["fidelity_gain"] = " Fidelity Sum"
    LabelsName["succ_request_cnt"] = "\#Finished Request"
    LabelsName["utilization"] = "Memory Utilization (\%)"


    for Xlabel in Xlabels:
        for Ylabel in Ylabels:
            dataFileName = Xlabel + '_' + Ylabel + '.ans'
            Ystart = 0
            Yend = 0
            Yinternal = 0
            if Ylabel == "fidelity_gain":
                Ystart = 0
                Yend = 25
                Yinternal = 5
                if Xlabel == "tao" or Xlabel == "num_nodes":
                    Yend = 20
            elif Ylabel == "succ_request_cnt":
                Ystart = 0
                Yend = 25
                Yinternal = 5
                if Xlabel == "tao" or Xlabel == "num_nodes":
                    Yend = 20
            elif Ylabel == "utilization":
                Ystart = 0
                Yend = 25
                Yinternal = 5    
            ChartGenerator(dataFileName, LabelsName[Xlabel], LabelsName[Ylabel], 0, 0, Ystart, Yend, Yinternal)
    # Xlabel
    # 0 #RequestPerRound
    # 1 totalRequest
    # 2 #nodes
    # 3 r
    # 4 swapProbability
    # 5 alpha
    # 6 SocialNetworkDensity

    # Ylabel
    # 0 algorithmRuntime 
    # 1 waitingTime
    # 2 idleTime
    # 3 usedQubits
    # 4 temporaryRatio

    # beta = "$\\it{\\beta}$ (# Req. per Time Slot) "
    # waiting = "Avg. Waiting Time"
    # swap = "Succ. Prob. of Swap. $\\mathcal{Q(v)}$"
    # runtime = "Runtime (s)"
    # ratio = "Temp. Sto. Ratio (%)"
    # alpha = "$\\it{\\alpha}$ "
    # timeslot = "Time Slot"
    # remain = "# Remain. Req."
    # r = "Max. Sto. Time $\\it{r}$ (# Time Slots)"


    # # rpr + waiting
    # ChartGenerator(getFilename(0, 1), beta, waiting, 0, 0, 0, 15, 3)
    
    # # alpha + ratio
    # # ChartGenerator(getFilename(5, 4), alpha, ratio, -4, -2, 0, 100, 20)
    
    # # q + waiting
    # ChartGenerator(getFilename(4, 1), swap, waiting, 0, 0, 0, 125, 25)
    
    # # q + ratio
    # # ChartGenerator(getFilename(4, 4), swap, ratio, 0, -2, 0, 100, 20)

    # # alpha + waiting
    # ChartGenerator(getFilename(5, 1), alpha, waiting, -3, 0, 0, 30, 6)

    # # r + waiting
    # ChartGenerator(getFilename(3, 1), r, waiting, 0, 0, 1.9, 2.4, 0.2)
    
    # # timeslot + remain
    # ChartGenerator("Timeslot_#remainRequest.txt", timeslot, remain + " (%)", 0, -2, 0, 100, 20)