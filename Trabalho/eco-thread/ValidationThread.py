from random import randint
import threading
import time


class ValidationThread(threading.Thread):
    def __init__(self, id, all_threads, semaforo_validation, semaforo, max_thread, teste):
        self.id = id
        self.all_threads = all_threads
        self.max_thread = max_thread

        self.semaforo_validation = semaforo_validation
        self.semaforo = semaforo
        self.teste = teste
        
        threading.Thread.__init__(self)

    def run(self):
        while(1):
            print('Tentando entrar')
            print(self.semaforo_validation._value)
            self.semaforo_validation.acquire()
            try:
                print('Entrou')
                for animal in self.all_threads:
                    position_test = animal.position
                    type_test = animal.tipo
                    for animal_teste in self.all_threads:
                        if position_test == animal_teste.position:
                            #alga encontra peixe e morre
                            if type_test == 1 and animal_teste.tipo == 2:
                                animal.stop_this_thread = False
                                print('alga foi comida')

                for i in range(self.max_thread):
                    self.semaforo.release()
                
                print('Terminou de liberar os animais')
               
               
            
            except Exception as e:
                print(e)
            
          