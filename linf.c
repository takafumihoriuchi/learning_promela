/*

// Linf: 現在から未来にかけて、Pが真である時間と偽である時間が永遠に繰り返される
bool p;
bool init_comp = 0;

// ltl linf { []<>p && []<>!p }
ltl linf { (p -> <>!p) && (!p -> <>p)}

// decides whether starting from "P holds" or "P does not hold"
proctype nd_init() {
    do
    :: p = 0; break;
    :: p = 1; break;
    od;
    init_comp = 1;
}

proctype minf() {
    do
    :: p = 1;
    :: p = 0;
    od;
}

init {
    run nd_init();
    (init_comp == 1)
    run minf();

    do
    :: printf("p: %d\n", p);
    od;
}

*/

/*
bool p = false;

// linf
ltl linf { []<>p && []<>!p }
// ltl linf { (p -> <>!p) && (!p -> <>p)}

// l1
// ltl lone { <>p && (!p U [](p || (p U [](!p)))) }

active proctype minf() {
    do
    :: p = !p;
    od;
}
*/


bool p = false;

// linf
ltl linf { []<>p && []<>!p }

// l1
// ltl lone { <>p && (!p U [](p || (p U [](!p)))) }

active proctype minf() {
    do
    :: p = !p;
    od;
}