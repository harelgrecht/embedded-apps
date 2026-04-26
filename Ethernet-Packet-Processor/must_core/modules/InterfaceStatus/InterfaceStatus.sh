#!/bin/bash

# Define GPIO pin numbers for yellow and green LEDs for each Ethernet device
YELLOW_LED_ETH0=502
GREEN_LED_ETH0=498

YELLOW_LED_ETH1=501
GREEN_LED_ETH1=497

YELLOW_LED_ETH2=504
GREEN_LED_ETH2=500

YELLOW_LED_ETH3=503
GREEN_LED_ETH3=499

# export GPIO
echo "Exporting GPIO pins..."
echo ${YELLOW_LED_ETH0} > /sys/class/gpio/export
echo ${GREEN_LED_ETH0} > /sys/class/gpio/export
echo ${YELLOW_LED_ETH1} > /sys/class/gpio/export
echo ${GREEN_LED_ETH1} > /sys/class/gpio/export
echo ${YELLOW_LED_ETH2} > /sys/class/gpio/export
echo ${GREEN_LED_ETH2} > /sys/class/gpio/export
echo ${YELLOW_LED_ETH3} > /sys/class/gpio/export
echo ${GREEN_LED_ETH3} > /sys/class/gpio/export
echo "GPIO expotred."

# Set up GPIO pins for output
echo "Setting up GPIO pins..."
echo out > /sys/class/gpio/gpio${YELLOW_LED_ETH0}/direction
echo out > /sys/class/gpio/gpio${GREEN_LED_ETH0}/direction
echo out > /sys/class/gpio/gpio${YELLOW_LED_ETH1}/direction
echo out > /sys/class/gpio/gpio${GREEN_LED_ETH1}/direction
echo out > /sys/class/gpio/gpio${YELLOW_LED_ETH2}/direction
echo out > /sys/class/gpio/gpio${GREEN_LED_ETH2}/direction
echo out > /sys/class/gpio/gpio${YELLOW_LED_ETH3}/direction
echo out > /sys/class/gpio/gpio${GREEN_LED_ETH3}/direction
echo "GPIO pins set up."

# Function for checking if a cable is connected for an Ethernet device
is_cable_connected() {
    local device=$1
    local carr=$(cat /sys/class/net/${device}/carrier)
    local operstate=$(cat /sys/class/net/${device}/operstate)

    if [ "$carr" -eq 1 ] && [ "$operstate" == "up" ]; then
        return 0
    else
        return 1
    fi
}

# Function for setting LEDs based on network connection status and traffic
update_eth_status() {
    local device=$1
    local yellow_led=$2
    local green_led=$3

    # Monitor network traffic and blink the green LED if there's activity
    local prev_rx=$(cat /sys/class/net/${device}/statistics/rx_bytes)
    local prev_tx=$(cat /sys/class/net/${device}/statistics/tx_bytes)

    if is_cable_connected; then
        echo 1 > /sys/class/gpio/gpio${yellow_led}/value
        local curr_rx=$(cat /sys/class/net/${device}/statistics/rx_bytes)
        local curr_tx=$(cat /sys/class/net/${device}/statistics/tx_bytes)

        if [ "$curr_rx" -ne "$prev_rx" ] || [ "$curr_tx" -ne "$prev_tx" ]; then
            echo 1 > /sys/class/gpio/gpio${green_led}/value
            sleep 0.7
            echo 0 > /sys/class/gpio/gpio${green_led}/value
        fi
        prev_rx=$curr_rx
        prev_tx=$curr_tx
    fi
    echo 0 > /sys/class/gpio/gpio${yellow_led}/value
    echo 0 > /sys/class/gpio/gpio${green_led}/value
}


# Function for startup LED effect
startup_leds() {
    # Turn on all yellow LEDs and then green LEDs in sequence, wait a bit and turn them off in reverse order
    echo "Running startup LED effect..."
    for i in {1..2}
    do
        echo 1 > /sys/class/gpio/gpio${GREEN_LED_ETH1}/value
        sleep 0.2
        echo 1 > /sys/class/gpio/gpio${YELLOW_LED_ETH1}/value
        sleep 0.2
        echo 1 > /sys/class/gpio/gpio${GREEN_LED_ETH0}/value
        sleep 0.2
        echo 1 > /sys/class/gpio/gpio${YELLOW_LED_ETH0}/value
        sleep 0.2
        echo 1 > /sys/class/gpio/gpio${GREEN_LED_ETH3}/value
        sleep 0.2
        echo 1 > /sys/class/gpio/gpio${YELLOW_LED_ETH3}/value
        sleep 0.2
        echo 1 > /sys/class/gpio/gpio${GREEN_LED_ETH2}/value
        sleep 0.2
        echo 1 > /sys/class/gpio/gpio${YELLOW_LED_ETH2}/value
        sleep 0.2

        echo 0 > /sys/class/gpio/gpio${YELLOW_LED_ETH2}/value
        sleep 0.2
        echo 0 > /sys/class/gpio/gpio${GREEN_LED_ETH2}/value
        sleep 0.2
        echo 0 > /sys/class/gpio/gpio${YELLOW_LED_ETH3}/value
        sleep 0.2
        echo 0 > /sys/class/gpio/gpio${GREEN_LED_ETH3}/value
        sleep 0.2
        echo 0 > /sys/class/gpio/gpio${YELLOW_LED_ETH0}/value
        sleep 0.2
        echo 0 > /sys/class/gpio/gpio${GREEN_LED_ETH0}/value
        sleep 0.2
        echo 0 > /sys/class/gpio/gpio${YELLOW_LED_ETH1}/value
        sleep 0.2
        echo 0 > /sys/class/gpio/gpio${GREEN_LED_ETH1}/value
        sleep 0.2
    done

    echo "Startup LED effect complete."
}

# Call startup LED effect function
startup_leds

# Continuously check network connection and set LEDs for each Ethernet device
while true; do
    update_eth_status eth0 ${YELLOW_LED_ETH0} ${GREEN_LED_ETH0} &
    update_eth_status eth1 ${YELLOW_LED_ETH1} ${GREEN_LED_ETH1} &
    update_eth_status eth2 ${YELLOW_LED_ETH2} ${GREEN_LED_ETH2} &
    update_eth_status eth3 ${YELLOW_LED_ETH3} ${GREEN_LED_ETH3} &
done

