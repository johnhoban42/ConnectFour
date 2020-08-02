/* Header for definitions and constants used throughout the project */
#pragma once

// Standard library includes
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>

// Networking
#define HOST "cs.rutgers.edu"
#define PORT 7777

// Status codes
#define SUCCESS 0
#define FAILURE -1

// Game state data
#define BLACK 1
#define RED 10
