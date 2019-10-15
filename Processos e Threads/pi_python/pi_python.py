from threading import Thread
import sys

def calcula_pi(tid, ntermos, nth):
    print('Alo Thread: %d %d %d' % (tid, ntermos, nth))

if len(sys.argv) != 3:
    print('%s <num_threads>' % sys.argv[0])
    sys.exit()

ntermos = int(sys.argv[1])
nth = int(sys.argv[2])
tid = [None] * nth

for i in range(nth):
    tid[i] = Thread(target=calcula_pi, args=[i, ntermos, nth])
    tid[i].start()

for i in range(nth):
    tid[i].join()