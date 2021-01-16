# Script to run many independent simulations
# with parameters read from a file, provided as
# a command line argument as background processes.
for i in (cat $argv[1])
    ./viral-simulation (string split " " $i) &
end
