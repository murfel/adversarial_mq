import matplotlib.pyplot as plt

xs = list(map(int, input().split()))
plt.plot(xs)
plt.xlabel('DeleteMin index')
plt.ylabel('Rank Error')
plt.show()
