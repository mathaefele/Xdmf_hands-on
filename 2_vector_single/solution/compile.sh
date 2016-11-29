HDF5_INC=-I/usr/include
HDF5_LIB="-L/usr/lib -lhdf5 -lm"
gcc $HDF5_INC  -std=c99 gen_data_vector.c -o gen_data_vector $HDF5_LIB
