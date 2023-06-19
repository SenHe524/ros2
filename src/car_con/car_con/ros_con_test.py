"""ROS2 example controller."""
# You may need to import some classes of the controller module. Ex:
#  from controller import Robot, Motor, DistanceSensor
import rclpy
from webots_ros2_msgs.srv import SetDifferentialWheelSpeed

from std_msgs.msg import Float64
from webots_ros2_driver import webots_launcher
from geometry_msgs.msg import Twist

class ExampleController():

    def __init__(self, webots_node, args):
        super().__init__('example_controller', args)

        self.__robot = webots_node.robot

        self.motor_fl = self.__robot.getMotor('jointfl')
        self.motor_fr = self.__robot.getMotor('jointfr')
        self.motor_bl = self.__robot.getMotor('jointbl')
        self.motor_br = self.__robot.getMotor('jointbr')
        self.motor_fl.setPosition(float('inf'))
        self.motor_bl.setPosition(float('inf'))
        self.motor_fr.setPosition(float('inf'))
        self.motor_br.setPosition(float('inf'))
        self.motor_fl.setVelocity(0)
        self.motor_bl.setVelocity(0)
        self.motor_fr.setVelocity(0)
        self.motor_br.setVelocity(0)
        self.ds = self.__robot.getPositionSensor('jointlink_sensor')
        self.ds.enable(self.timestep)
        self.gps = self.__robot.getGPS('gps_body')
        self.gps.enable(self.timestep)
        self.cmdVelSubscriber = self.create_subscription(Twist, 'cmd_vel',
                                                         self.cmdVel_callback,
                                                         10)
        print('inited')

    def cmdVel_callback(self, msg):
        print('cmdVel_callback')
        wheel_r = 0.035 #m 轮子半径
        wheel_dis = 0.145 # 左右轮子距离
        self.vel = msg.linear.x
        self.ang = msg.angular.z
        vr = (self.vel + self.ang * wheel_dis/2 ) /wheel_r 
        vl = (self.vel - self.ang * wheel_dis/2) /wheel_r 
        self.motor_fl.setVelocity(-vl)
        self.motor_bl.setVelocity(-vl)
        self.motor_fr.setVelocity(vr)
        self.motor_br.setVelocity(vr)
        print('vel:',self.vel,self.ang)
        print('vr:',vr,'  vl',vl)



def main(args=None):
    rclpy.init(args=args)

    exampleController = ExampleController(args=args)

    rclpy.spin(exampleController)
    rclpy.shutdown()

if __name__ == '__main__':
   main()


