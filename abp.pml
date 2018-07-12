#define MAX 100

mtype = {MSG, ACK};
chan s2r = [2] of {mtype, byte, bit};
chan r2s = [2] of {mtype, bit};


// original sender model
active proctype Sender() {
    
    byte msgid = 0;
    bit sending_b = 0;
    bit recieved_b;

    do
    :: s2r ! MSG(msgid, sending_b) ->
       if
       :: r2s ? ACK(recieved_b) ->
          if
          :: (recieved_b == sending_b) ->
              sending_b = 1 - sending_b;
              msgid = (msgid + 1) % MAX;
          :: else
          fi
       :: timeout // a predefined, global, read-only, boolean variable
       // true if no statement is executable in any active process, otherwise false
       // in this case, timeout が true になれば、何もしないで再送する
       fi
    od
}


// original reciever model
active proctype Receiver() {
    
    byte msgid;
    byte expected_msgid = 0;
    bit recieved_b;
    bit expected_b = 0;

    do
    :: s2r ? MSG(msgid, recieved_b) ->
       if
       :: (recieved_b == expected_b) ->
          expected_b = 1 - expected_b;
          expected_msgid = (expected_msgid + 1) % MAX;
       :: else
       fi;
       r2s ! ACK(recieved_b)
    od
}


/*
// representation of unreliable channel
active proctype Daemon() {
    do
    :: s2r ? _(_,_)
    :: r2s ? _(_)
    od
}
*/