# !bin/bash

for i in `seq 1 6`
do
	python learn.py ./data/feature."${i}".txt
	python classify.py ./data/test."${i}".txt
done
