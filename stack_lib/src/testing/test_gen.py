from random import randrange

commands = ["pop", "push", "last"]


n = randrange(1, 20)

print(n)

for cur_tests in range(n):
    cur_com = commands[randrange(3)]
    val = randrange(-10, 10)

    cur_test = cur_com
    if cur_com == "push":
        cur_test += " " + str(val)

    print(cur_test)


