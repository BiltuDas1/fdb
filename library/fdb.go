package fdb

import (
	"net"
	"strconv"
)

var conn net.Conn

// Connects to the database server
//
// Returns error same as https://pkg.go.dev/net#Dial
func Connect(ip string, port uint) (err error) {
	addr := ip + ":" + strconv.Itoa(int(port))
	conn, err = net.Dial("tcp", addr)
	return
}

// Writes the data on the database
func Write(url string, tokens []string, metadatas map[string]string) (err error) {
	return
}

// Read the value of the token from the database
func Read(token string) (response []byte, err error) {
	_, err = conn.Write([]byte(token))
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
