#include "module.hpp"
#include <string>

PyObject * mod_opencv;

bp::object
cvtColor2Wrap(bp::object obj_in, const std::string & encoding_in, const std::string & encoding_out)
{
  // Convert the Python input to an image
  cv::Mat mat_in;
  convert_to_VisionMat2(obj_in.ptr(), mat_in);

  // Call vision_bridge for color conversion
  vision_bridge::VisionImagePtr vision_image(new vision_bridge::VisionImage(
      std_msgs::msg::Header(), encoding_in, mat_in));

  cv::Mat mat = vision_bridge::cvtColor(vision_image, encoding_out)->image;

  return bp::object(boost::python::handle<>(pyopencv_from(mat)));
}

bp::object
cvtColorForDisplayWrap(
  bp::object obj_in,
  const std::string & encoding_in,
  const std::string & encoding_out,
  bool do_dynamic_scaling = false,
  double min_image_value = 0.0,
  double max_image_value = 0.0)
{
  // Convert the Python input to an image
  cv::Mat mat_in;
  convert_to_VisionMat2(obj_in.ptr(), mat_in);

  vision_bridge::VisionImagePtr vision_image(new vision_bridge::VisionImage(
      std_msgs::msg::Header(), encoding_in, mat_in));

  vision_bridge::CvtColorForDisplayOptions options;
  options.do_dynamic_scaling = do_dynamic_scaling;
  options.min_image_value = min_image_value;
  options.max_image_value = max_image_value;
  cv::Mat mat = vision_bridge::cvtColorForDisplay(/*source=*/ vision_image,
      /*encoding_out=*/ encoding_out,
      /*options=*/ options)->image;

  return bp::object(boost::python::handle<>(pyopencv_from(mat)));
}

BOOST_PYTHON_FUNCTION_OVERLOADS(cvtColorForDisplayWrap_overloads, cvtColorForDisplayWrap, 3, 6)

int VISION_MAT_CNWrap(int i)
{
  return VISION_MAT_CN(i);
}

int VISION_MAT_DEPTHWrap(int i)
{
  return VISION_MAT_DEPTH(i);
}

BOOST_PYTHON_MODULE(vision_bridge_boost)
{
  do_numpy_import();
  mod_opencv = PyImport_ImportModule("cv2");

  // Wrap the function to get encodings as OpenCV types
  boost::python::def("getVisionType", vision_bridge::getVisionType);
  boost::python::def("cvtColor2", cvtColor2Wrap);
  boost::python::def("VISION_MAT_CNWrap", VISION_MAT_CNWrap);
  boost::python::def("VISION_MAT_DEPTHWrap", VISION_MAT_DEPTHWrap);
  boost::python::def("cvtColorForDisplay", cvtColorForDisplayWrap,
    cvtColorForDisplayWrap_overloads(
      boost::python::args("source", "encoding_in", "encoding_out", "do_dynamic_scaling",
      "min_image_value", "max_image_value"),
      "Convert image to display with specified encodings.\n\n"
      "Args:\n"
      "  - source (numpy.ndarray): input image\n"
      "  - encoding_in (str): input image encoding\n"
      "  - encoding_out (str): encoding to which the image conveted\n"
      "  - do_dynamic_scaling (bool): flag to do dynamic scaling with min/max value\n"
      "  - min_image_value (float): minimum pixel value for dynamic scaling\n"
      "  - max_image_value (float): maximum pixel value for dynamic scaling\n"
  ));
}