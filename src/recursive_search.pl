pertence(X, [X|_]).

pertence(X, [_|Y]) :-
    pertence(X,Y).

pertence_deterministico(Elem, [Elem|_]) :- !.
pertence_deterministico(Elem, [_|Cauda]) :-
    pertence_deterministico(Elem, Cauda).

