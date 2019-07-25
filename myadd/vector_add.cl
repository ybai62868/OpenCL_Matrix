// Yang.Bai
// yb269@cornell.edu

# define BUFFER_SIZE 256
kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void vector_add(global int * c, 
                global const int * a, 
                global const int * b,
                const int n_elements) {
                    int arrayA[BUFFER_SIZE];
                    int arrayB[BUFFER_SIZE];
                    for ( int i = 0;i < n_elements; i += BUFFER_SIZE ) {
                        int size = BUFFER_SIZE;
                        if ( i + size > n_elements ) {
                            size = n_elements - i;
                        }
                        readA: for ( int j = 0; j < size; j++ ) {
                            arrayA[j] = a[i+j];
                        }
                        readB: for ( int j = 0; j < size; j++ ) {
                            arrayB[j] = b[i+j];
                        }
                        vadd_writeC: for ( int j = 0; j < size; j++ ) {
                            c[i+j] = arrayA[j] + arrayB[j];
                        }
                    }
                }
                