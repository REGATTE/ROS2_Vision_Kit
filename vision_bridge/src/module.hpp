#ifndef VISION_BRIDGE_MODULE_HPP_
#define VISION_BRIDGE_MODULE_HPP_

#include <iostream>
#include <boost/python.hpp>
#include <vision_bridge/vision_bridge.h>
#include <Python.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarrayobject.h>

#include <opencv2/core/core.hpp>

namespace bp = boost::python;

int convert_to_VisionMat2(const PyObject * o, cv::Mat & m);

PyObject * pyopencv_from(const cv::Mat & m);

static void * do_numpy_import()
{
  import_array();
  return nullptr;
}

#endif  // VISION_BRIDGE_MODULE_HPP_
