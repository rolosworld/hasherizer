/*
 Copyright (c) 2017 Rolando González Chévere <rolosworld@gmail.com>

 This file is part of hasherizer.

 hasherizer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 hasherizer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with hasherizer.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void bits( unsigned long data ) {
  for ( int i = 0; i < sizeof( data ) * 8; i++ ) {
    printf( "%d", data & 1 ? 1 : 0 );
    if ( !((i+1) % 8) ) {
      printf( " " );
    }
    data = data>>1;
  }

  printf( "\n" );
}

unsigned long uniqid( unsigned char version, unsigned char creator, unsigned char count ) {
  int version_bits = sizeof(version) * 8;
  int machine_bits = sizeof(creator) * 8;
  int count_bits = sizeof(count) * 8;
  struct timeval tv;

  gettimeofday(&tv, NULL);

  unsigned long stamp =
    (unsigned long)(tv.tv_sec) * 1000 +
    (unsigned long)(tv.tv_usec) / 1000;
  
  int max_bits = sizeof(stamp) * 8;

  unsigned long _version = version;
  _version = _version << (max_bits - version_bits); // Move value to the last byte

  unsigned long _creator = creator;
  _creator = _creator << (max_bits - (version_bits + machine_bits)); // Move value to the 2nd to last byte

  unsigned long _count = count;
  _count = _count << (max_bits - (version_bits + machine_bits + count_bits)); // Move value to the 3rd to last byte

  stamp = stamp << (version_bits + machine_bits + count_bits);
  stamp = stamp >> (version_bits + machine_bits + count_bits);
  stamp = _version | _creator | _count | stamp;

  return stamp;
}

// unsorted map to make it less predictable, a-z,A-Z,0-9
char map[] = "H30kcYBFGVgN6IRqWu45yJdrzAfDnKXEU7eaCp9hOvmobSl1ZLMQiTsj8t2Pxw";
unsigned char* hasherizer( unsigned long id ) {
  unsigned char* uniq_hash = (unsigned char*) malloc( 21 * sizeof( unsigned char ) ); // unsigned long in 64bit has 20 digits max
  unsigned short index = 0;
  unsigned short map_index = 0;
  unsigned char map_size = strlen( map );

  while ( id ) {
    map_index = id % map_size;
    id = id / map_size;
    uniq_hash[index++] = map[map_index];
  }

  uniq_hash[index] = '\0';
  return uniq_hash;
}


int main( int argc, char *argv[] ) {
  unsigned char* uniq_hash;
  unsigned char i = -1;
  while (i--) {
    uniq_hash = hasherizer( uniqid( 1, 1, i ) );
    printf( "%s\n", uniq_hash );
    free( uniq_hash );
  }
  return 0;
}
