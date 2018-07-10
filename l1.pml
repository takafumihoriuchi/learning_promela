bool p;

// l1
// ltl lone { <>p && (!p U [](p || (p U [](!p)))) }

// linf
ltl linf { []<>p && []<>!p }

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
