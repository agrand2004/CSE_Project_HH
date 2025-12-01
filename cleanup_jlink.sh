#!/bin/bash
# Quick script to kill any stuck J-Link GDB Server processes
pkill -9 JLinkGDBServer
echo "✓ Cleaned up J-Link processes"
