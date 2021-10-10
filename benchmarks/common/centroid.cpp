#include <benchmark/benchmark.h>
#include <pcl/common/centroid.h> // for 
#include <pcl/io/pcd_io.h> // for loadPCDFile

static void BM_compute3DCentroid(benchmark::State& state, const std::string& file) {
  // Perform setup here
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if(0!=pcl::io::loadPCDFile(file, *cloud)) {
    PCL_ERROR ("PCD file failed to load");
  }
  Eigen::Vector4f centroid;
  for (auto _ : state) {
    // This code gets timed
    benchmark::DoNotOptimize (pcl::compute3DCentroid (*cloud, centroid));
  }
}

static void BM_computeMeanAndCovarianceMatrixFile(benchmark::State& state, const std::string& file) {
  // Perform setup here
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if(0!=pcl::io::loadPCDFile(file, *cloud)) {
    PCL_ERROR ("PCD file failed to load");
  }
  Eigen::Vector4f centroid;
  Eigen::Matrix3f cov;
  for (auto _ : state) {
    // This code gets timed
    benchmark::DoNotOptimize (pcl::computeMeanAndCovarianceMatrix (*cloud, cov, centroid));
  }
}

static void BM_computeMeanAndCovarianceMatrixFile_Indices(benchmark::State& state, const std::string& file) {
  // Perform setup here
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if(0!=pcl::io::loadPCDFile(file, *cloud)) {
    PCL_ERROR ("PCD file failed to load");
  }
  pcl::Indices indices;
  indices.reserve(cloud->size());
  for(pcl::uindex_t i=0; i<(cloud->size()); ++i) indices.push_back(i);
  Eigen::Vector4f centroid;
  Eigen::Matrix3f cov;
  for (auto _ : state) {
    // This code gets timed
    benchmark::DoNotOptimize (pcl::computeMeanAndCovarianceMatrix (*cloud, indices, cov, centroid));
  }
}

static void BM_computeMeanAndCovarianceMatrixDense(benchmark::State& state) {
  // Perform setup here
  pcl::PointCloud<pcl::PointXYZ> cloud;
  for(std::int64_t i=0; i<state.range(0); ++i) {
    cloud.emplace_back(3*i+1, 3*i, 3*i+2);
  }
  cloud.is_dense = true;
  Eigen::Vector4f centroid;
  Eigen::Matrix3f cov;
  for (auto _ : state) {
    // This code gets timed
    benchmark::DoNotOptimize (pcl::computeMeanAndCovarianceMatrix (cloud, cov, centroid));
  }
}

static void BM_computeMeanAndCovarianceMatrixNonDense(benchmark::State& state) {
  // Perform setup here
  pcl::PointCloud<pcl::PointXYZ> cloud;
  for(std::int64_t i=0; i<state.range(0); ++i) {
    cloud.emplace_back(3*i+1, 3*i, 3*i+2);
  }
  cloud.is_dense = false;
  Eigen::Vector4f centroid;
  Eigen::Matrix3f cov;
  for (auto _ : state) {
    // This code gets timed
    benchmark::DoNotOptimize (pcl::computeMeanAndCovarianceMatrix (cloud, cov, centroid));
  }
}

static void BM_isFinite(benchmark::State& state, const std::string& file) {
  // Perform setup here
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if(0!=pcl::io::loadPCDFile(file, *cloud)) {
    PCL_ERROR ("PCD file failed to load");
  }
  std::size_t count = 0;
  for (auto _ : state) {
    // This code gets timed
    for (const auto& point : (*cloud))
      if (pcl::isFinite(point))
        ++count;
  }
  std::cout << "count=" << count << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 3)
  {
    std::cerr << "No test files given. Please download `table_scene_mug_stereo_textured.pcd` and `milk_cartoon_all_small_clorox.pcd`, and pass their paths to the test." << std::endl;
    return (-1);
  }
  //benchmark::RegisterBenchmark("BM_compute3DCentroid_mug", BM_compute3DCentroid, argv[1])->Unit(benchmark::kMillisecond);
  //benchmark::RegisterBenchmark("BM_compute3DCentroid_milk", BM_compute3DCentroid, argv[2])->Unit(benchmark::kMillisecond);
  benchmark::RegisterBenchmark("BM_computeMeanAndCovarianceMatrixFile_mug", BM_computeMeanAndCovarianceMatrixFile, argv[1])->Unit(benchmark::kMillisecond);
  benchmark::RegisterBenchmark("BM_computeMeanAndCovarianceMatrixFile_milk", BM_computeMeanAndCovarianceMatrixFile, argv[2])->Unit(benchmark::kMillisecond);
  benchmark::RegisterBenchmark("BM_computeMeanAndCovarianceMatrixDense", BM_computeMeanAndCovarianceMatrixDense)->Arg(50)->Arg(100)->Arg(200)->Unit(benchmark::kMicrosecond);
  benchmark::RegisterBenchmark("BM_computeMeanAndCovarianceMatrixNonDense", BM_computeMeanAndCovarianceMatrixNonDense)->Arg(50)->Arg(100)->Arg(200)->Unit(benchmark::kMicrosecond);
  // TODO dense and non-dense, with and without indices, 50/100/200 points large
  //benchmark::RegisterBenchmark("BM_computeMeanAndCovarianceMatrix_Indices_mug", BM_computeMeanAndCovarianceMatrixFile_Indices, argv[1])->Unit(benchmark::kMillisecond);
  //benchmark::RegisterBenchmark("BM_computeMeanAndCovarianceMatrix_Indices_milk", BM_computeMeanAndCovarianceMatrixFile_Indices, argv[2])->Unit(benchmark::kMillisecond);
  //benchmark::RegisterBenchmark("BM_isFinite_mug", BM_isFinite, argv[1]);//->Unit(benchmark::kMillisecond);
  //benchmark::RegisterBenchmark("BM_isFinite_milk", BM_isFinite, argv[2]);//->Unit(benchmark::kMillisecond);
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
