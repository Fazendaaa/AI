sequential(E1, E2, [E1, E2|_]).
sequential(E1, E2, [_|Tail]]) :-
    sequential(E1, E2, Tail).
