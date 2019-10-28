from random import randint
import threading
from maze_env import Maze
from ecossistema import pos_ecossistema
from timer import TimerThread

class IndividuoThread(threading.Thread):
    def __init__(self, id, calorias, nome, tipo, mutex, env):
        self.id = id
        self.mutex = mutex
        self.nome = nome
        self.env = env
        # 1 == Alga
        # 2 == Peixe
        # 3 == Foca
        # 4 == Tubarao
        self.tipo = tipo
        self.calorias = calorias
        threading.Thread.__init__(self)

    def run(self):
        #fará algo na thread
        with self.mutex:
            pos = randint(1,100)
            x = pos_ecossistema[pos][0]
            y = pos_ecossistema[pos][1]
            self.env.create_image(x,y, self.tipo)
            self.env.update()
            #self.env.after(100, self.run())


class Controller:
    def __init__(self):
        self.env = Maze(self)
        self.env.mainloop()
    def main(self):
        stdoutmutex = threading.Lock()
        threads = []

        for i in range(10):
            cal = 100
            nome = 'Algum'
            tipo = 1
            thread = IndividuoThread(i, cal, nome, tipo, stdoutmutex)
            thread.start()
            threads.append(thread)

        for thread in threads:
            thread.join()
    def teste(self, tubarao, foca, peixe, alga):
        threads = []
        stdoutmutex = threading.Lock()
        qtd_tub = int(tubarao)
        qtd_foca = int(foca)
        qtd_peixe = int(peixe)
        qtd_alga = int(alga)

        thread_timer = TimerThread(596, self.env)
        thread_timer.start()
        threads.append(thread_timer)

        #faz o numero de tubarao
        for i in range(qtd_tub):
            cal = 100
            nome = 'Tubarao'
            tipo = 4
            thread = IndividuoThread(i, cal, nome, tipo, stdoutmutex, self.env)
            thread.start()
            threads.append(thread)

        # faz o numero de foca
        for i in range(qtd_foca):
            cal = 100
            nome = 'Foca'
            tipo = 3
            thread = IndividuoThread(i, cal, nome, tipo, stdoutmutex, self.env)
            thread.start()
            threads.append(thread)

        # faz o numero de peixe
        for i in range(qtd_peixe):
            cal = 100
            nome = 'Peixe'
            tipo = 2
            thread = IndividuoThread(i, cal, nome, tipo, stdoutmutex, self.env)
            thread.start()
            threads.append(thread)
            # faz o numero de peixe
        for i in range(qtd_alga):
            cal = 100
            nome = 'Alga'
            tipo = 1
            thread = IndividuoThread(i, cal, nome, tipo, stdoutmutex, self.env)
            thread.start()
            threads.append(thread)

        #for thread in threads:
        #    thread.join()

Controller()