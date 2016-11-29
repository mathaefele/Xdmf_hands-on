#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <hdf5.h>
 
#define NumElements 3
#define NumNodes 6
 
void
write_hdf5_data()
{
	hid_t     file_id;
	hid_t     dataset_id, dataspace_id;
	hsize_t   dims[2];
	herr_t    status;   
	int 	  indx;
 
	/* Create the HDF5 file */
 
	file_id = H5Fcreate("2DUnstructuredMesh.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
 
	/* Connectivity data */
 
	int connectivity[NumElements][4];
 
	connectivity[0][0] = 0;connectivity[0][1] = 1;connectivity[0][2] = 2;connectivity[0][3] = 3;    
	connectivity[1][0] = 1;connectivity[1][1] = 4;connectivity[1][2] = 5;connectivity[1][3] = 2;
	connectivity[2][0] = 2;connectivity[2][1] = 5;connectivity[2][2] = 3;connectivity[2][3] = 3;
 
 
	/* Write Connectivity data to file */
 
 
	dims[0] = NumElements;
	dims[1] = 4;
	dataspace_id = H5Screate_simple(2, dims, NULL);
	dataset_id = H5Dcreate(file_id, "/Quadrilaterals", H5T_NATIVE_INT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, connectivity);
 	status = H5Dclose(dataset_id); 
        status = H5Sclose(dataspace_id);     
 
	/* Node coordinates data */
 
	float xyz[NumNodes][2];          
 
	xyz[0][0] = 0.0;xyz[0][1] = 0.0;
	xyz[1][0] = 1.0;xyz[1][1] = 0.0;	
	xyz[2][0] = 1.0;xyz[2][1] = 1.0;
	xyz[3][0] = 0.0;xyz[3][1] = 1.0;	
	xyz[4][0] = 2.5;xyz[4][1] = 0.0;
	xyz[5][0] = 2.0;xyz[5][1] = 2.0;	
 
	/* Write Node coordinates data to file */
 
	dims[0] = NumNodes;
	dims[1] = 2;	
        dataspace_id = H5Screate_simple(2, dims, NULL);
        dataset_id = H5Dcreate(file_id, "/XY", H5T_NATIVE_FLOAT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, xyz); 
	status = H5Dclose(dataset_id); 
        status = H5Sclose(dataspace_id);     
 
        /* Scalar data */
 
        float scalardata[NumNodes]; 
 
	for(indx = 0; indx < 6; indx++)
	{	
		scalardata[indx] = indx*100;
	}
 
	/* Write Scalar data to file */
 
	dims[0] = NumNodes;	
	dataspace_id = H5Screate_simple(1, dims, NULL);
	dataset_id = H5Dcreate(file_id, "/Scalar", H5T_NATIVE_FLOAT,dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status = H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, scalardata); 
	status = H5Dclose(dataset_id); 
	status = H5Sclose(dataspace_id);
 
	/* Close HDF entities */
 
        status = H5Fclose(file_id);
 
}
 
void
write_xdmf_xml()
{
    FILE *xmf = 0;
 
    /*
     * Open the file and write the XML description of the mesh.
     */
 
    xmf = fopen("2DUnstructuredMesh.xmf", "w");
    fprintf(xmf, "<?xml version=\"1.0\" ?>\n");
    fprintf(xmf, "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []>\n");
    fprintf(xmf, "<Xdmf Version=\"2.0\">\n");
    fprintf(xmf, "<Domain>\n");
    fprintf(xmf, "<Grid Name=\"2D Unstructured Mesh\">\n");
    fprintf(xmf, "<Topology TopologyType=\"Quadrilateral\" NumberOfElements=\"%d\">\n", NumElements);
    fprintf(xmf, "<DataItem Dimensions=\"%d\" Format=\"HDF\">\n", NumElements*4);
    fprintf(xmf, "2DUnstructuredMesh.h5:/Quadrilaterals\n");	
    fprintf(xmf, "</DataItem>\n");
    fprintf(xmf, "</Topology>\n");
    fprintf(xmf, "<Geometry GeometryType=\"XY\">\n");
    fprintf(xmf, "<DataItem Dimensions=\"%d\" Format=\"HDF\">\n", NumNodes*2);
    fprintf(xmf, "2DUnstructuredMesh.h5:/XY\n");
    fprintf(xmf, "</DataItem>\n");
    fprintf(xmf, "</Geometry>\n");
    fprintf(xmf, "<Attribute Name=\"Scalar\" AttributeType=\"Scalar\" Center=\"Node\">\n");
    fprintf(xmf, "<DataItem Dimensions=\"%d\" NumberType=\"Float\" Precision=\"4\" Format=\"HDF\">\n", NumNodes);
    fprintf(xmf, "2DUnstructuredMesh.h5:/Scalar\n");
    fprintf(xmf, "</DataItem>\n");
    fprintf(xmf, "</Attribute>\n");
    fprintf(xmf, "</Grid>\n");
    fprintf(xmf, "</Domain>\n");
    fprintf(xmf, "</Xdmf>\n");
    fclose(xmf);
}
 
int
main(int argc, char *argv[])
{
    write_hdf5_data();
    write_xdmf_xml();
 
    return 0;
}
