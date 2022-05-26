#!/usr/bin/expect
spawn scp image1.jpg Theo@192.168.79.124:/C:/Users/Theo/Pictures
expect "Password:"
send "password\r"
interact
