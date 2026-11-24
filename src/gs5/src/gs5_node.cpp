#include <chrono>
#include <memory>
#include <string>
#include <bits/stdc++.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using namespace std::chrono_literals;



class Gs5Node : public rclcpp::Node
{
  public:
    Gs5Node()
    : Node("Gs5_Node")
    {
      scan_in_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "laser_scan", 10,
      std::bind(&Gs5Node::scan_to_string(), this, std::placeholders::_1));

      publisher_ = this->create_publisher<sensor_msgs::msg::String>(output_topic_, 10);
      
        auto topic_callback =
        [this](const sensor_msgs::msg::LaserScan & msg) -> void {

          RCLCPP_INFO(this->get_logger(), "I Heard: '%s'", msg.data.c_str());

          auto out = std_msgs::msg::String();
          out.data = "Relayed: " + msg.data; 
          this->publisher_->publish(out);
        };
    }
  private:
    const std::string output_topic_ = "output_topic";

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_in_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    float range_min_;
    float range_max_;
    std::string string_out;

    std::string scan_to_string(const sensor_msgs::msg::LaserScan & msg) {
      range_min_ = msg.range_min;
      range_max_ = msg.range_max;

      std::string string_out = std::to_string(range_min_) + ", " + (std::to_string(range_max_)) + "\n";
    }
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Gs5Node>());
  rclcpp::shutdown();
  return 0;
}


/*
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>(topic_name_, 10);
    auto timer_callback =
      [this]() -> void {
        auto message = std_msgs::msg::String();
        message.data = "Hello, world! " + std::to_string(this->count_++);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        this->publisher_->publish(message);
      };
    timer_ = this->create_wall_timer(500ms, timer_callback);
  }

private:
  const std::string topic_name_ = "topic";
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}

*/


/*
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

//This example creates a subclass of Node

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(this->count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    this->publisher_->publish(message);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
*/
