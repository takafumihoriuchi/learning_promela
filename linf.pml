bool p = false;

// linf
// ltl linf { []<>p && []<>!p }

// l1
ltl lone { <>p && (!p U [](p || (p U [](!p)))) }

active proctype minf() {
    do
    :: p = !p;
    od;
}