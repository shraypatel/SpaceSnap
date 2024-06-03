import serial
import math
import open3d as o3d
import numpy as np

# Open serial communication with the time-of-flight sensor
ser = serial.Serial("COM3", 115200)
print("Establishing connection with sensor:", ser.name)

# Parameters for data collection
num_z = 3
samples = 64

# List to store collected data points
point_cloud_data = []

# Open a file for writing the point cloud data
output_file = open("./pointcloud.xyz", "w")

# Collect data from the sensor
for z_measurement in range(num_z):
    print(f"Collecting data for z-measurement {z_measurement + 1}/{num_z}")

    for sample_index in range(samples):
        distance = 0
        ser.write(b'1')  # Send trigger command to the sensor

        # Read distance data until a comma is encountered
        while True:
            byte_data = ser.read().decode()
            if byte_data == ",":
                break
            else:
                distance = int(str(distance) + str(byte_data))

        # Calculate Cartesian coordinates from polar coordinates
        angle = sample_index * (360.0 / samples)
        x_coordinate = round(distance * math.cos(math.radians(angle)), 1)
        y_coordinate = round(distance * math.sin(math.radians(angle)), 1)

        # Append the data point to the list
        point_cloud_data.append([x_coordinate, y_coordinate, 100 * z_measurement])

# Close the serial connection
print("Closing serial connection:", ser.name)
ser.close()

# Write the collected data points to the output file
for point in point_cloud_data:
    output_file.write(f"{point[0]} {point[1]} {point[2]}\n")

# Close the output file
output_file.close()

### VISUALIZE IN OPEN3D


pcd = o3d.io.read_point_cloud("./pointcloud.xyz", format='xyz')

lines = []

# create loops
for z in range(num_z):

    offset = samples * z

    for i in range(samples):
        if i < samples - 1:
            lines.append([i + offset, i+1 + offset])
        else:
            lines.append([i + offset, offset])


# connect the loops
for z in range(num_z-1):
    startIndex = z*samples
    nextStartIndex = (z+1)*samples

    for i in range(samples):
        lines.append([startIndex+i, nextStartIndex+i])


pts = np.asarray(pcd.points)
print(pts)


line_set = o3d.geometry.LineSet(
    points=o3d.utility.Vector3dVector(pts),
    lines=o3d.utility.Vector2iVector(lines),
)

o3d.visualization.draw_geometries([line_set], width=1280, height=720)