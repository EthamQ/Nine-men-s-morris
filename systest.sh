#!/bin/bash

LOGFILE1="output1.log"
LOGFILE2="output2.log"

echo -e "\e[31m"
echo "┏┏┓┏━┓┳━┓┳━┓o┓━┓┏━┓┏━┓"
echo "┃┃┃┃ ┃┃┳┛┃┳┛┃┗━┓┃/┃ ━┫"
echo "┛ ┇┛━┛┇┗┛┇┗┛┇━━┛┛━┛┗━┛"
echo -e "\e[0m"
echo "This script will now run 100 games of AI vs AI."
echo "Please make sure to compile the binary beforehand."
echo -e "Log files are at \e[32m$LOGFILE1\e[0m and \e[32m$LOGFILE2\e[0m."
echo "Are you ready?"
read

create_id () {
    # Source: sysprak-abgabe.sh 1.0.0
    ID=$(curl http://sysprak.priv.lab.nm.ifi.lmu.de/api/v1/matches \
    -H "Content-Type: application/json" \
    -X POST \
    -d '{"type":"NMMorris","gameGeneric":{"name":"","timeout":3000},"gameSpecific":{},"players":[{"name":"White Player","type":"COMPUTER"},{"name":"Black Player","type":"COMPUTER"}]}' 2>/dev/null | grep -Eow '([a-z0-9]{13})')

    if [[ $ID == "" ]] ; then
        echo "Error creating new Game-ID. Exiting...";
        exit;
    fi
}

clear_logs() {
    >$LOGFILE1
    >$LOGFILE2
}

run_matches() {
    for i in `seq 1 100`;
    do
        create_id

        echo "Running match $i with ID $ID."

        ./sysprak-client -g $ID &> $LOGFILE1 &
        ./sysprak-client -g $ID &> $LOGFILE2 &

        wait
    done  
}

clear_logs
run_matches

echo -e "\nWe're done. Here's a grep on TIMEOUT:\n"
grep -iTrn TIMEOUT $LOGFILE1 $LOGFILE2
grep -iTrn "S: -" $LOGFILE1 $LOGFILE2

