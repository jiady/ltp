import os
from time import gmtime, strftime
from sys import argv

exe ="./svm-light-TK-1.2.1/svm_classify"

if(len(argv)>1):
	test_file=argv[1]
else:
	test_file="./data/test.txt"

model_file="./svm_model/2.model"
output_file="./output/"+test_file.split("/")[-1]+"_" + strftime("%H_%M_%S_%d_%b", gmtime())+".result"
raw_test="./data/test_raw.txt"


fp= open(  output_file + "-fp",'w');
fn= open(  output_file + "-fn",'w');
dd= open(  output_file + "-dd",'w');

cmd = exe +" "+ test_file+" " + model_file +" "+output_file
print cmd

os.system(cmd)

labels=[]
sen = [line for line in open(test_file, 'r+') ]
raw = [line for line in open(raw_test,'r+')   ]

c =0;
for i in open(output_file, 'r+'):
	if (i[0]=='-' and sen[c][0]!='-'):
		dd.write(sen[c])
		fn.write(i)
		fn.write(sen[c])
		if(len(argv)==1):
			fn.write(raw[c])
	if (i[0]!='-' and sen[c][0]=='-'):
		dd.write(sen[c])
		fp.write(i)
		fp.write(sen[c])
		if(len(argv)==1):
			fp.write(raw[c])
	c=c+1

