# !bin/bash

cd examples
make clean
make
cd ..
./examples/predictor

cd svm
exe=./svm-light-TK-1.2.1/my_svm_classify 
model=./svm_model/2.model
input=./tmp_data/feature.txt
output=./tmp_data/output.txt

${exe} ${input} ${model} ${output}

cd ../
./examples/compare
