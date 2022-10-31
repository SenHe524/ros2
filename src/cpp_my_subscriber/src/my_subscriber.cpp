
// #include <chrono>
// #include <functional>
// #include <memory>
// #include <string>
// #include "rclcpp/rclcpp.hpp"
// #include "std_msgs/msg/string.hpp"

// using std::placeholders::_1;

// class MinimalSubscriber : public rclcpp::Node{
// public:
// 	MinimalSubscriber()
// 	: Node("minimal_subscriber")
// 	{
// 		subscription_ = this->create_subscription<std_msgs::msg::String>(
// 		"topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
// 	}

// private:
// 	void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
// 	{
// 		RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
// 	}
// 	rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
// };

// int main(int argc, char * argv[])
// {
// 	rclcpp::init(argc, argv);
// 	rclcpp::spin(std::make_shared<MinimalSubscriber>());
// 	rclcpp::shutdown();
// 	return 0;
// }


#include "rclcpp/rclcpp.hpp"

//  1.导入消息类型文件
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"

//  声明一个占位参数
using std::placeholders::_1;

//  定义一个类
class singledog: public rclcpp::Node
{
public:
    //  构造函数
    singledog(std::string name):Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "大家好，我是单身狗%s！", name.c_str());
        //  3.创建订阅者 发布者
        sub_novel = this->create_subscription<std_msgs::msg::String>(
            "sexy_girl", 10, std::bind(&singledog::novel_callback, this, _1));
        pub_money = this->create_publisher<std_msgs::msg::UInt32>("novel_money", 10);
    }

private:
    //  2.声明订阅者发布者
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_money;
    //  4.编写收到话题后的回调函数
    void novel_callback(const std_msgs::msg::String::SharedPtr novels)
    {
        //  回调函数处理
        //  收到小说后发布稿费
        std_msgs::msg::UInt32 money;
        money.data = 10;
        pub_money->publish(money);
        //  打印日志信息
        RCLCPP_INFO(this->get_logger(),"朕已阅%s!\n", novels->data.c_str());
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
