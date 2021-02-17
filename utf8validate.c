#include <stdio.h>

int main(void)
{
	int byte ,codePoint ; 
	int ASCII = 0 ; // number of ASCII
	int multiByte = 0 ; // number of character that need 2 or more bytes

	while ((byte = getchar()) != EOF)
	{	
		if ((byte & 0x80) == 0x00) // |1-byte UTF-8| using mask  0x80 = 10000000 checking if the first bit from the first byte is equal to 0
		{
	    	codePoint = byte ;
        if (((codePoint < 0x00) || (codePoint > 0xD7FF)) && ((codePoint < 0XE000) || (codePoint > 0x10FFFF))) // checking if the code point is valid or not. Valid code points [U+0000,U+D7FF] and [U+E000,U+10FFFF]
        {
          printf ("Invalid UTF-8 code point: U+%04X\n" ,codePoint) ;
          return 1 ;
        }
	      ASCII++ ;
	 	}
    else if ((byte & 0xE0) == 0xC0) // |2-byte UTF-8| using mask 0xE0 = 11100000 checking if the first 3 bits from the first byte are equal to 0xC0 = 11000000
	  {
      codePoint = byte & 0x1F ; // using mask 0x1F = 11111 to take the last 5 bits from the byte  
      if ((byte = getchar()) == EOF) // geting the 2nd byte 
      {
        printf ("Unexpected EOF!\n") ;
        return 1;
      }
      if ((byte & 0xC0) != 0x80) // using mask 0xC0 = 11000000 checking if the first 2 bits from the 2nd byte are not equal to 0x80 = 10000000 
      {
        printf ("Invalid tail byte 0x%02X\n" ,byte ) ;
        return 1 ;
      }
      codePoint <<= 6 ; // codepoint has the last 5 bits from the byte and now we left shifting the codepoint 6 places
      codePoint += (byte & 0x3F) ; // adding to codepoint the last 6 bits from the byte by using using mask 0x3F = 111111
      if (((codePoint < 0x00) || (codePoint > 0xD7FF)) && ((codePoint < 0XE000) || (codePoint > 0x10FFFF))) // checking if the code point is valid or not. Valid code points [U+0000,U+D7FF] and [U+E000,U+10FFFF]
      {
        printf ("Invalid UTF-8 code point: U+%04X\n" ,codePoint) ;
        return 1 ;
      }
      multiByte++ ;
		}
    else if ((byte & 0xF0) == 0xE0) // |3-byte UTF-8| using mask 0xF0=11110000 checking if the fisrt 4 bits from the first byte are equal to 0xE0 = 11100000
		{
      codePoint = byte & 0x0F; // using mask 0x0F = 1111 to take the last 4 bits from the byte
      for (int i = 0 ; i < 2 ; i++) // taking the 2nd and 3d bytes 
      {
        if ((byte = getchar()) == EOF)
        {
          printf ("Unexpected EOF!\n") ;
          return 1 ;
        }
        if ((byte & 0xC0) != 0x80) // using mask 0xC0 = 11000000 checking if the first 2 bits from the 2nd byte are not equal to 0x80 = 10000000 and for the 3d byte the same
        {
          printf ("Invalid tail byte 0x%02X\n" ,byte ) ;
          return 1 ;
        }
        codePoint<<=6 ; // codepoint has the last 5 bits from the byte and now we left shifting the codepoint 6 places
        codePoint += (byte & 0x3F) ; // adding to codepoint the last 6 bits from the byte by using using mask 0x3F = 111111
        if (((codePoint < 0x00) || (codePoint > 0xD7FF)) && ((codePoint < 0XE000) || (codePoint > 0x10FFFF))) // checking if the code point is valid or not. Valid code points [U+0000,U+D7FF] and [U+E000,U+10FFFF]
        {
          printf ("Invalid UTF-8 code point: U+%04X\n" ,codePoint) ;
          return 1 ;
        }
        if ((codePoint >= 0xD800) && (codePoint <= 0xDFFF)) // checking if the code point is inside the invalid [U+D800,U+DFFF] 
        {
           printf ("Invalid UTF-8 code point: U+%04X\n" ,codePoint) ;
            return 1 ;
        }
      }
      multiByte++ ;
		}
    else  if ((byte & 0xF8) == 0xF0) // |4-byte UTF-8| using mask 0xF8 = 11111000 cheking if the first 5 bits from the fisrt byte are equal to 0xF0 = 11110000
		{
      codePoint = byte & 0x07 ; // using mask 0x07 = 111 to take the last 3 bits from the byte 
      for (int i = 0 ; i < 3 ; i++) // taking the 2nd ,3d and 4th bytes
      {
        if ((byte = getchar()) == EOF)
        {
          printf ("Unexpected EOF!\n") ;
          return 1 ;
        }
        if ((byte & 0xC0) != 0x80) // using mask 0xC0 = 11000000 checking if the first 2 bits from the 2nd byte are not equal to 0x80 = 10000000 and for the 3d and 4th byte the same
        {
          printf ("Invalid tail byte 0x%02X\n" ,byte ) ;
          return 1 ;
        }
        codePoint<<=6 ; // codepoint has the last 5 bits from the byte and now we left shifting the codepoint 6 places
        codePoint += (byte & 0x3F) ; // adding to codepoint the last 6 bits from the byte by using using mask 0x3F = 111111
        if (((codePoint < 0x00) || (codePoint > 0xD7FF)) && ((codePoint < 0XE000) || (codePoint > 0x10FFFF))) // checking if the code point is valid or not. Valid code points [U+0000,U+D7FF] and [U+E000,U+10FFFF]
        {
          printf ("Invalid UTF-8 code point: U+%04X\n" ,codePoint) ;
          return 1 ;
        }
      }
      multiByte++ ;
		}
    else // the header byte is invalid
    {
      printf ("Invalid UTF-8 header byte: 0x%02X\n" ,byte) ;
      return 1 ;
    }
	}
	printf ("Found %d ASCII and %d multi-byte UTF-8 characters.\n" ,ASCII ,multiByte) ;
	return 0 ;
}