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
func Read(data []byte) (response []byte, err error) {
	conn, err = net.Dial("tcp", addr)
	if err != nil {
		return
	}

	_, err = conn.Write(data)
	if err != nil {
		return
	}

	buffer := make([]byte, 100)
	n, err := conn.Read(buffer)
	if err != nil {
		return
	}

	return buffer[:n], nil
}

func Close() {
	if conn != nil {
		defer conn.Close()
	}
}
