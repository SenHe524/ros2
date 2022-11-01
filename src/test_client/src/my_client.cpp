//  C++处理时间的库
#include <chrono>
// #include <functional>
// #include <memory>
// #include <string>

#include "rclcpp/rclcpp.hpp"
//  1.导入消息类型文件
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include "cpp_my_interfaces/srv/servicetest.hpp"

using namespace std::chrono_literals;

//  声明占位参数
using std::placeholders::_1;

//  声明类
class poorman: public rclcpp::Node
{
public:
    //  构造函数
    poorman(std::string name):Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "大家好，我是穷鬼%s！", name.c_str());
        //  3.创建客户端
        novel_client = this->create_client<cpp_my_interfaces::srv::Servicetest>("Sell_Novel");
    }
    void buy_novels()
    {
        RCLCPP_INFO(this->get_logger(), "去买小说！");
        //  等待服务器上线
        while(!novel_client->wait_for_service(std::chrono::seconds(1)))
        {
            if(!rclcpp::ok())
                    return ;
            RCLCPP_WARN(this->get_logger(), "等待服务器上线...");
        }
        //  构造请求数据
        auto request = std::make_shared<cpp_my_interfaces::srv::Servicetest_Request>();
        request->money = 5;
        //  发送异步请求
        novel_client->async_send_request(request, std::bind(&poorman::novel_callback, this, _1));
    }

private:
    //  声明客户端
    rclcpp::Client<cpp_my_interfaces::srv::Servicetest>::SharedPtr novel_client;

    //  接收小说的回调函数
    void novel_callback(rclcpp::Client<cpp_my_interfaces::srv::Servicetest>::SharedFuture response)
    {
        //  回调函数处理
        auto result = response.get();
        RCLCPP_INFO(this->get_logger(),"收到%d章小说,内容如下：\n", result->novels.size());
        for(std::string novel:result->novels)
        {
            RCLCPP_INFO(this->get_logger(),"%s\n", novel.c_str());
        }
    }

};



int main(int argc, char ** argv)
{
	//  初始化客户端库
    rclcpp::init(argc, argv);
    //  实例化继承了Node的类
    auto node = std::make_shared<poorman>("zheng");
    //  spin循环节点
    node->buy_novels();
    rclcpp::spin(node);
    //  关闭客户端库
    rclcpp::shutdown();
    return 0;
}
