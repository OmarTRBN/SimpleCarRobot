#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <termios.h>
#include <unistd.h>

int getch()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "keyboard_teleop");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    geometry_msgs::Twist msg;

    ROS_INFO("Use W/A/S/D to move the robot, and P to stop it. Press 'q' to quit.");

    while (ros::ok())
    {
        int ch = getch();
        std::cout << "The char is: " << static_cast<char>(ch) << std::endl;
        switch (ch)
        {   
            case 'w':
                msg.linear.x = 3;
                msg.angular.z = 0.0;
                break;

            case 's':
                msg.linear.x = -3;
                msg.angular.z = 0.0;
                break;

            case 'a':
                msg.linear.x = 0.0;
                msg.angular.z = 3;
                break;

            case 'd':
                msg.linear.x = 0.0;
                msg.angular.z = -3;
                break;

            case 'p':
                msg.linear.x = 0.0;
                msg.angular.z = 0.0;
                break;

            case 'q':
                msg.linear.x = 0.0;
                msg.angular.z = 0.0;
                pub.publish(msg);
                return 0;

            default:
                msg.linear.x = 0.0;
                msg.angular.z = 0.0;
                break;
        }

        pub.publish(msg);
        ros::spinOnce();
    }

    return 0;
}
