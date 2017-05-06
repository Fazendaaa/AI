pertence(X, [X|_]).
pertence(X, [_|Y]) :-
    pertence(X,Y).

array(X, Y, [X|Y]).

/*  */
sum([], 0).
sum([H|T], S) :-
    sum(T, S1),
    S is S1 + H.

/*  troca um número X por um número que é 2*X em uma lista  */

mult_x(X, [H|T], A) :-
    X == H,
    M is 2*X,
    array(M, T, A).

mult_x(X, [H|T], [H|B]) :-
    mult_x(X, T, B).

mult_x_array(X, [H|T], A) :-
    pertence(X, [H|T]),
    mult_x(X, [H|T], A).

/*  remove a primeira ocorrência de um elemento de uma lista    */

remove_first(_, [H], H).
remove_first(H, [H|T], T).
remove_first(X, [H|T], [H|A]) :-
    remove_first(X, T, A).

/*  remove todas as ocorrências de um elemento de uma lista */

remove_all(_, [], []).
remove_all(H, [H|T], A) :-
    remove_all(H, T, A).
remove_all(X, [H|T], [H|A]) :-
    remove_all(X, T, A).

/*  insere um elemento no fim da lista  */

last(X, [H], [H|X]).
last(X, [H|T], A) :-
    last(X, T, B),
    array(H, B, A).

/*  inverte a ordem dos elementos de uma lista  */

reverse([H], H).
reverse([H|T], A) :-
    reverse(T, B),
    array(B, H, A).

/*  n-ésimo elemento    */

n_element(0, [], []).
n_element(0, [H|_], H).
n_element(N, [H|T], A) :-
    P is N-1,
    n_element(P, T, A).

/*  ordenar os elementos de uma lista   */

qsort([], []).
qsort([H|T], A) :-
    partition
    