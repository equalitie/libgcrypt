/* testapi.c - for libgcrypt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>


#define BUG() do {fprintf ( stderr, "Ooops at %s:%d\n", __FILE__ , __LINE__ );\
		  exit(2);} while(0)

/* an ElGamal public key */
struct {
    const char *p,*g,*y;
} elg_testkey1 = {
  "0x9D559F31A6D30492C383213844AEBB7772963A85D3239F3611AAB93A2A985F64FB735B9259EC326BF5720F909980D609D37C288C9223B0350FBE493C3B5AF54CA23031E952E92F8A3DBEDBC5A684993D452CD54F85B85160166FCD25BD7AB6AE9B1EB4FCC9D300DAFF081C4CBA6694906D3E3FF18196A5CCF7F0A6182962166B",
  "0x5",
  "0x9640024BB2A277205813FF685048AA27E2B192B667163E7C59E381E27003D044C700C531CE8FD4AA781B463BC9FFE74956AF09A38A098322B1CF72FC896F009E3A6BFF053D3B1D1E1994BF9CC07FA12963D782F027B51511DDE8C5F43421FBC12734A9C070F158C729A370BEE5FC51A772219438EDA8202C35FA3F5D8CD1997B"
};

void
test_sexp ( int argc, char **argv )
{
    int rc, nbits;
    GCRY_SEXP sexp;
    GCRY_MPI key[3];

    if ( gcry_mpi_scan( &key[0], GCRYMPI_FMT_HEX, elg_testkey1.p, NULL ) )
	BUG();
    if ( gcry_mpi_scan( &key[1], GCRYMPI_FMT_HEX, elg_testkey1.g, NULL ) )
	BUG();
    if ( gcry_mpi_scan( &key[2], GCRYMPI_FMT_HEX, elg_testkey1.y, NULL ) )
	BUG();

    /* get nbits from a key */
    rc = gcry_sexp_build ( &sexp, NULL,
			   "(public-key(elg(p%m)(g%m)(y%m)))",
				  key[0], key[1], key[2] );
    if (rc) {
	fprintf (stderr, "gcry_sexp_build failed: rc=%d\n", rc );
	return;
    }
    fputs ( "DUMP of PK:\n", stderr );
    gcry_sexp_dump ( sexp );
    {  GCRY_SEXP x;
       x = gcry_sexp_cdr ( sexp );
       fputs ( "DUMP of CDR:\n", stderr );
       gcry_sexp_dump ( x );
       gcry_sexp_release ( x );
    }
    nbits = gcry_pk_get_nbits( sexp );
    printf ( "elg_testkey1 - nbits=%d\n", nbits );
    gcry_sexp_release( sexp );
}


int
main( int argc, char **argv )
{
    if ( argc < 2 )
	printf("%s\n", gcry_check_version ( NULL ) );
    else if ( !strcmp ( argv[1], "version") )
	printf("%s\n", gcry_check_version ( argc > 2 ? argv[2] : NULL ) );
    else if ( !strcmp ( argv[1], "sexp" ) )
	test_sexp ( argc-2, argv+2 );
    else {
	fprintf (stderr, "usage: testapi mode-string [mode-args]\n");
	return 1;
    }

    return 0;
}
