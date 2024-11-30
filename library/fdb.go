package fdb

import (
	"net"
	"strconv"
)

var addr string = "127.0.0.1:11111"
var conn net.Conn

// Connects to the database
func Connect(ip string, port uint) {
	addr = ip + ":" + strconv.Itoa(int(port))
}

// Read the value of the token from the database
func Read(data []byte) (err error) {
	conn, err = net.Dial("tcp", addr)

	_, err = conn.Write(data)
	return
}

func Close() {
	defer conn.Close()
}
