/*
#define MIN 2
#define MAX 15

int x = 0;
int count = 0;

proctype P() {
	int tmp;
	int k;
	for (k : 0 .. 4) {
		tmp = x;
		tmp++;
		x = tmp;
	}
	count++;
}

proctype check() {
	do
	:: count == 3 ->
		assert(x >= MIN && x <= MAX);
		break;
	od;
}

init {
	run P();
	run P();
	run P();
	run check();
}
*/

///////////////////////////////////

/*
int flg = 0;
int x = 10;
int y = 0;

proctype thread_1() {
	do
	:: (x != y) ->
		x = x - 1;
		y = y + 1;
	:: else ->
		flg++;
		break;
	od;
}

proctype thread_2() {
	do
	:: (x == y) ->
		atomic {
			x = 8;
			y = 2;
		}
		flg++;
		break;
	od;
}

proctype check() {
	do
	:: flg == 2 ->
		assert((x == 8 && y == 2) || (x == 5 && y == 5));
		break;
	od;
}

init {
	run thread_1();
	run thread_2();
	run check();
}
*/

///////////////////////////////////

mtype = {p, v};
chan S = [0] if {mtype};

int x = 0;
proctype check(int N) {
	assert(x >= 0 && x <= N);
}

proctype counting_semaphore(int N) {
	int count = N;
	do
	:: count > 0 ->
		S ? p;
		count--;
	:: count == 0 ->
		S ? v;
		count++;
	od;
}

proctype client() {
	do
	::	S ! p;
		x++;
		/* critical section */
		x--;
		S ! v;
	od;
}

init {
	int N = 3;
	run check(N);
	run counting_semaphore(N);
	run client();
	run client();
	run client();
	run client();
	run client();
}
