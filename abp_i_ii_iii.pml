chan s2r = [0] of {byte};

// sends three kinds of messages (0,1,2)
// in specific sequence
proctype S() {
    byte message;
    // m0 (zero or more but not infinity)
    message = 0;
    do
    ::  s2r ! message;
    ::  break;
    od;
    // m1
    message = 1;
    s2r ! message;
    // m0 (zero or more but not infinity)
    message = 0;
    do
    ::  s2r ! message;
    ::  break;
    od;
    // m2
    message = 2;
    s2r ! message;
    // m0 (more than once, possibly infinity)
    message = 0;
    do
    ::  s2r ! message;
    od;
}

// run by: spin -search -a abp_i_ii_iii.c
// when recieve specific sequence
// passes an accept label infinitely many times
proctype R() {
    byte message;
L1: do
    :: s2r ? message ->
        if
        :: (message == 1) ->
            do
            :: s2r ? message ->
                if
                :: (message == 2) ->
                    do
                    :: s2r ? message ->
                        if
                        :: (message == 0) -> // sequence accepted (one m0 at tail)
accept_infinite_zero_tail:  do // checks for infinite loops
                            :: s2r ? message -> // sequence accepted (two or more m0 at tail)
                                if
                                :: (message == 0) -> skip;
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

init {
    run S();
    run R();
}
