/*
// L1: 現在から未来にかけて、Pが真となる時間のまとまりがただ一度だけある
// （検証が開始するより前にPがどのような状態であったかは、気にしない。）

bool P, past, now;

// Pが真か偽どちらで始まるかは（こちらには）わからない
// なので、どちらの場合も非決定的に検証する
proctype nd_init() {
    do
    :: P = 0; past = 0; now = 0;
       break;
    :: P = 1; past = 1; now = 1;
       break;
    od;
}

// a promela model that satisfies L_1 but not L_inf
proctype l1() {
    if
    :: (past == 0 && now == 0) ->
    :: (past == 1 && )
    fi;
}

init {
    run nd_init();
    run l1();
}
*/




/*
// pretty good model, before writing ltl

bool P;

// decides whether starting from "P holds" or "P does not hold"
proctype nd_init() {
    do
    :: P = 0; break;
    :: P = 1; break;
    od;
}

proctype mone() {
    if
    :: (P == 0) -> do
                   :: P = 0;
                   :: P = 1; break;
                   od;
    fi;

    do
    :: P = 1;
    :: break;
    od;

    do
    :: P = 0;
    od;
}

init {
    run nd_init();
    run mone();
    
    do
    :: printf("P: %d\n", P);
    od;
}

*/

/*

// ltl lone { <>[](count == 1) && ((count == 1) -> <>[](!P))}

bool P = -1; // set to any value other than 1 (hold) or 0 (don't hold)
int count = 0; // counts rising edges
// (P holding initially will be counted as one)
bool init_comp = 0;

// always call this process BEFORE command "P = 1;"
proctype check_rise() {
    if
    :: (P != 1) -> count++;
    fi;
}

// decides whether starting from "P holds" or "P does not hold"
proctype nd_init() {
    do
    :: P = 0; break;
    :: run check_rise(); P = 1; break;
    od;
    init_comp = 1;
}

proctype mone() {
    if
    :: (P == 0) ->
        do
        :: P = 0;
        :: run check_rise(); P = 1; break;
        od;
    fi;

    do
    :: run check_rise(); P = 1;
    :: break;
    od;

    do
    :: P = 0;
    od;
}

init {
    run nd_init();
    (init_comp == 1);
    run mone();

    do
    :: printf("P: %d, count: %d\n", P, count);
    od;
}

*/

/*

#define XONE x==1
ltl lone { <>XONE && (!XONE U [](XONE || (XONE U []!XONE))) }

// always put this macro before command "P = 1"
#define CHECK_RISE if :: (P != 1) -> count++; fi;

#define P_HOLD P == 1
#define COUNT_IS_ONE count == 1

ltl lone { <>[](COUNT_IS_ONE) && ((COUNT_IS_ONE) -> <>[](!(P_HOLD)))}

int P = -1; // set to any value other than 1 (hold) or 0 (don't hold)
int count = 0; // counts rising edges
// (P holding initially will be counted as one)
bool init_comp = 0;

// decides whether starting from "P holds" or "P does not hold"
proctype nd_init() {
    do
    :: P = 0; break;
    :: CHECK_RISE; P = 1; break;
    od;
    init_comp = 1;
}

proctype mone() {
    if
    :: (P == 0) ->
        do
        :: P = 0;
        :: CHECK_RISE; P = 1; break;
        od;
    fi;

    do
    :: CHECK_RISE; P = 1;
    :: break;
    od;

    do
    :: P = 0;
    od;
}

init {
    run nd_init();
    (init_comp == 1);
    run mone();

    // ここの位置でのprintfでは、Pの全ての変化を捉えているわけではないことに注意
    do
    :: printf("P: %d, count: %d\n", P, count);
    od;
}

*/

/*

bool p;

// l1
ltl lone { <>p && (!p U [](p || (p U [](!p)))) }

// linf
// ltl linf { []<>p && []<>!p }
// ltl linf { (p -> <>!p) && (!p -> <>p)}

bool initialized = false;
proctype nd_init() {
    do
    :: p = false; break;
    :: p = true; break;
    od;
    initialized = true;
}

proctype mone() {
    if
    :: (p == false) -> p = true;
    fi;

    do
    :: p = true;
    :: break;
    od;

    do
    :: p = false;
    od;
}

init {
    run nd_init();
    (initialized == true);
    run mone();
}

*/




bool p;

// l1
ltl lone { <>p && (!p U [](p || (p U [](!p)))) }

// linf
// ltl linf { []<>p && []<>!p }

bool initialized = false;
proctype nd_init() {
    do
    :: p = false; break;
    :: p = true; break;
    od;
    initialized = true;
}

proctype mone() {
    if
    :: (p == false) -> p = true;
    fi;

    do
    :: p = true;
    :: break;
    od;

    do
    :: p = false;
    od;
}

init {
    run nd_init();
    (initialized == true);
    run mone();
}
