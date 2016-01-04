//
// Created by jiadongyu on 1/4/16.
//


#include "predictor.h"

int main(int argv ,char * argc[]){
    omp_set_num_threads(omp_get_num_procs());
    Predictor predictor;
    predictor.main();
    return 0;

}

