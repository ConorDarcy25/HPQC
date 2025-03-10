import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import pandas as pd
import sys
import os

def generate_path(home_folder=os.path.expanduser("~"), subfolder='data', basename='animate_line', extension='gif'):
    """Creates file path for storing output."""
    output_folder = os.path.join(home_folder, subfolder)
    filename = basename + '.' + extension
    output_path = os.path.join(output_folder, filename)
    return output_path

def execute_time_step(data, rope, i):
    """Updates positions for animation."""
    x_positions, y_positions = extract_position(data, i)
    rope.set_data(x_positions, y_positions)
    return i

def reset_plots(rope, y_positions):
    """Resets plot data."""
    x_positions, y_positions = configure_rope()
    rope.set_data(x_positions, y_positions)

def animate(time, data, rope):
    """Handles animation updates."""
    x_positions, y_positions = extract_position(data, i=time)
    rope.set_data(x_positions, y_positions)
    return rope,

def configure_animation(frame_count=125, fps=25):
    """Configures animation parameters."""
    run_time = frame_count / fps
    interval = max(int(1000 / fps), 1)
    times = np.linspace(0, run_time, frame_count)
    return times, interval, fps

def configure_rope(length=50, step_size=1, start=0):
    """Configures the string model."""
    points = int(length / step_size) + 1
    x_positions = np.linspace(start, start + length, points)
    y_positions = np.zeros(points)
    return x_positions, y_positions

def configure_plot(x_positions, y_positions):
    """Sets up the plot."""
    fig, ax1 = plt.subplots()
    fig.suptitle('String Vibration Simulation')
    ax1.set_ylim(-1.1, 1.1)
    rope, = ax1.plot(x_positions, y_positions, "o", markersize=5, color="green")
    return fig, rope

def get_data(filename):
    """Reads data from a CSV file."""
    try:
        data = pd.read_csv(filename)
    except FileNotFoundError:
        print(f"File not found: {filename}")
        exit(1)
    return data, len(data.columns) - 2, len(data)

def extract_position(data, i=0):
    """Extracts positions from a data frame."""
    row = data.iloc[i]
    y_positions = np.array(row[2:])
    x_positions = np.arange(len(y_positions))
    return x_positions, y_positions

def get_file_name():
    """Gets the filename from command-line arguments."""
    if len(sys.argv) < 2:
        print("Usage: python script.py [FILENAME].csv")
        exit(1)
    filename = sys.argv[1]
    if not filename.endswith(".csv"):
        print("Error: File must be a CSV.")
        exit(1)
    return filename

def main():
    """Main function."""
    filename = get_file_name()
    data, num_positions, num_times = get_data(filename)
    times, interval, fps = configure_animation(frame_count=num_times)
    x_positions, y_positions = extract_position(data)
    fig, rope = configure_plot(x_positions, y_positions)
    ani = animation.FuncAnimation(fig, animate, num_times, interval=interval, blit=True, fargs=(data, rope))
    output_file = generate_path()
    ani.save(filename=output_file, writer="pillow", fps=fps)

if __name__ == "__main__":
    main()
