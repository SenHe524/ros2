
// #include <chrono>
// #include <functional>
// #include <memory>
// // #include <string>

// #include "rclcpp/rclcpp.hpp"
// #include "std_msgs/msg/string.hpp"

// using namespace std::chrono_literals;

// /* This example creates a subclass of Node and uses std::bind() to register a* member function as a callback from the timer. */

// class Publisher : public rclcpp::Node{
//   public:
//     Publisher()
//     : Node("minimal_publisher"), count_(0)
//     {
//       publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
//       timer_ = this->create_wall_timer(
//       500ms, std::bind(&Publisher::timer_callback, this));
//     }


//   private:
//     void timer_callback()
//     {
//       auto message = std_msgs::msg::String();
//       message.data = "Hello, world! " + std::to_string(count_++);
//       RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
//       publisher_->publish(message);
//     }
//     rclcpp::TimerBase::SharedPtr timer_;
//     rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
//     size_t count_;
//   };


//   int main(int argc, char * argv[])
//   {
//     rclcpp::init(argc, argv);
//     rclcpp::spin(std::make_shared<Publisher>());
//     rclcpp::shutdown();
//     return 0;
//   }

//  C++处理时间的库
#include <chrono>

#include "rclcpp/rclcpp.hpp"
//  1.导入消息类型文件
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"

using namespace std::chrono_literals;

//  声明占位参数
using std::placeholders::_1;

std_msgs::msg::UInt32 money_;
std_msgs::msg::UInt32 count_;

//  声明类
class singledog: public rclcpp::Node
{
public:
    //  构造函数
    singledog(std::string name):Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "大家好，我是单身狗%s！", name.c_str());
        money_.data = 0;
        count_.data = 0;
        //  3.创建发布者订阅者
        pub_novel = this->create_publisher<std_msgs::msg::String>("sexy_girl", 10);
        sub_money = this->create_subscription<std_msgs::msg::UInt32>(
            "novel_money", 10, std::bind(&singledog::money_callback, this, _1));
        timer_ = this->create_wall_timer(1000ms, std::bind(&singledog::timer_callback, this));
    }

private:
    //  3.声明发布者订阅者
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_novel;
    rclcpp::Subscription<std_msgs::msg::UInt32>::SharedPtr sub_money;
    rclcpp::TimerBase::SharedPtr timer_;

    //  2.编写回调函数
    void money_callback(const std_msgs::msg::UInt32::SharedPtr money)
    {
        //  回调函数处理
        money_.data += money->data;
        RCLCPP_INFO(this->get_logger(),"收到稿费%d元,现有稿费%d元\n", money->data, money_.data);
    }
    void timer_callback()
    {
        //  回调函数处理
        std_msgs::msg::String novel;
        novel.data = "novels:" + std::to_string(count_.data++);
        pub_novel->publish(novel);
        RCLCPP_INFO(this->get_logger(),"发布小说!\n");
    }
};



int main(int argc, char ** argv)
{
	//  初始化客户端库
    rclcpp::init(argc, argv);
    //  实例化继承了Node的类
    auto node = std::make_shared<singledog>("wang");
    //  spin循环节点
    rclcpp::spin(node);
    //  关闭客户端库
    rclcpp::shutdown();
    return 0;
}
