import numpy as np
import time

def get_grid(su, x, y):
  a=np.zeros(9, dtype=np.int)
  for i in range(3):
    for j in range(3):
      a[i*3+j]=su[x-x%3+i][y-y%3+j]
  return a

def get_nonzero(su, i, j):
  r_count = np.count_nonzero(su[i, :])
  c_count = np.count_nonzero(su[:, j])
  g_count = np.count_nonzero(get_grid(su, i, j))
  return c_count+r_count+g_count

def get_weak_grid(non_zeros, su):
  max_nonZero=0
  j_=0
  k_=0
  for k in range(9):
    for j in range(9):
      if su[k][j]==0:
        non_zeros[k*9+j]=get_nonzero(su, k, j)
        if non_zeros[k*9+j]>max_nonZero:
          max_nonZero=non_zeros[k*9+j]
          j_=j
          k_=k
  if max_nonZero!=0:
    return (k_,j_)
  else:
    print(su)
    print("solved!")
    return (-1,-1)

def solve(s):
  (x, y)=get_weak_grid(non_zeros, s)
  if x==-1:
    return True
  current=np.concatenate((s[x,:], s[:,y], get_grid(s, x, y)), axis=0)

  current=np.unique(current)

  candidtate=np.setdiff1d(range(10), current)
  #print("candidate: ", candidtate)

  if get_weak_grid(non_zeros, s)==(-1,-1):
    return True
  else:
    for c in candidtate:
      s[x, y]=c
      if(solve(s)):
        return True
      else:
        s[x, y]=0


if __name__ == '__main__':
  non_zeros = np.zeros(81, dtype=np.int)
  data = "sudoku.txt"

  matrix = [line.strip() for line in open('sudoku.txt')]
  sudoku = np.zeros((9,9), dtype=np.int)

  for j in range(len(matrix)):
    col = 0
    for i in matrix[j]:
      if i!=' ':
        sudoku[col, j]=i
        col=col+1

  begin=time.time()
  solve(sudoku)
  end=time.time()
  print("time elapsed: ", end-begin, "s")
