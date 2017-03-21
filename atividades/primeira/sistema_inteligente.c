/*  -------------------------------- IMPORTS -------------------------------  */

#include <ctype.h>
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

static char *string_lower( char *str ) {
    for( int i = 0; str[ i ]; i++ )
        str[ i ] = tolower( str[ i ] );

    return str;
}

static void free_person( Person **person ) {
    if( NULL != *person ) {
        free( person_name( *person ) );
        free( person_course( *person ) );
        free( *person );
        *person = NULL;
    }
}

static Person *new_person( char *name, char *course, int age ) {
    Person *new = NULL;

    if( NULL != ( new = malloc( sizeof( Person ) ) ) ) {
        if(  NULL != name && NULL != course ) {
            person_name( new ) = string_lower( name );
            person_course( new ) = string_lower( course );
            person_age( new ) = ( 0 < age ) ? age : 0;
        }

        else
            free_person( &new );
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

static Connection *new_connection( Person *person_one, Person *person_two,
                                   int friendship ) {
    Connection *new = NULL;

    if( NULL != ( new = malloc( sizeof( Connection ) ) ) ) {
        if( NULL != person_one && NULL != person_two ) {
            connection_first( new ) = person_one;
            connection_second( new ) = person_two;
            connection_friendship( new ) = ( 0 < friendship ) ? friendship : 0;
        }

        else
            free_connection( &new );
    }

    return new;
}

static bool is_friendship( Connection *connection ) {
    int pontuation = 0;

    if( NULL != connection && NULL != connection_first( connection )
                           && NULL != connection_second( connection ) ) {
        if( !strcmp( person_name( connection_first( connection ) ),
                     person_name( connection_second( connection ) ) ) ) {
            pontuation += 1;
        }

        if( person_age( connection_first( connection ) ) ==
            person_age( connection_second( connection ) ) ) {
            pontuation += 2;
        }

        if( !strcmp( person_course( connection_first( connection ) ),
                     person_course( connection_second( connection ) ) ) ) {
            pontuation += 3;
        }

        if( 6 < connection_friendship( connection ) ) {
            pontuation += 5;
        }

        /*  case they don't know each other but have the same name, same age and
            are  enrolled  in the same course they would have six points -- wich
            means  they  would be friends, thats why this penalty in two points;
            so the algorithm could work
        */
        else if( 0 == connection_friendship( connection ) ) {
            pontuation -= 2;
        }
    }

    return ( 4 < pontuation ) ? true : false;
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
            "nomes, idades, cursos e ha quanto tempo se conhecem.\n\n"
            "Digite os seguintes dados da primeira pessoa:\n"
            "\"primeiro nome,idade,sigla do curso\"\n");
    scanf( "%s[^,],%d[^,],%s", name_one, &age_one, course_one );
    person_one = new_person( name_one, course_one, age_one );

    printf( "\nDigite os seguintes dados da segunda pessoa:\n"
            "\"primeiro nome,idade,sigla do curso\"\n");
    scanf( "%s[^,],%d[^,],%s", name_two, &age_two, course_two );
    person_two = new_person( name_two, course_two, age_two );

    printf( "\nAgora digite ha quanto tempo, em anos, se conhecem:\n" );
    scanf( "%d", &friendship );
    connection = new_connection( person_one, person_two, friendship );

    printf( "\nEles sao amigos?\nR: %s\n",
          ( 1 == is_friendship( connection ) ) ? "sim" : "nao" );

    free_connection( &connection );

    return 0;
}
