import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import pandas as pd
import math as m
import random

from pathlib import Path
import sys

def generate_path(home_folder=str(Path.home()), subfolder='/data/', basename='output', extension='txt'):
    output_folder = home_folder + subfolder
    filename = basename + '.' + extension
    output_path = output_folder + filename
    return output_path

def execute_time_step(data, rope, i):
    x_positions, y_positions = extract_position(data, i=i)
    rope.set_data(x_positions, y_positions)
    return i

def reset_plots(rope, y_positions):
    x_positions, y_positions = configure_rope()
    rope.set_data(x_positions, y_positions)

def animate(time, data, rope):
    x_positions, y_positions = extract_position(data, i=time)
    rope.set_data(x_positions, y_positions)
    return rope,

def configure_animation(frame_count=125, fps=25):
    run_time = frame_count / fps
    interval = int(1000 / fps) if fps > 0 else 1
    time_step = interval / 1000
    times = np.linspace(0., run_time, int(frame_count))
    return times, interval, fps

def configure_rope(length=50, end_point=True, step_size=1, start=0):
    points = int((length / step_size) + end_point)
    stop = start + length
    x_positions = np.linspace(start=start, stop=stop, num=points, endpoint=end_point)
    y_positions = np.zeros(points)
    return x_positions, y_positions

def configure_plot(x_positions, y_positions):
    fig, ax1 = plt.subplots(1, 1)
    fig.suptitle('Simulation of Vibrations in a string')
    ax1.set_ylim(-1.1, 1.1)
    rope, = ax1.plot(x_positions, y_positions, "o", markersize=5, color="green", label="points on string")
    ax1.legend(handles=[rope], bbox_to_anchor=(0, 0), loc='upper left')
    return fig, rope

def get_data(filename, other=2):
    try:
        data = pd.read_csv(filename, skipinitialspace=True)
    except FileNotFoundError:
        print("Oh no! The file", filename, "is missing! Try again!")
        exit(1)
    num_positions = len(data.columns) - other
    num_times = len(data)
    return data, num_positions, num_times

def extract_position(data, i=0, other=2):
    row = data.iloc[i]
    y_positions = np.array(row[other:]).flatten()
    x_positions = np.arange(len(row) - other)
    return x_positions, y_positions

def get_file_name():
    if len(sys.argv) < 2:
        print("Oops! You forgot to add a filename. Try again!")
        exit(1)
    filename = sys.argv[1]
    if ".csv" not in filename:
        print("Umm... are you sure that's a CSV file?")
        exit(1)
    return filename

def main():
    filename = get_file_name()
    if len(sys.argv) > 2:
        output_basename = sys.argv[2]
    else:
        output_basename = "animate_string_file"
    if len(sys.argv) > 3:
        try:
            fps = int(sys.argv[3])
        except ValueError:
            print("Hey! FPS should be a number!")
            exit(1)
    else:
        fps = 25
    
    data, num_positions, num_times = get_data(filename)
    times, interval, fps = configure_animation(frame_count=num_times, fps=fps)
    x_positions, y_positions = extract_position(data)
    fig, rope = configure_plot(x_positions, y_positions)
    ani = animation.FuncAnimation(fig, animate, num_times, interval=interval, blit=True, fargs=(data, rope))
    output_file = generate_path(basename=output_basename, extension='gif')
    ani.save(filename=output_file, writer="pillow", fps=fps)
    print("Yay! Your animation is saved as", output_file)

if __name__ == "__main__":
    main()
