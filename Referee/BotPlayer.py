import time
import os
import sys
from subprocess import PIPE, Popen
from threading import Thread
from queue import Queue, Empty

currentTimeMs = lambda: int(round(time.time() * 1000))

ON_POSIX = 'posix' in sys.builtin_module_names

class BotPlayer:
    def __init__(self,playerBot : str,playerId : str,timelimit : float):
        self.human = False
        self.id = playerId
        self.playerProcess = Popen(playerBot,shell=False, stdout=PIPE, bufsize=1, stdin=PIPE, close_fds=ON_POSIX)
        self.queue = Queue()
        self.thread = Thread(target=self.enqueueOutput, args=(self.playerProcess.stdout, self.queue))
        self.thread.daemon = True
        self.thread.start()
        self.timelimit = timelimit # time limit in ms

    def send(self, msg):
        self.playerProcess.stdin.write(bytes(str(msg) + "\n", 'UTF-8'))
        self.playerProcess.stdin.flush()

    def receive(self, cast):
        msg = ""
        start = currentTimeMs()

        while msg=="":
            try:  
                data = self.queue.get_nowait()
            except Empty:
                if currentTimeMs() - start > self.timelimit:
                    print(f"Timeout for player {self.id}")
                    return cast("")
            else:
                msg = data.decode("utf-8").strip()

        return cast(msg)

    def finish(self):
        self.playerProcess.kill()

    def enqueueOutput(self,out, queue):
        while True:
            line = out.readline()
            queue.put(line)
        out.close()
