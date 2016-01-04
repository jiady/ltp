# !bin/bash

cd examples
make clean
make
cd ../
./examples/genfeature

cd ./svm/data
./cut.sh
cd ../
python learn.py
python classify.py
