/*
    编写ros2节点的一般步骤：
    1.导入库文件：#include  "rclcpp/rclcpp.hpp"
    2.初始化客户端库：rclcpp::init(argc, argv);
    3.新建节点对象：
    **
    auto：智能指针
    make_shared：创造指针
    <rclcpp::node>：node型的指针
    ("cpp_test")：初始化node名为："cpp_test"
    **
    auto node_test = std::make_shared<rclcpp::Node>("cpp_test");
    4.spin循环节点：rclcpp::spin(node_test);
    5.关闭客户端库：rclcpp::shutdown();

*/


#include <cstdio>

#include  "rclcpp/rclcpp.hpp"
int main(int argc, char ** argv)
{

    rclcpp::init(argc, argv);
    auto node_test = std::make_shared<rclcpp::Node>("cpp_test");
    RCLCPP_INFO(node_test->get_logger(),"hello cpp test!\n");
    rclcpp::spin(node_test);
    rclcpp::shutdown();

    return 0;
}
