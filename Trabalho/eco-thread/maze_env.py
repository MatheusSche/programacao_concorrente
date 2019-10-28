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
        self.lb.place(x=650, y=5)

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
        self.input_tub.insert(tk.END, '10')
        ###########################################################################

        # Focas
        ########################################################################
        self.title_foc = tk.Label(self.canvas, text='Focas:', width=12)
        self.title_foc.place(x=430, y=73)

        self.input_foc = tk.Entry(self.canvas, width=10)
        self.input_foc.place(x=550, y=73)
        self.input_foc.insert(tk.END, '10')
        ###########################################################################

        # Peixes
        ########################################################################
        self.title_peixe = tk.Label(self.canvas, text='Peixes:', width=12)
        self.title_peixe.place(x=430, y=96)

        self.input_peixe = tk.Entry(self.canvas, width=10)
        self.input_peixe.place(x=550, y=96)
        self.input_peixe.insert(tk.END, '10')
        ###########################################################################

        # Algas
        ########################################################################
        self.title_algas = tk.Label(self.canvas, text='Algas:', width=12)
        self.title_algas.place(x=430, y=120)

        self.input_algas = tk.Entry(self.canvas, width=10)
        self.input_algas.place(x=550, y=120)
        self.input_algas.insert(tk.END, '10')
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
        self.btn_ini.place(x=430, y=143)
        ############################################################

        # Coins
        ##################################################################
        #self.img = tk.PhotoImage(file="focaT.png").subsample(30)
        #self.image = self.canvas.create_image(0, 0, anchor=tk.NW, image=self.img)
        #self.image = self.canvas.create_image(360, 360, anchor=tk.NW, image=self.img)
        #self.image = self.canvas.create_image(360, 0, anchor=tk.NW, image=self.img)
        #self.image = self.canvas.create_image(80, 120, anchor=tk.NW, image=self.img)
        #self.image = self.canvas.create_image(120, 160, anchor=tk.NW, image=self.img)
        #self.image = self.canvas.create_image(0, 280, anchor=tk.NW, image=self.img)
        #########################

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




        # create origin
        origin = np.array([20, 380])


        # create red rect
        # self.rect = self.canvas.create_rectangle(
        #     origin[0] - 10, origin[1] - 10,
        #     origin[0] + 10, origin[1] + 10,
        #     fill='red')
        #self.mario = tk.PhotoImage(file="alga.png").subsample(2)
        #self.rect = self.canvas.create_image(0,0,anchor=tk.NW, image=self.mario)
        #self.canvas.move(self.rect, 10, 365)

        # pack all
        self.canvas.pack()

    def create_image(self, x, y, tipo):
        if tipo == 1:
            self.image = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_alga)
        elif tipo == 2:
            self.image = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_peixe)
        elif tipo == 3:
            self.image = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_foca)
        elif tipo == 4:
            self.image = self.canvas.create_image(x, y, anchor=tk.NW, image=self.img_tubarao)



    def delete(self):
        self.tex.delete('1.0', tk.END)
    def escreve(self, conteudo):
        self.tex.insert(tk.END, conteudo)

    def reset(self):
        self.update()
        time.sleep(0.30)
        #self.canvas.delete(self.rect)
        # origin = np.array([20, 380])
        # self.rect = self.canvas.create_rectangle(
        #     origin[0] - 10, origin[1] - 10,
        #     origin[0] + 10, origin[1] + 10,
        #     fill='red')
        # return observation
        #self.rect = self.canvas.create_image(0,0,anchor=tk.NW, image=self.mario)
        #self.canvas.move(self.rect, 10, 365)
        return

    def step(self, action):
        s = self.canvas.coords(self.rect)
        base_action = np.array([0, 0])
        if action == 2:   # up
            if s[1] > UNIT:
                base_action[1] -= UNIT
        elif action == 1:   # down
            if s[1] < (MAZE_H - 1) * UNIT:
                base_action[1] += UNIT
        elif action == 3:   # right
            if s[0] < (MAZE_W - 1) * UNIT:
                base_action[0] += UNIT
        elif action == 4:   # left
            if s[0] > UNIT:
                base_action[0] -= UNIT

        self.canvas.move(self.rect, base_action[0], base_action[1])  # move agent


    def render(self):
        time.sleep(0.1)
        self.update()


def update():
    for t in range(10):
        s = env.reset()
        while True:
            env.render()
            a = 1
            env.step(a)


if __name__ == '__main__':
    env = Maze()
    env.after(100, update)
    env.mainloop()
