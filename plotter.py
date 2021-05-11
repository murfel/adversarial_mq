from typing import List

import matplotlib.pyplot as plt
import numpy as np

# y_label = 'Ранг элемента'
# x_label = 'Порядковый номер операции «удалить минимум»'
# title = '16 очередей, |большая очередь| = 1\'000, |мал.| = 100'
# uni_equal_size_no_shuffle_label = '(Идеал) равн. заполнение'
# uni_no_shuffle_label = 'Без shuffle, равн. заполнение'
# adv_no_shuffle_label = 'Без shuffle, худ. заполнение'
# adv_perm_logN_label = 'После perm. shuffle, log N раз'
# adv_tree_logN_label = 'После tree shuffle, log N раз'

y_label = 'Rank'
x_label = 'DeleteMin serial number'
title = '16 queues, |big q| = 1\'000, |small q| = 100'
uni_equal_size_no_shuffle_label = 'Uni filling, equal q sizes (ideal)'
uni_no_shuffle_label = 'Uni. filling, no shuffle'
adv_no_shuffle_label = 'Adv. filling, no shuffle'
adv_perm_logN_label = 'Adv. filling, perm. shuffle'
adv_tree_logN_label = 'Adv. filling, tree shuffle'


def ranks_to_means_and_stds(ranks: List[int]):
    sz = len(ranks)
    arr = np.array(ranks)
    return [np.mean(arr[:i]) for i in range(1, sz)], [np.std(arr[:i]) for i in range(1, sz)]


def main():
    with open('cmake-build-debug/input.txt') as f:
        def my_read():
            return list(map(int, f.readline().strip().split()))
        uni_noshuf = my_read()
        uni_shuf = my_read()
        uni_shuf_buf_10 = my_read()
        uni_shuf_buf_15 = my_read()
        uni_shuf_buf_25 = my_read()

    # m1, std1 = ranks_to_means_and_stds(adv_noshuf)
    # m2, std2 = ranks_to_means_and_stds(adv_shuf)
    # m3, std3 = ranks_to_means_and_stds(adv_shuf_buf)

    m1, std1 = ranks_to_means_and_stds(uni_noshuf)
    m2, std2 = ranks_to_means_and_stds(uni_shuf)
    m3, std3 = ranks_to_means_and_stds(uni_shuf_buf_10)
    m4, std4 = ranks_to_means_and_stds(uni_shuf_buf_15)
    m5, std5 = ranks_to_means_and_stds(uni_shuf_buf_25)

    plt.plot(m1, color='red', markersize=1, label='no shuf')
    plt.plot(m3, color='orange', markersize=1, label='shuf, |buffer| = 10')
    plt.plot(m4, color='yellow', markersize=1, label='shuf, |buffer| = 15')
    plt.plot(m5, color='green', markersize=1, label='shuf, |buffer| = 25')
    plt.plot(m2, color='blue', markersize=1, label='shuf')
    # plt.plot(std4, '-', color='orange', markersize=1, label='std, shuf, |buffer| = 15')
    # plt.plot(std5, '-', color='darkgreen', markersize=1, label='std, shuf, |buffer| = 25')
    # plt.plot(std2, '-', color='darkblue', markersize=1, label='std, shuf')

    # plt.plot(ys5, 'bo', markersize=1, label=uni_equal_size_no_shuffle_label)
    # plt.plot(ys3, 'go', markersize=1, label=adv_tree_logN_label)
    # plt.plot(ys2, 'ro', markersize=1, label=adv_no_shuffle_label)
    # plt.plot(ys4, 'bo', markersize=1, label=adv_perm_logN_label)
    # plt.plot(ys1, 'bo', markersize=1, label=uni_no_shuffle_label)

    plt.xlabel('DeleteMin id', fontsize=14)
    plt.ylabel('Mean rank on a prefix', fontsize=14)
    plt.legend(fontsize=12, markerscale=10)
    plt.title('m=4,8,16,32 pqs, k elems per pq, k=5000, elems for 0 pq are inserted monot from 0 to k,\n'
              ' others inserts are uniform from k to m*k, p[push]=0.5,\n'
              'pqs are never empty (except for no-shuffle)', fontsize=10)
    plt.show()


if __name__ == '__main__':
    main()
