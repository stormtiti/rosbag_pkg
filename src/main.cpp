
#include <ros/ros.h>                     ///< ros error
#include <rosbag/bag.h>                  ///< read from bagfile
#include <sensor_msgs/CompressedImage.h> ///< camera image type
#include <rosbag/view.h>

#include <boost/foreach.hpp>             ///< boost interation

#include <cv_bridge/cv_bridge.h>         ///< opencv bridge
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>

#define foreach BOOST_FOREACH

int main()
{
    std::string OPENCV_WINDOW = "Image window";
    ///< load bag file
    rosbag::Bag bag;
    bag.open("/home/robotics/data/raw_data_2.bag", rosbag::bagmode::Read);

    cv::namedWindow(OPENCV_WINDOW, 0);

    ///< topic setup
    std::vector<std::string> topics;
    topics.push_back(std::string("/camera_16360216/pg_16360216/image_raw/compressed"));

    rosbag::View view(bag, rosbag::TopicQuery(topics));

    ///< loop for fetching image topic
    foreach(rosbag::MessageInstance const m, view)
    {
        sensor_msgs::CompressedImage::ConstPtr img_ptr = m.instantiate<sensor_msgs::CompressedImage>();
        if (img_ptr != NULL)
        {

            cv_bridge::CvImagePtr cv_ptr;
            try
            {
                cv_ptr = cv_bridge::toCvCopy(img_ptr, sensor_msgs::image_encodings::BGR8);
            }
            catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("cv_bridge exception: %s", e.what());
            }

            ///< display cv img
            cv::imshow(OPENCV_WINDOW, cv_ptr->image);
            cv::waitKey(3);
        }
    }

    ///< close rosbag file
    bag.close();
}