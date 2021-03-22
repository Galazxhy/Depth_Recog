#!/bin/bash

sudo cp -p ./depth_recog_autostart.service /etc/systemd/system
sudo chmod +x /etc/systemd/system/depth_recog_autostart.service

sudo systemctl daemon-reload
sudo systemctl enable depth_recog_autostart.service
sudo systemctl start depth_recog_autostart.service

exit 0
