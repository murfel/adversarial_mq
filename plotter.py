import matplotlib.pyplot as plt
import statistics

xs = [1, 2, 3]
print(len(xs))

print('avg', statistics.mean(xs))
print('max', max(xs))

plt.plot(xs, 'bo', markersize=1)
plt.xlabel('DeleteMin operation serial number')
plt.ylabel('Rank Error')
plt.show()
