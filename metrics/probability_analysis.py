#!/usr/bin/env python3

import sys
import pandas as pd

if __name__ == "__main__":
  x = pd.read_csv(sys.argv[1] , header=None)
  x.columns = ["lambda", "doctor", "virus"]
  print(x.describe())
