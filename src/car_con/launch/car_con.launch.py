#!/usr/bin/env python

# Copyright 1996-2019 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Launch Webots and the controller."""

import os

import launch

from webots_ros2_driver.utils import ControllerLauncher
from webots_ros2_driver.webots_launcher import WebotsLauncher

from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # Webots
    webots = WebotsLauncher(
        world=os.path.join(get_package_share_directory('car_con'), 'worlds', 'car_test.wbt')
    )

    # Controller node
    synchronization = launch.substitutions.LaunchConfiguration('synchronization', default=False)
    controller = ControllerLauncher(package='car_con',
                                    executable='ros_con_test',
                                    parameters=[{'synchronization': synchronization}],
                                    output='screen')
    return launch.LaunchDescription([
        webots,
        controller,
        # Shutdown launch when Webots exits.
        launch.actions.RegisterEventHandler(
            event_handler=launch.event_handlers.OnProcessExit(
                target_action=webots,
                on_exit=[launch.actions.EmitEvent(event=launch.events.Shutdown())],
            )
        ),
    ])

