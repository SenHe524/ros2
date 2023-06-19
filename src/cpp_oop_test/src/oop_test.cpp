#include <cstdio>
#include "rclcpp/rclcpp.hpp"

class single_dogNode:public rclcpp::Node
{
private:

public:
	single_dogNode(std::string name):Node(name)
	{
		RCLCPP_INFO(this->get_logger(), "大家好，我是%s!，我再也不是单身狗了！\n",name.c_str());
	}
	~single_dogNode()
	{
		
	}
};


int main(int argc, char ** argv)
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<single_dogNode>("aduan");
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}
