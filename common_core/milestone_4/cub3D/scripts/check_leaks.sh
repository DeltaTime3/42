#!/bin/bash

# Memory Leak Detection Script for cub3D
# Usage: ./scripts/check_leaks.sh [map_file]

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default map file
MAP_FILE=${1:-"test_map_cub.cub"}
PROGRAM="./cub3d"
VALGRIND_LOG="valgrind_leak_report.txt"

echo -e "${BLUE} cub3D Memory Leak Detection${NC}"
echo -e "${BLUE}================================${NC}"

# Check if program exists
if [ ! -f "$PROGRAM" ]; then
    echo -e "${RED} Error: $PROGRAM not found. Run 'make' first.${NC}"
    exit 1
fi

# Check if map file exists
if [ ! -f "$MAP_FILE" ]; then
    echo -e "${RED} Error: Map file $MAP_FILE not found.${NC}"
    exit 1
fi

echo -e "${YELLOW} Running Valgrind analysis...${NC}"
echo "Map file: $MAP_FILE"
echo "Program: $PROGRAM"
echo ""

# Run Valgrind with comprehensive leak checking
timeout 15 valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --track-fds=yes \
    --log-file="$VALGRIND_LOG" \
    "$PROGRAM" "$MAP_FILE" 2>/dev/null

# Check if Valgrind ran successfully
if [ ! -f "$VALGRIND_LOG" ]; then
    echo -e "${RED} Valgrind analysis failed${NC}"
    exit 1
fi

echo -e "${BLUE} Analysis Results:${NC}"
echo "===================="

# Extract key information from Valgrind log
HEAP_SUMMARY=$(grep -A 3 "HEAP SUMMARY:" "$VALGRIND_LOG")
LEAK_SUMMARY=$(grep "All heap blocks were freed\|definitely lost\|indirectly lost\|possibly lost" "$VALGRIND_LOG")
ERROR_SUMMARY=$(grep "ERROR SUMMARY:" "$VALGRIND_LOG")

# Display heap usage
echo -e "${YELLOW}Memory Usage:${NC}"
echo "$HEAP_SUMMARY" | while read line; do
    if [[ $line == *"total heap usage"* ]]; then
        echo -e "  ${BLUE}$line${NC}"
    elif [[ $line == *"in use at exit"* ]]; then
        if [[ $line == *"0 bytes in 0 blocks"* ]]; then
            echo -e "  ${GREEN} $line${NC}"
        else
            echo -e "  ${RED} $line${NC}"
        fi
    fi
done

echo ""

# Display leak information
echo -e "${YELLOW}Leak Status:${NC}"
if echo "$LEAK_SUMMARY" | grep -q "All heap blocks were freed"; then
    echo -e "  ${GREEN} No memory leaks detected!${NC}"
elif echo "$LEAK_SUMMARY" | grep -q "definitely lost"; then
    echo -e "  ${RED} Memory leaks found:${NC}"
    echo "$LEAK_SUMMARY" | grep "lost" | sed 's/^/    /'
else
    echo -e "  ${YELLOW}  Check the full report for details${NC}"
fi

echo ""

# Display error summary
echo -e "${YELLOW}Error Summary:${NC}"
echo "$ERROR_SUMMARY" | while read line; do
    if [[ $line == *": 0 errors"* ]]; then
        echo -e "  ${GREEN} $line${NC}"
    else
        echo -e "  ${YELLOW}  $line${NC}"
    fi
done

echo ""
echo -e "${BLUE} Full report saved to: $VALGRIND_LOG${NC}"
echo -e "${BLUE} Tip: Use 'less $VALGRIND_LOG' to view the complete analysis${NC}"
