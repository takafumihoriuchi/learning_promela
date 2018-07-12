// S ->   message   -> R
chan s2r = [0] of {byte, bit};

// S <- acknowledge <- R
chan r2s = [0] of {bit};


// sends three kinds of messages (m_0, m_1, m_2) in specific sequence
proctype S() {
    
    byte message;
    bit sending_bit = 0;
    bit recieved_bit;

    // m_0 (zero or more but not infinity)
    message = 0;
    do
    ::  do
        :: s2r ! message, sending_bit ->
            if
            :: r2s ? recieved_bit ->
                if
                :: (recieved_bit == sending_bit) ->
                    sending_bit = 1 - sending_bit;
                    break;
                :: else -> skip
                fi;
            :: timeout
            fi;
        od;
    ::  break;
    od;
    
    // m_1
    message = 1;
    do
    :: s2r ! message, sending_bit ->
        if
        :: r2s ? recieved_bit ->
            if
            :: (recieved_bit == sending_bit) ->
                sending_bit = 1 - sending_bit;
                break;
            :: else -> skip
            fi;
        :: timeout
        fi;
    od;

    // m_0 (zero or more but not infinity)
    do
    ::  do
        :: s2r ! message, sending_bit ->
            if
            :: r2s ? recieved_bit ->
                if
                :: (recieved_bit == sending_bit) ->
                    sending_bit = 1 - sending_bit;
                    break;
                :: else -> skip
                fi;
            :: timeout
            fi;
        od;
    ::  break;
    od;
    
    // m_2
    message = 2;
    do
    :: s2r ! message, sending_bit ->
        if
        :: r2s ? recieved_bit ->
            if
            :: (recieved_bit == sending_bit) ->
                sending_bit = 1 - sending_bit;
                break;
            :: else -> skip
            fi;
        :: timeout
        fi;
    od;
    
    // m_0 (more than once, possibly infinity)
    message = 0;
    do
    ::  do
        :: s2r ! message, sending_bit ->
            if
            :: r2s ? recieved_bit ->
                if
                :: (recieved_bit == sending_bit) ->
                    sending_bit = 1 - sending_bit;
                    break;
                :: else -> skip
                fi;
            :: timeout
            fi;
        od;
    od;
}


// run by: spin -search -a abp_iv.pml
// when recieve specific sequence passes an accept label infinitely many times
proctype R() {

    byte message;
    bit recieved_bit;
    bit expected_bit = 0;

L1: do
    :: s2r ? message, recieved_bit ->
        if
        :: (recieved_bit == expected_bit) ->
            expected_bit = 1 - expected_bit;
        :: else -> skip;
        fi;
        r2s ! recieved_bit;
        if
        :: (message == 1) ->
            do
            :: s2r ? message, recieved_bit ->
                if
                :: (recieved_bit == expected_bit) ->
                    expected_bit = 1 - expected_bit;
                :: else -> skip;
                fi;
                r2s ! recieved_bit;
                if
                :: (message == 2) ->
                    do
                    :: s2r ? message, recieved_bit ->
                        if
                        :: (recieved_bit == expected_bit) ->
                            expected_bit = 1 - expected_bit;
                        :: else -> skip;
                        fi;
                        r2s ! recieved_bit;
                        if
                        :: (message == 0) -> // sequence accepted
accept_infinite_zero_tail:  do // checks for infinite loops
                            :: s2r ? message, recieved_bit ->
                                if
                                :: (recieved_bit == expected_bit) ->
                                    expected_bit = 1 - expected_bit;
                                :: else -> skip;
                                fi;
                                r2s ! recieved_bit;
                                if
                                :: (message == 0) -> skip; // continuous m_0 at tail
                                :: (message != 0) -> goto L1;
                                fi;
                            od;
                        :: else -> goto L1;
                        fi;
                    od;
                :: else -> skip;
                fi;
            od;
        :: else -> skip;
        fi;
    ::  timeout -> break;
    od;
}


// representation of unreliable channel
proctype Daemon() {
    do
    :: s2r ? _(_,_)
    :: r2s ? _(_)
    od
}


init {
    run S();
    run R();
    run Daemon();
}

/*

tkf-mbp:assignment02 takafumih$ spin -search -a abp_iv.pml
pan:1: too many fields in recv (at depth 5)
pan: wrote abp_iv.pml.trail

(Spin Version 6.4.8 -- 2 March 2018)
Warning: Search not completed
    + Partial Order Reduction

Full statespace search for:
    never claim             - (none specified)
    assertion violations    +
    acceptance   cycles     + (fairness disabled)
    invalid end states  +

State-vector 52 byte, depth reached 5, errors: 1
        5 states, stored
        0 states, matched
        5 transitions (= stored+matched)
        0 atomic steps
hash conflicts:         0 (resolved)

Stats on memory usage (in Megabytes):
    0.000   equivalent memory usage for states (stored*(State-vector + overhead))
    0.274   actual memory usage for states
  128.000   memory used for hash table (-w24)
    0.534   memory used for DFS stack (-m10000)
  128.730   total actual memory usage



pan: elapsed time 0 seconds

*/