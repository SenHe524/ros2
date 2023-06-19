from setuptools import setup

package_name = 'car_con'
data_files = []
data_files.append(('share/ament_index/resource_index/packages', ['resource/' + package_name]))
data_files.append(('share/' + package_name, ['package.xml']))
#launch 文件
data_files.append(('share/' + package_name, ['launch/car_con.launch.py']))
#机器人的环境
data_files.append(('share/' + package_name + '/worlds', [
    'worlds/car_test.wbt','worlds/.car_test.wbproj',
    'worlds/ros_example.wbt', 'worlds/.ros_example.wbproj', 
]))
#机器人的模型
data_files.append(('share/' + package_name + '/protos', [
    'protos/CarTest.proto'                               
]))

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=data_files,
    install_requires=['setuptools', 'launch'],
    zip_safe=True,
    maintainer='xu',
    maintainer_email='2514632982@qq.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [  
            #添加可执行的文件  
            'ros_con_test = car_con.ros_con_test:main'
        ],
        'launch.frontend.launch_extension': ['launch_ros = launch_ros']
    },
)
