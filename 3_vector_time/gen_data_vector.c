#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#include <hdf5.h>
 
// The number of cells in the X, Y dimensions
#define NX 30
#define NY 20
#define NT 30
#define M_PI 3.1415926535897932
void
write_hdf5_data()
{
    hid_t     file_id;
    file_id = H5Fcreate("xdmfvector.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    float valx, valy;
    int t=0;
    //Number of rounds
    int nrounds = 1;
    char dsetname[20];
    float dalpha = ((float) nrounds) / ((float) NT) * 2.0*M_PI;
 
    // Create the coordinate data.
    float *x = (float *) malloc(NX * sizeof(float));
    float *y = (float *) malloc(NY * sizeof(float));
    float dx = 2.0 / NX;
    float dy = 1.0 / NY;
    for (int i = 0; i < NX; i++)
      x[i] = -1. + i*dx;
    for (int i = 0; i < NY; i++)
      y[i] = i*dy;
 
    // Write the data file.
    hid_t     dataset_id, dataspace_id;
    hsize_t   dims3d[3], dims1d[1];
    herr_t    status;
    const char *coordNames[] = {"/X", "/Y"};
 
    /* Write separate coordinate arrays for the x and y coordinates. */
    for(int did = 0; did < 2; ++did)
    {
        dims1d[0] = did == 0 ? NX : NY;
        dataspace_id = H5Screate_simple(1, dims1d, NULL);
 
        dataset_id = H5Dcreate(file_id, coordNames[did], H5T_NATIVE_FLOAT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
 
        status = H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, did == 0 ? x : y);
 
        status = H5Dclose(dataset_id);
 
        status = H5Sclose(dataspace_id);
    }
 
    // Create the vector data.
    float *vector = (float *) malloc(2 * NX * NY * sizeof(float));
    // Write the vector data.
    dims3d[0] = NY;
    dims3d[1] = NX;
    dims3d[2] = 2;
    dataspace_id = H5Screate_simple(3, dims3d, NULL);
 
    for (t = 0; t < NT; t++)
    {
      valx = cos(t*dalpha);
      valy = sin(t*dalpha);
      for (int j = 0; j < NY; j++)
      {
        for (int i = 0; i < NX; i++)
        {
          vector[2*(i+NX*j)] = valx;
          vector[1+2*(i+NX*j)] = valy;
        }
      }
      sprintf(dsetname,"/vector-%d",t);

      dataset_id = H5Dcreate(file_id, dsetname, H5T_NATIVE_FLOAT,dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      status = H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vector);
      status = H5Dclose(dataset_id);
    }
 
    // Free the data.
    status = H5Sclose(dataspace_id);
    free(x);
    free(y);
    free(vector);
 
    status = H5Fclose(file_id);
}
 
void
write_xdmf_xml()
{
    FILE *xmf = 0;
    xmf = fopen("xdmfvector.xmf", "w");
    fclose(xmf);
}
 
int
main(int argc, char *argv[])
{
    write_hdf5_data();
    write_xdmf_xml();
 
    return 0;
}

