#!/bin/bash

sudo systemctl stop depth_recog_auto_autostart.service
sudo rm -r /etc/systemd/system/depth_recog_autostart.service
sudo systemctl daemon-reload

exit 0
