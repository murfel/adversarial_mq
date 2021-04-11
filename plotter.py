import matplotlib.pyplot as plt
import statistics

xs = [1, 2, 3]
print(len(xs))

print('abs avg', statistics.mean(map(abs, xs)))
print('neg avg', statistics.mean(filter(lambda x: x < 0, xs)))
print('pos avg', statistics.mean(filter(lambda x: x > 0, xs)))
print('num zeros', xs.count(0))

plt.plot(xs, 'bo', markersize=1)
plt.xlabel('DeleteMin operation serial number')
plt.ylabel('Rank Error (expected - actual)')
plt.show()
