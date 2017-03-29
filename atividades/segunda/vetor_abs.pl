%   Para instalar no Linux basta procurar o pacote 'swi-prolog', e rodar no
%   terminal com o comando:
%
%                           swipl vetor_abs.pl
%
%   obs:  mesmo  que  a  extensão seja a mesma de Perl, recomendo não mudar para
%   .pro  para evitar conflito uma vez que na minha máquina o SWI não reconheçeu
%   tal  extensão  como  válida.  Além  disso,  para  matar  o  programa deve-se
%   pressionar Ctrl + D -- no Windows não sei como seria, provavelment Ctrl + Z.
%
%   Use  o  segundo  comando  na  sua interface SWI-Prolog para fazer o teste do
%   código:
%   
%                       vetor_abs( [ -1, 2, -3 ], R ).
%

%   Caso   seja   uma  chamada  do  tipo  inserir( 1, 2, V ),  irá  retornar  em
%   V = [ 1, 2 ].

inserir_vetor( X, R, [ X | R ] ).

%   Caso  de  parada  da  recursão. Como Prolog funciona dando match na primeira
%   chamada  de  descrição  de  função que encontra o caso de parada sempre será
%   verificado na chamada recursiva da próxima definição.

vetor_abs( [], [] ).

%   Segunda descrição da função
vetor_abs( [ X | Y ], Z ) :-
    %   Vale  aqui  ressaltar  que  toda  vez  que for chamada a função aqui ela
    %   verificará  a  primeira descrição antes de rodar a recursiva.
    vetor_abs( Y, W ),
    A is abs( X ),
    %   Irá  adicionar  o absoluto da cabeça da lista com a lista de retorno, W, 
    %   da  chamada  anterior.  Ou seja,  a  função  irá percorrer toda a lista,
    %   calcular o absoluto de sua cabeça -- quando for o último elmento, cabeça
    %   e  cauda são o mesmo e W = [] -- para então adiciar adiciná-los na lista
    %   de retorno Z.
    inserir_vetor( A, W, Z ).