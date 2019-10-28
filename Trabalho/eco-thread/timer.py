import threading
global count
count = 0

class TimerThread(threading.Thread):

    def __init__(self, id, env):
        self.id = id
        self.env = env
        threading.Thread.__init__(self)

    def reset(self):
        global count
        count = 1
        self.env.t.set('00:00:00')

    def start(self):
        global count
        count = 0
        self.start_timer()

    def run(self):
        global count
        self.timer()

    def stop(self):
        global count
        count = 1

    def start_timer(self):
        global count
        if (count == 0):
            self.d = str(self.env.t.get())
            h, m, s = map(int, self.d.split(":"))

            h = int(h)
            m = int(m)
            s = int(s)
            if (s < 59):
                s += 1
            elif (s == 59):
                s = 0
                if (m < 59):
                    m += 1
                elif (m == 59):
                    h += 1
            if (h < 10):
                h = str(0) + str(h)
            else:
                h = str(h)
            if (m < 10):
                m = str(0) + str(m)
            else:
                m = str(m)
            if (s < 10):
                s = str(0) + str(s)
            else:
                s = str(s)
            self.d = h + ":" + m + ":" + s

            self.env.t.set(self.d)
            if (count == 0):
                self.env.canvas.after(930, self.start_timer)