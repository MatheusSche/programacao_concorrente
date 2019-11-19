import numpy as np
import time
import sys

if sys.version_info.major == 2:
    import Tkinter as tk
else:
    import tkinter as tk


UNIT = 40   # pixels
MAZE_H = 10  # grid height
MAZE_W = 10  # grid width


class Maze(tk.Tk, object):

    def __init__(self, controller):
        super(Maze, self).__init__()
        self.action_space = ['u', 'd', 'l', 'r']
        self.n_actions = len(self.action_space)
        self.title('Ecossistemas Threads')
        self.geometry('{0}x{1}'.format(MAZE_H * 80, MAZE_H * 40))
        self.control = controller
        self._build_maze()
        self.img_foca = tk.PhotoImage(file="focaT.png").subsample(30)
        self.img_alga = tk.PhotoImage(file="alga.png").subsample(30)
        self.img_tubarao = tk.PhotoImage(file="peixe.png").subsample(15)
        self.img_peixe = tk.PhotoImage(file="tubarao.png").subsample(90)
        self.imagens = {}

    def _build_maze(self):
        self.canvas = tk.Canvas(self, bg='blue',
                           height=MAZE_H * UNIT * 2,
                           width=MAZE_W * UNIT * 2)
        # Caixa de Texto
        ##################################################################
        self.tex = tk.Text(master=self.canvas, height=10, width=40)
        self.tex.place(x=430, y=200)
        # timer
        self.t = tk.StringVar()
        self.t.set("00:00:00")
        self.lb = tk.Label(self.canvas, textvariable=self.t)
        self.lb.config(font=("Courier 20 bold"))
        self.lb.place(x=660, y=5)

        # Botões e labels
        ##########################################################3
        self.title_app = tk.Label(self.canvas, text='Populações', width=16, height=1)
        self.title_app.place(x=430, y=5)
        self.title_app.config(font=("Courier", 12))

        # Tubarões
        ########################################################################
        self.title_tub = tk.Label(self.canvas, text='Tubarões:', width=12)
        self.title_tub.place(x=430, y=50)

        self.input_tub = tk.Entry(self.canvas, width=10)
        self.input_tub.place(x=550, y=50)
        self.input_tub.insert(tk.END, '1')
        ###########################################################################

        # Focas
        ########################################################################
        self.title_foc = tk.Label(self.canvas, text='Focas:', width=12)
        self.title_foc.place(x=430, y=73)

        self.input_foc = tk.Entry(self.canvas, width=10)
        self.input_foc.place(x=550, y=73)
        self.input_foc.insert(tk.END, '1')
        ###########################################################################

        # Peixes
        ########################################################################
        self.title_peixe = tk.Label(self.canvas, text='Peixes:', width=12)
        self.title_peixe.place(x=430, y=96)

        self.input_peixe = tk.Entry(self.canvas, width=10)
        self.input_peixe.place(x=550, y=96)
        self.input_peixe.insert(tk.END, '1')
        ###########################################################################

        # Algas
        ########################################################################
        self.title_algas = tk.Label(self.canvas, text='Algas:', width=12)
        self.title_algas.place(x=430, y=120)

        self.input_algas = tk.Entry(self.canvas, width=10)
        self.input_algas.place(x=550, y=120)
        self.input_algas.insert(tk.END, '1')
        ###########################################################################

        # Calorias
        ########################################################################
        self.title_cal = tk.Label(self.canvas, text='Calorias:', width=12)
        self.title_cal.place(x=430, y=143)

        self.input_cal = tk.Entry(self.canvas, width=10)
        self.input_cal.place(x=550, y=143)
        self.input_cal.insert(tk.END, '1000')
        ###########################################################################


        # Botao iniciar
        ##################################################################
        self.btn_ini = tk.Button(self.canvas,
                                 text='Iniciar',
                                 width=10,
                                 command=(lambda
                                              text='teste':
                                              self.control.teste(
                                              self.input_tub.get(),
                                              self.input_foc.get(),
                                              self.input_peixe.get(),
                                              self.input_algas.get(),

                                          )
                                          )
                                 )
        self.btn_ini.place(x=430, y=170)
        ############################################################

        # Botao parar
        ##################################################################
        self.btn_stop = tk.Button(self.canvas,
                                 text='Parar',
                                 width=10,
                                 command=(lambda
                                              text='teste':
                                          self.control.teste(
                                              self.input_tub.get(),
                                              self.input_foc.get(),
                                              self.input_peixe.get(),
                                              self.input_algas.get(),

                                          )
                                          )
                                 )
        self.btn_stop.place(x=530, y=170)
        ############################################################


        # create grids
        for c in range(0, MAZE_W * UNIT, UNIT):
            x0, y0, x1, y1 = c, 0, c, MAZE_W * UNIT
            self.canvas.create_line(x0, y0, x1, y1)
        for r in range(0, MAZE_H * UNIT, UNIT):
            x0, y0, x1, y1 = 0, r, MAZE_W * UNIT, r
            self.canvas.create_line(x0, y0, x1, y1)

        #Linhas externas
        self.line1 = self.canvas.create_line(0,400,400,400, width=1)
        self.line2 = self.canvas.create_line(0, 0, 400, 0, width=1)
        self.line3 = self.canvas.create_line(0, 0, 0, 400, width=1)
        self.line3 = self.canvas.create_line(400, 0, 400, 400, width=1)

        self.canvas.pack()

    def create_image(self, x, y, tipo, id):
        chave = 'chave_{}'.format(id)
        if tipo == 1:
            self.imagens[chave] = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_alga)
        elif tipo == 2:
            self.imagens[chave] = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_peixe)
        elif tipo == 3:
            self.imagens[chave] = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_foca)
        elif tipo == 4:
            self.imagens[chave] = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_tubarao)
    
    def delete_image(self, id):
        chave = 'chave_{}'.format(id)
        self.canvas.delete(self.imagens[chave])


    def delete(self):
        self.tex.delete('1.0', tk.END)
    
    def escreve(self, conteudo):
        self.tex.insert(tk.END, conteudo)


if __name__ == '__main__':
    env = Maze()
    env.mainloop()
