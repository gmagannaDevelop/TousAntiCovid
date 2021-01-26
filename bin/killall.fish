# Script to kill all background processes that
# were spawned via a call to runall.fish
for i in (ps aux | grep viral-simulation | grep -v grep | awk '{print $2}') 
    kill -15 $i
end
