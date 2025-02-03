// code to test if boost and vulkan are installed and work

#include <iostream>
#include <chrono>
#include <boost/compute.hpp>
#include <H5Cpp.h>

// Simulation parameters
constexpr float STEP_SIZE = 0.001f;
constexpr float TOTAL_TIME = 100.0f;
constexpr unsigned int PIXELS = 1024;
constexpr float Q1MIN = -M_PI;
constexpr float Q1MAX = M_PI;
constexpr float Q2MIN = -M_PI;
constexpr float Q2MAX = M_PI;

namespace compute = boost::compute;

using namespace std;
using compute::float4_;

struct Pendulum
{
	float q1, q2, p1, p2;
};

// Add struct adaptation for Boost.Compute
BOOST_COMPUTE_ADAPT_STRUCT(Pendulum, Pendulum, (q1, q2, p1, p2))


// Function to load OpenCL kernel from file
std::string load_kernel_source(const std::string& file_name) {
	std::ifstream file(file_name);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open kernel file: " + file_name);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}


// Equations of motion for double pendulum
void derivatives(const Pendulum* p, Pendulum* dpdt) {
	const float g = 9.806f;  // gravity

	// Intermediate calculations
	float cos_1 = cos(p->q1 - p->q2);
	float cos_2 = cos_1 * cos_1;
	float sin_1 = sin(p->q1 - p->q2);
	float denom = (2.f - cos_2);
	float denom_2 = denom * denom;
	float prod = p->p1 * p->p2;
	float numer = sin_1 * ( (p->p1 * p->p2 + 2.f * p->p2 * p->p2)*cos_1 - prod * (2 + cos_2)) ;
	float f2 = numer / denom_2;

	// Angular velocity derivatives
	dpdt->q1 = (-       p->p1 + p->p2 * cos_1) / denom;
	dpdt->q2 = (- 2.f * p->p2 + p->p1 * cos_1) / denom;

	// Momentum derivatives
	dpdt->p1 = - f2 + 2 * g * sin(p->q1);
	dpdt->p2 = + f2 +     g * sin(p->q2);
}

// Function to save flip times to HDF5
void save_flip_times_to_hdf5(const std::vector<float>& flip_times, const std::string& filename) {
	try {
		// Create a new HDF5 file
		H5::H5File file(filename, H5F_ACC_TRUNC);

		// Create a dataspace for the flip times
		hsize_t dims[1] = {flip_times.size()};
		H5::DataSpace dataspace(1, dims);

		// Create a dataset in the file
		H5::DataSet dataset = file.createDataSet("flip_times", H5::PredType::NATIVE_FLOAT, dataspace);

		// Write the flip times to the dataset
		dataset.write(flip_times.data(), H5::PredType::NATIVE_FLOAT);

		std::cout << "\nFlip times saved to " << filename << std::endl;
	} catch (H5::Exception& e) {
		std::cerr << "HDF5 Error: " << e.getDetailMsg() << std::endl;
	}
}

int main() {
	// Get the total number of pendulums
	constexpr unsigned int N = PIXELS * PIXELS /2;

	// Initialize Boost.Compute context, queue, and device// get the default device
	const compute::device device = compute::system::default_device();
	const compute::context context(device);
	compute::command_queue queue(context, device, compute::command_queue::enable_profiling);

	// timer
	auto start_time = chrono::high_resolution_clock::now();

	// Create an PIXELS/2 x PIXELS grid of double pendulums
	std::vector<Pendulum> pendulums(N);
	// Initialize pendulums
	for (int i = 0; i < N; ++i) {
		pendulums[i].q1 = Q1MIN + int(i / PIXELS) * (Q1MAX - Q1MIN) / PIXELS;
		pendulums[i].q2 = Q2MIN + int(i % PIXELS) * (Q2MAX - Q2MIN) / PIXELS;
		pendulums[i].p1 = 0.;
		pendulums[i].p2 = 0.;
	}

	// Allocate device buffers
	compute::buffer pendulum_buffer(context, N * sizeof(Pendulum), CL_MEM_READ_ONLY);
	compute::buffer flip_time_buffer(context, N * sizeof(float), CL_MEM_WRITE_ONLY);

	// Copy data to device
	compute::copy(
		pendulums.data(), // Source: host data (Pendulum*)
		pendulums.data() + N, // End of host data
		compute::make_buffer_iterator<Pendulum>(pendulum_buffer, 0), // Destination: buffer iterator
		queue
	);

	// Load and build kernel
	std::string kernel_source = load_kernel_source("../flip_time.cl");

	// Compile the kernel program
	compute::program program = compute::program::create_with_source(kernel_source, context);
	try {
		program.build();
	} catch (const compute::opencl_error& e) {
		std::cerr << "OpenCL build error: " << e.what() << std::endl;
		std::cerr << "Build log: " << program.build_log() << std::endl;
		return 1;
	}

	// Set up kernel
	compute::kernel kernel(program, "flip_time_simulation");
	kernel.set_arg(0, pendulum_buffer);
	kernel.set_arg(1, flip_time_buffer);
	kernel.set_arg(2, sizeof(float), &STEP_SIZE);
	kernel.set_arg(3, sizeof(float), &TOTAL_TIME);
	kernel.set_arg(4, sizeof(int), &N);

	// Execute kernel
	size_t global_work_size = N;
	queue.enqueue_nd_range_kernel(kernel, 1, nullptr, &global_work_size, nullptr);

	// Copy the flip times back to the host
	std::vector<float> flip_times(N);
	compute::copy(
		compute::make_buffer_iterator<float>(flip_time_buffer, 0),
		compute::make_buffer_iterator<float>(flip_time_buffer, N),
		flip_times.begin(),
		queue
	);

	// Save flip times to HDF5 file
	save_flip_times_to_hdf5(flip_times, "../data/flip_times.h5");

	// Final timing
	auto end_time = chrono::high_resolution_clock::now();
	double total_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;

	std::cout << "Total computation time: " << std::fixed << std::setprecision(1)
			  << total_time << " seconds"<< std::endl;

	return 0;
}