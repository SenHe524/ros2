#include <cstdio>
#include <string>
//  C++处理时间的库
#include <chrono>
//  1.导入消息类型文件
#include "rclcpp/rclcpp.hpp"
#include "cpp_my_interfaces/srv/servicetest.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include <queue>
using namespace std::chrono_literals;

//  声明占位参数
using std::placeholders::_1;
using std::placeholders::_2;

//  声明类
class singledog: public rclcpp::Node
{
public:
    //  构造函数
    singledog(std::string name):Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "大家好，我是%s！", name.c_str());
        //  创建订阅者
        sub_novel = this->create_subscription<std_msgs::msg::String>("sexy_girl", 10,
                        std::bind(&singledog::novel_callback, this, _1));
        //  创建发布者
        pub_money = this->create_publisher<std_msgs::msg::UInt32>("novel_money", 10);
        //  声明并创建服务器，并自定义回调组
        sell_novel_group = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        sell_server = this->create_service<cpp_my_interfaces::srv::Servicetest>("Sell_Novel", 
            std::bind(&singledog::sell_novel_callback, this, _1, _2), rmw_qos_profile_services_default, sell_novel_group);
        this->declare_parameter<std::int64_t>("novel_price", this->novel_price);
    }

private:
    //  声明订阅者
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;
    //  声明发布者
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_money;
    //  声明服务器
    rclcpp::Service<cpp_my_interfaces::srv::Servicetest>::SharedPtr sell_server;
    //  声明回调组，实现多线程
    rclcpp::CallbackGroup::SharedPtr sell_novel_group;
    //  声明小说队列
    std::queue<std::string> novel_queue;
    //  声明书的单价
    unsigned int novel_price = 1;
    
    void novel_callback(const std_msgs::msg::String::SharedPtr novels)
    {
        //  收到小说后发布稿费
        std_msgs::msg::UInt32 money;
        money.data = 10;
        pub_money->publish(money);
        novel_queue.push(novels->data);
        RCLCPP_INFO(this->get_logger(), "收到小说:%s", novels->data.c_str());
    }

    //  编写回调函数
    void sell_novel_callback(const cpp_my_interfaces::srv::Servicetest::Request::SharedPtr request,
                    const cpp_my_interfaces::srv::Servicetest::Response::SharedPtr response)
    {
        RCLCPP_INFO(this->get_logger(), "收到一个买书的请求，一共给了%d元",request->money);
        //  计算应该给客户端的小说数量
        this->get_parameter("novel_price", this->novel_price);
        size_t num = (int)request->money / this->novel_price;
        //  判断当前书库中的书是否足够
        if(num > novel_queue.size())
        {
            RCLCPP_INFO(this->get_logger(), "当前书库中有书%d本，不满足客户端所买书籍数量", novel_queue.size(), num);
            rclcpp::Rate rate(1);
            //  此时需要等待书库中书的数量足够
            while(novel_queue.size() < num)
            {
                if(!rclcpp::ok())
                    return ;
                RCLCPP_INFO(this->get_logger(), "等待中，目前还差%d本书", num - novel_queue.size());
                rate.sleep();
            }
        }
        RCLCPP_INFO(this->get_logger(), "当前书库中有书%d本，可以售卖给客户端%d本", novel_queue.size(), num);

        for(int i = 0; i < (int)num; i++)
        {
            response->novels.push_back(novel_queue.front());
            novel_queue.pop();

        }

    }

};



int main(int argc, char ** argv)
{
	//  初始化客户端库
    rclcpp::init(argc, argv);
    //  实例化继承了Node的类
    auto node = std::make_shared<singledog>("li");
    //  声明多线程执行器
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    //  spin循环节点
    // rclcpp::spin(node);
    executor.spin();
    //  关闭客户端库
    // executor.remove_node(node);
    rclcpp::shutdown();
    return 0;
}
