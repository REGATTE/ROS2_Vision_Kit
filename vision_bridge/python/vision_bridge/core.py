import sys

import sensor_msgs.msg


class VisionBridgeError(TypeError):
    """This is the error raised by :class:`vision_bridge.VisionBridge` methods when they fail."""

    pass


class VisionBridge(object):
    """
    The VisionBridge is an object that converts between OpenCV Images and ROS Image messages.

       .. doctest::
           :options: -ELLIPSIS, +NORMALIZE_WHITESPACE

           >>> import cv2
           >>> import numpy as np
           >>> from vision_bridge import VisionBridge
           >>> br = VisionBridge()
           >>> dtype, n_channels = br.encoding_as_visiontype2('8UC3')
           >>> im = np.ndarray(shape=(480, 640, n_channels), dtype=dtype)
           >>> msg = br.cv2_to_imgmsg(im)  # Convert the image to a message
           >>> im2 = br.imgmsg_to_cv2(msg) # Convert the message to a new image
           >>> # Convert the image to a compress message
           >>> cmprsmsg = br.cv2_to_compressed_imgmsg(im)
           >>> # Convert the compress message to a new image
           >>> im22 = br.compressed_imgmsg_to_cv2(msg)
           >>> cv2.imwrite("this_was_a_message_briefly.png", im2)

    """

    def __init__(self):
        import cv2
        self.visiontype_to_name = {}
        self.visiondepth_to_numpy_depth = {cv2.CV_8U: 'uint8', cv2.CV_8S: 'int8',
                                       cv2.CV_16U: 'uint16', cv2.CV_16S: 'int16',
                                       cv2.CV_32S: 'int32', cv2.CV_32F: 'float32',
                                       cv2.CV_64F: 'float64'}

        for t in ['8U', '8S', '16U', '16S', '32S', '32F', '64F']:
            for c in [1, 2, 3, 4]:
                nm = '%sC%d' % (t, c)
                self.visiontype_to_name[getattr(cv2, f'CV_{nm}')] = nm

        self.numpy_type_to_visiontype = {'uint8': '8U', 'int8': '8S', 'uint16': '16U', 'int16': '16S', 'int32': '32S', 'float32': '32F', 'float64': '64F'} | {v: k for (k, v) in self.numpy_type_to_visiontype.items()}

    def dtype_with_channels_to_visiontype2(self, dtype, n_channels):
        return '%sC%d' % (self.numpy_type_to_visiontype[dtype.name], n_channels)

    def visiontype2_to_dtype_with_channels(self, visiontype):
        from vision_bridge.boost.vision_bridge_boost import VISION_MAT_CNWrap, VISION_MAT_DEPTHWrap
        return self.visiondepth_to_numpy_depth[VISION_MAT_DEPTHWrap(visiontype)], VISION_MAT_CNWrap(visiontype)

    def encoding_to_visiontype2(self, encoding):
        from vision_bridge.boost.vision_bridge_boost import getVisionType

        try:
            return getVisionType(encoding)
        except RuntimeError as e:
            raise VisionBridgeError(e)

    def encoding_to_dtype_with_channels(self, encoding):
        return self.visiontype2_to_dtype_with_channels(self.encoding_to_visiontype2(encoding))

    def compressed_imgmsg_to_cv2(self, cmprs_img_msg, desired_encoding='passthrough'):
        """
        Convert a sensor_msgs::CompressedImage message to an OpenCV :cpp:type:`cv::Mat`.

        :param cmprs_img_msg:   A :cpp:type:`sensor_msgs::CompressedImage` message
        :param desired_encoding:  The encoding of the image data, one of the following strings:

           * ``"passthrough"``
           * one of the standard strings in sensor_msgs/image_encodings.h

        :rtype: :cpp:type:`cv::Mat`
        :raises VisionBridgeError: when conversion is not possible.

        If desired_encoding is ``"passthrough"``, then the returned image has the same format
        as img_msg. Otherwise desired_encoding must be one of the standard image encodings

        This function returns an OpenCV :cpp:type:`cv::Mat` message on success,
        or raises :exc:`vision_bridge.VisionBridgeError` on failure.

        If the image only has one channel, the shape has size 2 (width and height)
        """
        import cv2
        import numpy as np

        str_msg = cmprs_img_msg.data
        buf = np.ndarray(shape=(1, len(str_msg)),
                         dtype=np.uint8, buffer=cmprs_img_msg.data)
        im = cv2.imdecode(buf, cv2.IMREAD_ANYCOLOR)

        if desired_encoding == 'passthrough':
            return im

        from vision_bridge.boost.vision_bridge_boost import cvtColor2

        try:
            res = cvtColor2(im, 'bgr8', desired_encoding)
        except RuntimeError as e:
            raise VisionBridgeError(e)

        return res

    def imgmsg_to_cv2(self, img_msg, desired_encoding='passthrough'):
        """
        Convert a sensor_msgs::Image message to an OpenCV :cpp:type:`cv::Mat`.

        :param img_msg:   A :cpp:type:`sensor_msgs::Image` message
        :param desired_encoding:  The encoding of the image data, one of the following strings:

           * ``"passthrough"``
           * one of the standard strings in sensor_msgs/image_encodings.h

        :rtype: :cpp:type:`cv::Mat`
        :raises VisionBridgeError: when conversion is not possible.

        If desired_encoding is ``"passthrough"``, then the returned image has the same format
        as img_msg. Otherwise desired_encoding must be one of the standard image encodings

        This function returns an OpenCV :cpp:type:`cv::Mat` message on success,
        or raises :exc:`vision_bridge.VisionBridgeError` on failure.

        If the image only has one channel, the shape has size 2 (width and height)
        """
        import numpy as np
        dtype, n_channels = self.encoding_to_dtype_with_channels(img_msg.encoding)
        dtype = np.dtype(dtype)
        dtype = dtype.newbyteorder('>' if img_msg.is_bigendian else '<')

        img_buf = np.asarray(img_msg.data, dtype=dtype) if isinstance(img_msg.data, list) else img_msg.data

        if n_channels == 1:
            im = np.ndarray(shape=(img_msg.height, int(img_msg.step/dtype.itemsize)),
                            dtype=dtype, buffer=img_buf)
            im = np.ascontiguousarray(im[:img_msg.height, :img_msg.width])
        else:
            im = np.ndarray(shape=(img_msg.height, int(img_msg.step/dtype.itemsize/n_channels), n_channels),
                            dtype=dtype, buffer=img_buf)
            im = np.ascontiguousarray(im[:img_msg.height, :img_msg.width, :])

        # If the byte order is different between the message and the system.
        if img_msg.is_bigendian == (sys.byteorder == 'little'):
            im = im.byteswap().newbyteorder()

        if desired_encoding == 'passthrough':
            return im

        from vision_bridge.boost.vision_bridge_boost import cvtColor2

        try:
            res = cvtColor2(im, img_msg.encoding, desired_encoding)
        except RuntimeError as e:
            raise VisionBridgeError(e)

        return res

    def cv2_to_compressed_imgmsg(self, cvim, dst_format='jpg'):
        """
        Convert an OpenCV :cpp:type:`cv::Mat` type to a ROS sensor_msgs::CompressedImage message.

        :param cvim:      An OpenCV :cpp:type:`cv::Mat`
        :param dst_format:  The format of the image data, one of the following strings:

        http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html
        http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#Mat
        * imread(const string& filename, int flags)
           * bmp, dib
           * jpeg, jpg, jpe
           * jp2
           * png
           * pbm, pgm, ppm
           * sr, ras
           * tiff, tif

        :rtype:           A sensor_msgs.msg.CompressedImage message
        :raises VisionBridgeError: when the ``cvim`` has a type that is incompatible with ``format``


        This function returns a sensor_msgs::Image message on success,
        or raises :exc:`vision_bridge.VisionBridgeError` on failure.
        """
        import cv2
        import numpy as np
        if not isinstance(cvim, (np.ndarray, np.generic)):
            raise TypeError('Your input type is not a numpy array')
        cmprs_img_msg = sensor_msgs.msg.CompressedImage()
        cmprs_img_msg.format = dst_format
        ext_format = f'.{dst_format}'
        try:
            cmprs_img_msg.data.frombytes(np.array(cv2.imencode(ext_format, cvim)[1]).tobytes())
        except RuntimeError as e:
            raise VisionBridgeError(e)

        return cmprs_img_msg

    def cv2_to_imgmsg(self, cvim, encoding='passthrough', header = None):
        """
        Convert an OpenCV :cpp:type:`cv::Mat` type to a ROS sensor_msgs::Image message.

        :param cvim:      An OpenCV :cpp:type:`cv::Mat`
        :param encoding:  The encoding of the image data, one of the following strings:

           * ``"passthrough"``
           * one of the standard strings in sensor_msgs/image_encodings.h
        :param header:    A std_msgs.msg.Header message

        :rtype:           A sensor_msgs.msg.Image message
        :raises VisionBridgeError: when the ``cvim`` has a type that is incompatible with ``encoding``

        If encoding is ``"passthrough"``, then the message has the same encoding as the image's
        OpenCV type. Otherwise desired_encoding must be one of the standard image encodings

        This function returns a sensor_msgs::Image message on success,
        or raises :exc:`vision_bridge.VisionBridgeError` on failure.
        """
        import numpy as np
        if not isinstance(cvim, (np.ndarray, np.generic)):
            raise TypeError('Your input type is not a numpy array')
        img_msg = sensor_msgs.msg.Image()
        img_msg.height = cvim.shape[0]
        img_msg.width = cvim.shape[1]
        if header is not None:
            img_msg.header = header
        if len(cvim.shape) < 3:
            vision_type = self.dtype_with_channels_to_visiontype2(cvim.dtype, 1)
        else:
            vision_type = self.dtype_with_channels_to_visiontype2(cvim.dtype, cvim.shape[2])
        if encoding == 'passthrough':
            img_msg.encoding = vision_type
        else:
            img_msg.encoding = encoding
            # Verify that the supplied encoding is compatible with the type of the OpenCV image
            if self.visiontype_to_name[self.encoding_to_visiontype2(encoding)] != vision_type:
                raise VisionBridgeError(f'encoding specified as {encoding}, but image has incompatible type {vision_type}')

        if cvim.dtype.byteorder == '>':
            img_msg.is_bigendian = True
        img_msg.data.frombytes(cvim.tobytes())
        img_msg.step = len(img_msg.data) // img_msg.height

        return img_msg