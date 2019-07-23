/*
    Yang.Bai
    yb269.cornell.edu
*/

# include "vadd.h"

static const int DATA_SIZE = 128;

static const std::string error_message = 
    "Error: Result mismath:\n"
    "i = %d CPU result = %d Device result = %d\n";

int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <xclbin>" << std::endl;
        return EXIT_FAILURE;
    }
    char * xclbinFilename = argv[1];

    size_t size_t_bytes = DATA_SIZE * sizeof(int);
    vector<int, aligned_allocator<int> > source_a(DATA_SIZE, 4);
    vector<int, aligned_allocator<int> > source_b(DATA_SIZE, 12);
    vector<int, aligned_allocator<int> > source_result(DATA_SIZE);

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform = platforms[0];
    std::cout << "Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
    cl::Device device = devices[0];
    std::cout << "Device: " << device.getInfo<CL_DEVICE_NAME>() << "\n";

    cl::Context context(device);
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE);

    std::cout << "Loading: " << xclbinFilename << "'\n";
    std::ifstream bin_file(xclbinFilename, std::ifstream::binary);
    bin_file.seekg (0, bin_file.end);
    unsigned nb = bin_file.tellg();
    bin_file.seekg (0, bin_file.beg);
    char * buf = new char [nb];
    bin_file.read(buf, nb);

    cl::Buffer buffer_a(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            size_in_bytes, source_a.data());
    cl::Buffer buffer_b(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
            size_in_bytes, source_b.data());
    cl::Buffer buffer_result(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,
            size_in_bytes, source_results.data());

    cl::Program::Binaries bins;
    bins.push_back({buf, nb});
    devices.resize(1);
    cl::Program program(context, devices, bins);

    q.enqueueMigrateMemObjects(inBufVec, 0/* 0 means from host */);
    cl::Kernel krnl_vector_add(program, "vector_add");

    int narg = 0;
    krnl_kernel_add.setArg(narg++, buffer_result);
    krnl_kernel_add.setArg(narg++, buffer_a);
    krnl_kernel_add.setArg(narg++, buffer_b);
    krnl_kernel_add.setArg(narg++, DATA_SIZE);

    q.enqueueTask(krnl_vector_add);
    q.enqueueMigrateMemObjects(outBufVec, CL_MIGRATE_MEM_OBJECT_HOST);
    q.finish();

    int match = 0;
    print ("Result = \n");
    for (int i = 0; i < DATA_SIZE; i++) {
        int host_result = source_a[i] + source_b[i];
        if (source_result[i] != host_result) {
            printf(error_message.c_str(), i, host_result, source_results[i]);
            match = 1;
            break;
        } else {
            printf("%d", source_results[i]);
            if (((i + 1) % 16) == 0) {
                printf("\n");
            }
        }
    }

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
    return (match ? EXIT_FAILURE : EXIT_SUCCESS);

}