import subprocess
import matplotlib.pyplot as plt
import numpy as np

subprocess = subprocess.Popen("./bin/main print", shell=True, stdout=subprocess.PIPE)
subprocess_return = subprocess.stdout.read()

info = subprocess_return.decode("utf-8")

trajectory = [[float(e) for e in s.split(",")] for s in info.split("\n") if s != ""]
dt = 0.01

# x axis for time-based plots
time_range = np.linspace(0, len(trajectory) * dt, len(trajectory))

# path
plt.subplot(2, 2, 1)
plt.title("Path")
plt.axis("equal")

x = [step[0] for step in trajectory]
y = [step[1] for step in trajectory]
plt.plot(x, y, label="Trajectory")

# plt.grid() # grid in path.plot
plt.xlabel("x (m)")
plt.ylabel("y (m)")
plt.grid()
plt.legend()

# angle
plt.subplot(2, 2, 2)
plt.title("Angle")

a = [step[2] for step in trajectory]
plt.plot(time_range, a)

plt.xlabel("Time (s)")
plt.ylabel("Degrees")
plt.grid()

# # curvature
# plt.subplot(2, 3, 3)
# plt.title("Curvature")

# c = [step[3] for step in trajectory]
# plt.plot(time_range, c)

# plt.xlabel("Time (s)")
# plt.ylabel("Curvature (1/r)")
# plt.grid()

# velocity
plt.subplot(2, 2, 3)
plt.title("Velocity")

v = [step[3] for step in trajectory]
plt.plot(time_range, v, label="Raw")
v = [step[4] for step in trajectory]
plt.plot(time_range, v, label="Limited")

plt.xlabel("Time (s)")
plt.ylabel("Velocity (m/s)")
plt.grid()
plt.legend()

# # angular velocity
# plt.subplot(2, 3, 5)
# plt.title("Angular Velocity")

# w = [step[6] for step in trajectory]
# plt.plot(time_range, w)

# plt.xlabel("Time (s)")
# plt.ylabel("Angular Velocity (rad/s)")
# plt.grid()

# wheel speeds
plt.subplot(2, 2, 4)
plt.title("Wheel Speeds")

plt.plot(time_range, [step[5] for step in trajectory], label="Left")
plt.plot(time_range, [step[6] for step in trajectory], label="Right")

plt.xlabel("Time (s)")
plt.ylabel("Velocity (m/s)")
plt.grid()
plt.legend()

plt.gcf().set_tight_layout(True)
plt.show()
