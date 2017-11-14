import threading
import random
import time


class Philosopher(threading.Thread):
    running = True

    def __init__(self, _name, fork_on_left, fork_on_right):
        super(Philosopher, self).__init__()
        self.name = _name
        self.fork_on_left = fork_on_left
        self.fork_on_right = fork_on_right

    def run(self):
        while (self.running):
            time.sleep(random.uniform(3, 13))
            print('%s is hungry.' % self.name)
            self.dine()

    def dine(self):
        fork1, fork2 = self.fork_on_left, self.fork_on_right

        while self.running:
            fork1.acquire(True)
            locked = fork2.acquire(False)

            if locked:
                break

            fork1.release()
            print('%s swaps forks' % self.name)
            fork1, fork2 = fork2, fork1
        else:
            return

        self.dining()
        fork1.release()
        fork2.release()

    def dining(self):
        print('%s starts eating ' % self.name)
        time.sleep(random.uniform(1, 10))
        print('%s finishes eating and leaves to think.' % self.name)


def DiningPhilosophers():

    forks = [threading.Lock() for _ in range(5)]

    names = ['Mike', 'Bill', 'Kate', 'John', 'Patrick']

    philosophers = []

    for i in range(5):
        philosophers.append(Philosopher(names[i], fork_on_left=forks[i % 5], fork_on_right=forks[(i + 1) % 5]))

    random.seed(123)
    Philosopher.running = True

    for p in philosophers:
        p.start()

    time.sleep(50)

    Philosopher.running = False
    print("Now we're finishing.")

if __name__ == '__main__':
    DiningPhilosophers()
