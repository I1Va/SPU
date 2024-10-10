stack = []

n = int(input())

for i in range(n):
    cur_com = input().split()

    if (cur_com[0] == "push"):
        stack.append(int(cur_com[1]))
    elif (cur_com[0] == "pop"):
        if (len(stack) == 0):
            print("pop_error")
        else:
            stack.pop()
    elif (cur_com[0] == "last"):
        if (len(stack) == 0):
            print("last_error")
        else:
            print(stack[-1])

