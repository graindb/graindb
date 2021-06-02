#!/usr/bin/env python3
import argparse

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

font = {  # 'weight' : 'bold',
    'size': 14}

matplotlib.rc('font', **font)


# python3 scripts/plan_spectrum.py -o ~/Developer/graindb-benchmark/plan_spectrum/job/plan_spectrum_q4.pdf -j 0.0
# -l Duck Grain -p spectrum ~/Developer/graindb-benchmark/plan_spectrum/job/plan_spectrum_q4_duck.csv
# ~/Developer/graindb-benchmark/plan_spectrum/job/plan_spectrum_q4_grain.csv

def parse_args():
    parser = argparse.ArgumentParser(
        description='given multiple output file abostule paths, ' \
                    'plots the plan spectrum chart.')
    parser.add_argument('files', nargs='+', help='output.data files to plot')
    parser.add_argument('-o', '--output', help='output directory to save to')
    parser.add_argument('-l', '--labels', nargs='+', help='labels to plot')
    parser.add_argument('-w', '--width',
                        help="figure's width", type=float, default=2)
    parser.add_argument('-e', '--height',
                        help="figure's height", type=float, default=3)
    parser.add_argument('-j', '--jitter',
                        help="jitter to add", type=float, default=0.08)
    parser.add_argument('-m', '--marker',
                        help='the marker used in the figure', default='.')
    parser.add_argument('-f', '--fontsize', help='the font size')
    parser.add_argument('-p', '--plot',
                        choices=['spectrum', 'icost_correlation'], help='the type of plot',
                        default='spectrum')
    return parser.parse_args()


def get_data(data_file):
    x_data = []
    y_data = []
    with open(data_file) as f:
        for line in f:
            try:
                line = line.split("\n")[0]  # remove '\n'
                line = line.split(",")
                y_data.append(float(line[0]))  # /1000)
            except:
                pass  # first line
    y_data.sort()
    return y_data


def get_data_x(data_file):
    x_data = []
    y_data = []
    with open(data_file) as f:
        for line in f:
            line = line.split("\n")[0]  # remove '\n'
            line = line.split(",")
            x_data.append(float(line[3]))
            y_data.append(float(line[0]) / 1000)
    return y_data, x_data


def plot_spectrum(jitter, width, height, output_file, files, labels, marker_):
    plan_labels = []
    data_to_plot = []
    x = []
    number_plans = 0
    for idx, data_file in enumerate(files):
        data = get_data(data_file)
        data_to_plot.append(np.array(data))
        number_plans = len(data)
        x.append(idx)
        plan_labels.append(labels[idx])
    plt.figure(figsize=(width, height))
    plt.rcParams['axes.xmargin'] = 0
    ax = plt.gca()
    ax.grid
    ax.tick_params(axis='x', which='major', pad=3)
    # ax.set_xlabel('Plans', fontsize=12)
    ax.set_ylabel('Run time (ms)', fontsize=12)
    for i in x:
        x_axis = np.random.normal(1 + i, jitter, size=len(data_to_plot[i]))
        plt.plot(x_axis, data_to_plot[i], 'r.', alpha=0.8, marker=marker_)
    for idx, i in enumerate(x):
        x[idx] = i + 1
    plt.yticks(rotation=45, fontsize=12)
    plt.xticks(x, plan_labels, fontsize=12)
    plt.xlim(x[0] - 0.5, x[len(x) - 1] + 0.5)
    plt.savefig(fname=output_file, format='pdf',
                bbox_inches='tight')


def plot_icost_correlation(jitter, width, height, output_file, in_file, marker):
    data, x = get_data_x(in_file)
    plt.figure(figsize=(width, height))
    # plt.rcParams['axes.xmargin'] = 0
    ax = plt.gca()
    ax.grid
    ax.ticklabel_format(style='sci', axis='x', scilimits=(0, 0))
    # ax.tick_params(axis='x', which='major', pad=3)
    ax.set_xlabel('I-Cost', fontsize=15)
    ax.set_ylabel('Run time (secs)', fontsize=15)
    plt.plot(x, data, 'r.', alpha=0.8, marker=marker)
    step = int((max(x) - min(x)) / 5)
    ticks = [y for y in np.arange(min(x), max(x) + 1, step)]
    plt.savefig(fname=output_file, format='pdf', bbox_inches='tight')


def main():
    args = parse_args()
    if args.plot == 'spectrum' and len(args.files) != len(args.labels):
        # each file to a label
        return
    if args.plot == 'spectrum':
        plot_spectrum(args.jitter, args.width, args.height, args.output,
                      args.files, args.labels, args.marker)
    else:
        plot_icost_correlation(args.jitter, args.width, args.height,
                               args.output, args.files[0], args.marker)


if __name__ == '__main__':
    main()
