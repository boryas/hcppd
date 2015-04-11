#! /bin/sh

pkill hcppd
sudo ./hcppd --dynamic_content_server $1
