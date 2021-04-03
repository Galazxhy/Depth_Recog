#!/bin/bash

sudo systemctl stop depth_recog_autostart.service
sudo systemctl disable depth_recog_autostart.service
sudo systemctl daemon-reload

exit 0
