#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*  ------------------------------ STRUCTURES ------------------------------  */

typedef struct person {
    char *name, *course;
    int age;
} Person;

typedef struct connection {
    Person *first, *second;
    //  for how long they know each other
    int friendship;
} Connection;

/*  -------------------------------- MACROS --------------------------------  */

#define STRING_SIZE 50

#define person_name( p ) ( ( p )->name )
#define person_course( p ) ( ( p )->course )
#define person_age( p ) ( ( p )->age )

#define connection_first( c ) ( ( c )->first )
#define connection_second( c ) ( ( c )->second )
#define connection_friendship( c ) ( ( c )->friendship )

/*  ------------------------------- LIBRARY --------------------------------  */

static Person *new_person( char *name, char *course, int age ) {
    Person *new = NULL;

    if( NULL != ( new = malloc( sizeof( Person ) ) ) ){
        person_name( new ) = name;
        person_course( new ) = course;
        person_age( new ) = age;
    }

    return new;
}

static void free_person( Person **person ) {
    if( NULL != *person ) {
        free( person_name( *person ) );
        free( person_course( *person ) );
        free( *person );
        *person = NULL;
    }
}

static Connection *new_connection( Person *person_one, Person *person_two,
                                   int friendship ) {
    Connection *new = NULL;

    if( NULL != ( new = malloc( sizeof( Connection ) ) ) ){
        connection_first( new ) = person_one;
        connection_second( new ) = person_two;
        connection_friendship( new ) = friendship;
    }

    return new;
}

static void free_connection( Connection **connection ) {
    if( NULL != *connection ) {
        free_person( &connection_first( *connection ) );
        free_person( &connection_second( *connection ) );
        free( *connection );
        *connection = NULL;
    }
}

static bool is_friendship( Connection *connection ) {
    return false;
}

/*  --------------------------------- MAIN ---------------------------------  */

int main( int argc, char **argv ) {
    Person *person_one = NULL, *person_two = NULL;
    Connection *connection = NULL;

    char *name_one = NULL, *name_two = NULL, *course_one = NULL,
         *course_two = NULL;
    int age_one = 0, age_two = 0, friendship = 0;

    name_one = malloc( sizeof( char ) * STRING_SIZE );
    name_two = malloc( sizeof( char ) * STRING_SIZE );
    course_one = malloc( sizeof( char ) * STRING_SIZE );
    course_two = malloc( sizeof( char ) * STRING_SIZE );

    printf( "\t\t\t\"Quem gosta de quem?\"\n\n"
            "O Programa compara a relacao entre duas pessoas e diz se elas sao "
            "ou nao amigas. Para isso sera necessario informar dados como o "
            "nomes, idades, cursos e ha quanto tempo se conhecem.\n\n" );
    printf( "Digite os seguintes dados da primeira pessoa:\n"
            "\"primeiro nome, idade, sigla do curso\"\n");
    scanf( "%s %d %s", name_one, &age_one, course_one );
    person_one = new_person( name_one, course_one, age_one );

    printf( "\nDigite os seguintes dados da segunda pessoa:\n"
            "\"primeiro nome, idade, sigla do curso\"\n");
    scanf( "%s %d %s", name_two, &age_two, course_two );
    person_two = new_person( name_two, course_two, age_two );

    printf( "\nAgora digite ha quanto tempo, em anos, se conhecem -- 0 caso nao"
            " se conhecam:\n");
    scanf( "%d", &friendship );
    connection = new_connection( person_one, person_two, friendship );

    printf( "Eles sao amigos?\nR: %s\n",
             ( 1 == is_friendship( connection ) ) ? "sim" : "nao" );

    free_connection( &connection );

    return 0;
}
