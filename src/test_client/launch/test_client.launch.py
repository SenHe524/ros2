#   导入头文件
from launch import LaunchDescription
from launch_ros.actions import Node

#   定义
def generate_launch_description():
    #   创建节点描述
    pub_node = Node(
        package = "cpp_my_publisher",
        executable = "my_publisher"
    )

    srv_node = Node(
        package = "test_srv",
        executable = "my_srv",
        parameters = [{'novel_price': 2}]
        )

    client_node = Node(
        package = "test_client",
        executable = "my_client"
        )
    #   launch_description
    launch_d = LaunchDescription([pub_node, srv_node, client_node])

    return launch_d;