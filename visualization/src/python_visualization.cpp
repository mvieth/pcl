#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

NB_MODULE(pcl_visualization_ext, m)
{
  nb::class_<pcl::visualization::PCLVisualizer>(m, "PCLVisualizer")
      .def(nb::init<>())
      /*.def("AddPointCloudXYZ",
             nb::overload_cast<const pcl::PointCloud<pcl::PointXYZ>::ConstPtr&,
                               const std::string&,
                               int>(&pcl::visualization::PCLVisualizer::addPointCloud),
             "Add point cloud to visualizer.")*/
      .def("AddPointCloudXYZ",
           nb::overload_cast<const pcl::PointCloud<pcl::PointXYZ>::ConstPtr&,
                             const std::string&,
                             int>(
           &pcl::visualization::PCLVisualizer::addPointCloud<pcl::PointXYZ>),
           "Add point cloud.")
      .def("removeAllShapes", &pcl::visualization::PCLVisualizer::removeAllShapes, "Clear the visualizer.")
      .def("spinOnce", &pcl::visualization::PCLVisualizer::spinOnce,"runs the visualizer loop once")
      .def("addCoordinateSystem",
           nb::overload_cast<double, const std::string&, int>(&pcl::visualization::PCLVisualizer::addCoordinateSystem),
           "Adds a coordinate system");
}
