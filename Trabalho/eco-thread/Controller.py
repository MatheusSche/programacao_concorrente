from random import randint
import threading
from maze_env import Maze
from ecossistema import pos_ecossistema
from timer import TimerThread
import time
from ValidationThread import ValidationThread

bloquear = True

class IndividuoThread(threading.Thread):
    def __init__(self, id, calorias, nome, tipo, semaforo_validation, env, semaforo, teste, max_t):
        self.id = id
        
        self.nome = nome
        self.env = env
        self.tipo = tipo
        self.calorias = calorias
        self.position = None

        self.stop_this_thread = True
        self.death_state = False

        self.semaforo = semaforo
        self.semaforo_validation = semaforo_validation
        self.teste = teste
        self.max_t = max_t

        threading.Thread.__init__(self)

    def run(self):
        time.sleep(1)
        #with self.mutex:
        pos = randint(1,100)
        self.position = pos
        x = pos_ecossistema[pos][0]
        y = pos_ecossistema[pos][1]
        self.env.create_image(x,y, self.tipo, self.id)
        
        while(True):
            time.sleep(2)
            with self.teste:
                self.semaforo.acquire()
                try:
                    print('Thread {} -- Semaforo {} -- Calorias {}'.format(self.id,self.semaforo._value, self.calorias))
                    if self.stop_this_thread != False and self.calorias>0:
                        if self.tipo != 1 and self.tipo != 0:
                            pos = self.move(pos)
                            self.position = pos
                            x = pos_ecossistema[pos][0]
                            y = pos_ecossistema[pos][1]
                                
                            self.env.delete_image(self.id)
                            self.env.create_image(x,y, self.tipo, self.id)

                            
                        self.calorias-=100     

                    else:
                        print('Morreu')

                        self.env.delete_image(self.id)
                        self.position = 0 
                        self.calorias = 0
                        self.tipo = 0
                    
                    if self.semaforo._value == 0:
                        print('Liberou entrada')
                        self.semaforo_validation.release()
                             
                   

                except Exception as e:
                    print(e)
            time.sleep(1) 
            
        
        return  True
            
    
    def move(self, actual_position):
        not_valid = True
        next_position = None
        
        while(not_valid):
            pos = randint(0,3)
            ##########################################################################################3
            if pos == 0:
                next_position = actual_position + 10
            if pos == 1:
                next_position = actual_position + 1
            if pos == 2:
                next_position = actual_position - 10
            if pos == 3:
                next_position = actual_position - 1
            ###################################################################################################3
            
            #fora dos limites da matriz
            if next_position <= 0 or next_position > 100:
                not_valid = True
            
            #tenta passar de uma posição final 0 para uma final 1, exemplo 10 -> 11
            elif actual_position % 10 == 0 and next_position % 10 == 1:
                not_valid = True
            
            #o mesmo de sima mas ao contrario
            elif actual_position % 10 == 1 and next_position % 10 == 0:
                not_valid = True        
            
            else:
                not_valid = False
            #######################################################################################
        return next_position

class Controller:
    def __init__(self):
        self.env = Maze(self)
        self.env.mainloop()
    
    
    def main(self, tubarao, foca, peixe, alga, calorias):
        threads = []
        qtd_tub = int(tubarao)
        qtd_foca = int(foca)
        qtd_peixe = int(peixe)
        qtd_alga = int(alga)
        cal = int(calorias) 
      
        MAX_THREAD = qtd_foca + qtd_peixe + qtd_tub + qtd_alga
        nome = None

      
        semaforo = threading.Semaphore(MAX_THREAD)
        semaforo_validation = threading.Semaphore(0)
        teste = threading.Lock()
        

        thread_timer = TimerThread(596, self.env)
        thread_timer.start()
        #threads.append(thread_timer)
        next_id = 0
        
        #faz o numero de tubarao
        for i in range(qtd_tub):
            tipo = 4
            
            thread = IndividuoThread(next_id, 
                                    cal, 
                                    nome, 
                                    tipo, 
                                    semaforo_validation,
                                    self.env, 
                                    semaforo,
                                    teste=teste,
                                    max_t=MAX_THREAD)
            thread.start()
            threads.append(thread)
            next_id+=1

        # faz o numero de foca
        for i in range(qtd_foca):
            tipo = 3
            
            thread = IndividuoThread(next_id, cal, nome, tipo, semaforo_validation, self.env, semaforo,  teste=teste, max_t=MAX_THREAD)
            thread.start()
            threads.append(thread)
            next_id += 1

        # faz o numero de peixe
        for i in range(qtd_peixe):
            tipo = 2
            thread = IndividuoThread(next_id, cal, nome, tipo, semaforo_validation, self.env, semaforo,   teste=teste, max_t=MAX_THREAD)
            thread.start()
            threads.append(thread)
            next_id += 1
            
        # faz o numero de peixe
        for i in range(qtd_alga):
            tipo = 1
            thread = IndividuoThread(next_id, cal, nome, tipo, semaforo_validation, self.env, semaforo,   teste=teste, max_t=MAX_THREAD)
            thread.start()
            threads.append(thread)
            next_id += 1

        thread_validacao = ValidationThread(133, threads, semaforo_validation, semaforo, MAX_THREAD,  teste=teste)
        thread_validacao.start()


Controller()