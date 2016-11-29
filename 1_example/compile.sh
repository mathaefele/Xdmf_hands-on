HDF5_INC=-I/usr/include
HDF5_LIB="-L/usr/lib -lhdf5 -lm"
gcc $HDF5_INC  -std=c99 gen_data_grid.c -o gen_data_grid $HDF5_LIB
gcc $HDF5_INC  -std=c99 gen_data_unstructured.c -o gen_data_unstructured $HDF5_LIB
