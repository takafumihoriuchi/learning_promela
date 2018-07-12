#define MAX 100

mtype = {MSG, ACK};
chan s2r = [2] of {mtype, byte, bit};
chan r2s = [2] of {mtype, bit};


active proctype Sender() {
    
    byte msgid = 0;
    bit b = 0;
    bit expected_b;

    do
    :: s2r ! MSG(msgid, b) ->
       if
       :: r2s ? ACK(expected_b) -> // 変数名について：正確には、'expected'ではなく、実際に受け取ったもの？
          if                       // 'expected'なのは、bに入っている値？
          :: (expected_b == b) ->
             b = 1 - b;
             msgid = (msgid + 1) % MAX;
          :: else
          fi
       :: timeout
       fi
    od
}


active proctype Receiver() {
    
    byte msgid;
    byte expected_msgid = 0;
    bit b;
    bit expected_b = 0;
    
    do
    :: s2r ? MSG(msgid, b) ->
       if
       :: (b == expected_b) ->
          expected_b = 1 - expected_b;
          expected_msgid = (expected_msgid + 1) % MAX;
       :: else
       fi;
       r2s ! ACK(b)
    od
}