mtype = { button, move, open, close };

#define NF	4	/* number of floors */
#define NP	2	/* number of passengers */

chan elv = [2] of { mtype, byte };
chan ctl = [2] of { mtype, byte };

show byte floor;		/* elevator position 0 to NF-1 */
show bool doors_open=true;	/* status of elevator doors */

// added codes for problem 3-2
byte w1_floor = 0;
byte w1_iam = 0;
bool w1_doors_open = false;
ltl no_hunger_w1 { []<>(w1_floor == w1_iam && w1_doors_open) }
// added codes for problem 3-2
byte w2_floor = 0;
byte w2_j = 0;
bool w2_doors_open = false;
ltl no_hunger_w2 { []<>(w2_floor == w2_j && w2_doors_open) }
//

active proctype controller()
{	byte j;
	bool must_visit[NF];

	do
	:: ctl?button(j) ->	/* a request to visit floor j */
		assert(j >= 0 && j < NF);
		must_visit[j] = true;
	
		j = 0;	/* check where to go */
		do
		:: must_visit[(floor+j)%NF] ->
			if
			:: j == 0 /* already there */
			:: else ->
				elv!close,0;
				(!doors_open);
				j = (floor+j)%NF;
				elv!move(j);
				(floor == j);
				elv!open,0;
				(doors_open)
			fi;
			must_visit[floor] = false;
			break /* inner loop */
		:: else ->
			if	/* check all floors */
			:: j < NF-1 -> j++
			:: else ->
				/* shows up in xspin MSCs */
				printf("MSC: nothing to do\n");
				break /* inner loop */
			fi
	od	od
}

active proctype elevator()
{
	do
	:: elv?move(floor) -> assert(doors_open == false)
	:: elv?close,_ -> doors_open = false
	:: elv?open,_  -> doors_open = true
	od
}

byte iam[NP];	/* current or desired floor for each passenger */

active [NP] proctype passenger()
{	byte j;

	iam[_pid%NP] = (_pid%NF);	/* place passengers at different floors */
	assert(_pid >= 2 && _pid < 2+NP);

	do
	:: ctl!button(iam[_pid%NP]) ->	/* push call button */

		// added codes for problem 3-2
		w1_floor = floor;
		w1_iam = iam[_pid%NP];
		w1_doors_open = doors_open;
		//
		
		/* wait to enter elevator */
W1:		(floor == iam[_pid%NP] && doors_open);
		printf("MSC: %d enters at floor %d\n", _pid, iam[_pid%NP]);
		j = 0;	/* choose a destination */
		do
		:: j < NF ->
			if
			:: break /* got to this floor */
			:: j++   /* or consider another */
			fi
		:: j >= NF ->	/* can't decide, go to floor 0 */
			j = 0;
			break
		od;
		ctl!button(j);	/* push destination button */

		// added codes for problem 3-2
		w2_floor = floor;
		w2_j = j;
		w2_doors_open = doors_open;
		//

		/* wait to exit elevator */
W2:		(floor == j && doors_open);
		iam[_pid%NP] = j;
		printf("MSC: %d exits at floor %d\n", _pid, iam[_pid%NP]);
	od
}
