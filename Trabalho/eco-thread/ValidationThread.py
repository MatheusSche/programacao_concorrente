from random import randint
import threading
import time


class ValidationThread(threading.Thread):
    def __init__(self, id, all_threads, semaforo_validation, semaforo, max_thread, env):
        self.id = id
        self.all_threads = all_threads
        self.max_thread = max_thread
        self.stop_validation = True
        self.env = env

        self.semaforo_validation = semaforo_validation
        self.semaforo = semaforo
        
        threading.Thread.__init__(self)

    def run(self):
        placar = self.max_thread
        while(self.stop_validation):
            print('Tentando entrar')
            print(self.semaforo_validation._value)
            self.semaforo_validation.acquire()
            try:
                print('Entrou')
                for animal in self.all_threads:
                    
                    position_test = animal.position
                    type_test = animal.tipo
                 

                    for predador in self.all_threads:
                        
                        if position_test == predador.position:
                    
                            if type_test == 1 and predador.tipo == 2:
                                predador.calorias+=animal.calorias
                                animal.stop_this_thread = False
                                print('Peixe comeu a alga')
                                placar-=1
                            
                            elif type_test == 2 and predador.tipo == 3:
                                predador.calorias+=animal.calorias
                                animal.stop_this_thread = False
                                print('Foca comeu o peixe')
                                placar-=1

                            elif type_test == 2 and predador.tipo == 4:
                                predador.calorias+=animal.calorias
                                animal.stop_this_thread = False
                                print('Tubarão comeu o peixe')
                                placar-=1
                                
                            elif type_test == 3 and predador.tipo == 4:
                                predador.calorias+=animal.calorias
                                animal.stop_this_thread = False
                                print('Tubarão comeu a Foca')
                                placar-=1

                                
                self.env.p.set(placar)
                print(self.max_thread)
                print(placar)
                for i in range(self.max_thread):
                    self.semaforo.release()
                
                print('Terminou de liberar os animais')
               
               
            
            except Exception as e:
                print(e)
            
          