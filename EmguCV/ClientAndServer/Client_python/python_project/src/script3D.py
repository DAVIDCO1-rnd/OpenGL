# import matplotlib.pyplot as plt
# from matplotlib import cm
# from matplotlib.ticker import LinearLocator
# import numpy as np
#
# fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
#
# # Make data.
# X = np.arange(-5, 5, 0.25)
# Y = np.arange(-5, 5, 0.25)
# X, Y = np.meshgrid(X, Y)
# R = np.sqrt(X**2 + Y**2)
# Z = np.sin(R)
#
# # Plot the surface.
# surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
#                        linewidth=0, antialiased=False)
#
# # Customize the z axis.
# ax.set_zlim(-1.01, 1.01)
# ax.zaxis.set_major_locator(LinearLocator(10))
# # A StrMethodFormatter is used automatically
# ax.zaxis.set_major_formatter('{x:.02f}')
#
# # Add a color bar which maps values to colors.
# fig.colorbar(surf, shrink=0.5, aspect=5)
#
# plt.show()


import matplotlib.pyplot as plt
import numpy as np

x = np.array([1, 2, 3, 4, 5])
print(type(x))
y = np.array([1, 4, 9, 16, 25])
z = np.array([1, 8, 27, 64, 125])
# Creating the figure object
fig = plt.figure()
# keeping the projection = 3d
# ctreates the 3d plot
ax = plt.axes(projection = '3d')
ax.plot3D(z, y, x)
plt.show()