import random


random.seed()


NUMBER = int(input('Number of rows and collumns: '))


mat = [[random.randint(-10000, 10000) for i in range(NUMBER)] for i in range(NUMBER)]

with open('matrix1.txt', 'w') as f:
  for i, row in enumerate(mat):
    for j, num in enumerate(row):
      f.write(str(num))
      if (j+1) != NUMBER:
        f.write(',')
    if (i+1) != NUMBER:
      f.write('\n')

mat = [[random.randint(-10000, 10000) for i in range(NUMBER)] for i in range(NUMBER)]

with open('matrix2.txt', 'w') as f:
  for i, row in enumerate(mat):
    for j, num in enumerate(row):
      f.write(str(num))
      if (j+1) != NUMBER:
        f.write(',')
    if (i+1) != NUMBER:
      f.write('\n')
