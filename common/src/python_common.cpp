#include <nanobind/nanobind.h>

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

namespace nb = nanobind;

NB_MODULE(pcl_common_ext, m)
{
  nb::class_<pcl::PointXYZ>(m, "PointXYZ")
      .def(nb::init())
      .def(nb::init<float, float, float>())
      .def_rw("x", &pcl::PointXYZ::x)
      .def_rw("y", &pcl::PointXYZ::y)
      .def_rw("z", &pcl::PointXYZ::z)
      .def("__repr__", [](const pcl::PointXYZ& p) {
        return nb::str("PointXYZ(x={}, y={}, z={})").format(p.x, p.y, p.z);
      });

  nb::class_<pcl::PointCloud<pcl::PointXYZ>>(m, "PointcloudXYZ")
      .def(nb::init<>())
      .def("append", &pcl::PointCloud<pcl::PointXYZ>::push_back)
      .def("at", nb::overload_cast<std::size_t>(&pcl::PointCloud<pcl::PointXYZ>::at), "Get a point from the cloud");
}
