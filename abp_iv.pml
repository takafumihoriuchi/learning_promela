// S ->   message   -> R
chan s2r = [0] of {byte, bit};

// S <- acknowledge <- R
chan r2s = [0] of {bit};


// sends three kinds of messages (0,1,2)
// in specific sequence
proctype S() {
    
    byte message;
    bit sending_bit = 0;
    bit recieved_bit;

    // m0 (zero or more but not infinity)
    message = 0;
    do
    ::  do // 一回送ろうとしたものは、責任を持ってちゃんと送れるまで送り続ける
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
    
    // m1
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

    // m0 (zero or more but not infinity)
    do
    ::  do // 一回送ろうとしたものは、責任を持ってちゃんと送れるまで送り続ける
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
    
    // m2
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
    
    // m0 (more than once, possibly infinity)
    message = 0;
    do
    ::  do // 一回送ろうとしたものは、責任を持ってちゃんと送れるまで送り続ける
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


// run by: spin -search -a abp_iv.c
// when recieve specific sequence
// passes an accept label infinitely many times
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
                        :: (message == 0) -> // sequence accepted (one m0 at tail)
                            printf("accepted\n");
accept_infinite_zero_tail:  do // checks for infinite loops
                            :: s2r ? message, recieved_bit -> // sequence accepted (two or more m0 at tail)
                                if
                                :: (recieved_bit == expected_bit) ->
                                    expected_bit = 1 - expected_bit;
                                :: else -> skip;
                                fi;
                                r2s ! recieved_bit;
                                if
                                :: (message == 0) -> printf("continuous zero at tail\n"); // skip;
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
