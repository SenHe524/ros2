###     编写lauch文件
    ##   导入头文件
    from launch import LaunchDescription
    from launch_ros.actions import Node

    ##   定义
    def generate_launch_description():
        #   创建节点描述: 在以下添加需要启动的节点
        #   且节点启动顺序与下面添加的顺序一致
        pub_node = Node(
            package = "cpp_my_publisher",
            executable = "my_publisher"
        )
        srv_node = Node(
            package = "test_srv",
            executable = "my_srv",
            ##  修改参数值
            parameters = [{'novel_price': 2}]
            )
            
        client_node = Node(
            package = "test_client",
            executable = "my_client"
            )
        #   launch_description
        #   实例化LaunchDescription对象
        launch_d = LaunchDescription([
            pub_node, 
            srv_node, 
            client_node
            ])

        return launch_d;

###     python包中修改setup.py/cmake包中修改CMakeList.txt
    ##  python包
        #   添加两个引用
        from glob import glob
        import os
        #   在data_files中添加/**/包含的语句：
        data_files = [
            ('share/ament_index/resource_index/packages',
                ['resource/' + package_name]),
            ('share/' + package_name, ['package.xml']),
            /*
            (os.path.join('share', package_name, ''launch), glov('launch/*.launch.py)),
            */
        ]
    
    ##  cmake包
        #   在CMakeList.txt中添加如下语句：
        install(DIRECTORY launch
        DESTINATION share/${PROJECT_NAME}
)




