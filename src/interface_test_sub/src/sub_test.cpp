//  C++处理时间的库
#include <chrono>

#include "rclcpp/rclcpp.hpp"
//  1.导入消息类型文件
// #include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "cpp_my_interfaces/msg/test.hpp"
using namespace std::chrono_literals;

//  声明占位参数
using std::placeholders::_1;

cpp_my_interfaces::msg::Test axis1;


//  声明类
class singledog: public rclcpp::Node
{
public:
    //  构造函数
    singledog(std::string name):Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "大家好，我是%s！", name.c_str());

        //  3.创建发布者订阅者
        pub_test = this->create_publisher<cpp_my_interfaces::msg::Test>("my_axis", 10);
        sub_test = this->create_subscription<cpp_my_interfaces::msg::Test>(
            "yes_or_not", 10, std::bind(&singledog::meg_callback, this, _1));
        timer_ = this->create_wall_timer(1000ms, std::bind(&singledog::timer_callback, this));
    }

private:
    //  3.声明发布者订阅者
    rclcpp::Publisher<cpp_my_interfaces::msg::Test>::SharedPtr pub_test;
    rclcpp::Subscription<cpp_my_interfaces::msg::Test>::SharedPtr sub_test;
    rclcpp::TimerBase::SharedPtr timer_;
    cpp_my_interfaces::msg::Test axis;

    //  2.编写回调函数
    void meg_callback(const cpp_my_interfaces::msg::Test::SharedPtr msg)
    {
        //  回调函数处理
        RCLCPP_INFO(this->get_logger(),"收到消息:%s\n",msg->msgs.data.c_str());
        if(msg->flag.data)
        {
            RCLCPP_INFO(this->get_logger(),"哈哈哈,(%d,%d)的人是单身狗!\n",msg->x1.data,msg->y1.data);
        }
        else
        {
            RCLCPP_INFO(this->get_logger(),"你居然不是单身狗,我不跟你玩,你走开!\n");
        }
    }
    void timer_callback()
    {
        //  回调函数处理
        this->axis.flag.set__data(false);
        this->axis.x1.data = 7;
        this->axis.y1.data = 4;
        this->axis.msgs.data = "我是一个有女朋友的人！";
        pub_test->publish(this->axis);
        RCLCPP_INFO(this->get_logger(),"发布坐标!\n");
    }
};



int main(int argc, char ** argv)
{
	//  初始化客户端库
    rclcpp::init(argc, argv);
    //  实例化继承了Node的类
    auto node = std::make_shared<singledog>("li");
    //  spin循环节点
    rclcpp::spin(node);
    //  关闭客户端库
    rclcpp::shutdown();
    return 0;
}
