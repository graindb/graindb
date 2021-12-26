#!/usr/bin/env python3
import argparse

import matplotlib
import matplotlib.pyplot as plt

font = {  # 'weight' : 'bold',
    'size': 14}

matplotlib.rc('font', **font)


def parse_args():
    parser = argparse.ArgumentParser(
        description='Given input and output file absolute paths, plots the histogram chart. Input csv file contains '
                    '[DuckDB, GRainDB] for each query.')
    parser.add_argument('file', help='input file to plot')
    parser.add_argument('-o', '--output', help='output directory to save to')
    parser.add_argument('-t', '--title', help='plot title')
    parser.add_argument('-w', '--width',
                        help="figure's width", type=float, default=6)
    parser.add_argument('-e', '--height',
                        help="figure's height", type=float, default=4)
    return parser.parse_args()


def get_data(data_file):
    x_data = []
    y_data = []
    with open(data_file) as f:
        for line in f:
            try:
                line = line.split("\n")[0]  # remove '\n'
                line = line.split(",")
                x_data.append(float(line[0]))
                y_data.append(float(line[1]))
            except:
                pass  # first line
    return x_data, y_data


def plot_histogram(width, height, input_file, output_file, title):
    x_data, y_data = get_data(input_file)
    data = [x_data, y_data]
    plt.figure(figsize=(width, height))
    plt.rcParams['axes.xmargin'] = 0
    ax = plt.gca()
    ax.grid
    ax.tick_params(axis='both', which='both', pad=3, labelsize=14)
    n, bins, patches = plt.hist(data, 40, cumulative=True, alpha=0.0)
    print(n)
    print(bins)
    bin_width = (bins[1] - bins[0]) / 2
    x = []
    for bin_x in bins[:-1]:
        x.append(bin_x + bin_width)
    plt.plot(x, n[0], color='green', label='DuckDB', linestyle='dashed')
    plt.plot(x, n[1], color='orange', label='GRainDB')

    plt.plot(x[0], n[1][0], "^", color='orange')
    ax.set_ylabel('Num of plans', fontsize=18)
    ax.set_xlabel('Query execution time (in msec)', fontsize=18)
    ax.legend(fontsize='small', labelcolor=['green', 'orange'])
    ax.set_title(title, fontsize=20)
    plt.legend(prop={'size': 18})
    plt.show()
    # plt.savefig(fname=output_file, format='pdf',
    #             bbox_inches='tight')


def main():
    args = parse_args()
    plot_histogram(args.width, args.height, args.file, args.output, args.title)


if __name__ == '__main__':
    main()
