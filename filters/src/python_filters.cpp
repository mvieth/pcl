#include <nanobind/nanobind.h>

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/filters/passthrough.h>

namespace nb = nanobind;

NB_MODULE(pcl_filters_ext, m)
{
#if 0 // abstract class
  nb::class_<pcl::Filter<pcl::PointXYZ>(m, "FilterXYZ")
      .def(nb::init<>())
      .def("filter", &pcl::Filter<pcl::PointXYZ>::filter)
      ;
#endif
#if 0 // abstract class
  nb::class_<pcl::FilterIndices<pcl::PointXYZ>(m, "FilterIndicesXYZ")
      .def(nb::init<>())
      .def("setNegative", &pcl::FilterIndices<pcl::PointXYZ>::setNegative)
      ;
#endif
  nb::class_<pcl::PassThrough<pcl::PointXYZ>
  //, pcl::FilterIndices<pcl::PointXYZ> // parent class
  >(m, "PassThroughXYZ")
      .def(nb::init<>())
      .def("setFilterLimits", &pcl::PassThrough<pcl::PointXYZ>::setFilterLimits)
      .def("setFilterFieldName", &pcl::PassThrough<pcl::PointXYZ>::setFilterFieldName)
      .def("setNegative", &pcl::PassThrough<pcl::PointXYZ>::setNegative) // TODO this is actually a function from FilterIndices
      .def("filter", nb::overload_cast<pcl::PointCloud<pcl::PointXYZ>&>(&pcl::PassThrough<pcl::PointXYZ>::filter)) // TODO this is actually a function from Filter
      ;
}
