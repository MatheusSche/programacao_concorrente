from random import randint
import threading
import time


class ValidationThread(threading.Thread):
    def __init__(self, id, all_threads, lock, semaforo, max_thread):
        self.id = id
        self.all_threads = all_threads
        self.lock = lock
        self.semaforo = semaforo
        self.max_thread = max_thread
        
        threading.Thread.__init__(self)

    def run(self):
        while(1):
            time.sleep(1)
            posi_animais = {}
            
            for animal in self.all_threads:
                chave = 'chave_{}'.format(animal.position)
            
                try:
                    posi_animais[chave].append(animal)
                except:
                     posi_animais[chave] = []
                     posi_animais[chave].append(animal)

            for i in range(self.max_thread + 1):
                self.semaforo.release()
            time.sleep(1)
          