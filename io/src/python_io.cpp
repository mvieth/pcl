#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

NB_MODULE(pcl_io_ext, m)
{
  m.def("ReadCloudXYZ", [](const std::string& filePath) {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PCDReader p;
        p.read(filePath, cloud);
        return cloud;
      },
      "Read a PCD file containing PointXYZ data",
      nb::arg("filePath"));
}
