#include "web_video_server/web_video_server.h"

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto nh = std::make_shared<rclcpp::Node>("web_video_server");
  auto private_nh = std::make_shared<rclcpp::Node>("_web_video_server");

  web_video_server::WebVideoServer server(nh, private_nh);
  server.setup_cleanup_inactive_streams();
  server.spin();

  return (0);
}
