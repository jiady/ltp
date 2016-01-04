import os
from sys import argv

exe ="./svm-light-TK-1.2.1/svm_learn"
if(len(argv)>1):
	train_file=argv[1]
else:
	train_file="./data/train.txt"
model_file="./svm_model/1.model"
option="-t 5 -C + "

cmd = exe +" "+option +" "+ train_file+" " + model_file
print cmd

os.system(cmd)

