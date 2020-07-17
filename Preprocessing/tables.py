import sys
from ctypes import *

#valuesForRotors = [ [ 0x0b, 0x07, 0x05, 0x0e, 0x0f, 0x06, 0x00, 0x0d, 0x02, 0x04, 0x0c, 0x0a, 0x09, 0x01, 0x08, 0x03, ],
#                    [ 0x00, 0x0b, 0x01, 0x09, 0x07, 0x08, 0x0c, 0x0f, 0x04, 0x06, 0x0d, 0x05, 0x03, 0x0a, 0x0e, 0x02, ],
#                    [ 0x09, 0x06, 0x0c, 0x00, 0x01, 0x0f, 0x0e, 0x04, 0x0b, 0x0a, 0x05, 0x08, 0x03, 0x0d, 0x02, 0x07, ],
#                    [ 0x09, 0x02, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x0d, 0x0a, 0x0e, 0x0b, 0x05, 0x08, 0x0c, 0x04, 0x06, ],
#                    [ 0x01, 0x0a, 0x06, 0x02, 0x0d, 0x0e, 0x09, 0x00, 0x03, 0x0c, 0x05, 0x04, 0x0b, 0x08, 0x07, 0x0f, ],
#                    [ 0x07, 0x05, 0x0b, 0x0a, 0x02, 0x0f, 0x04, 0x09, 0x01, 0x06, 0x03, 0x0d, 0x0c, 0x08, 0x00, 0x0e, ],
#                    [ 0x0a, 0x03, 0x0c, 0x08, 0x0d, 0x04, 0x0b, 0x05, 0x07, 0x06, 0x00, 0x0e, 0x01, 0x02, 0x09, 0x0f, ],
#                    [ 0x0f, 0x04, 0x0d, 0x00, 0x05, 0x0c, 0x06, 0x0a, 0x02, 0x08, 0x07, 0x01, 0x0b, 0x09, 0x03, 0x0e, ],
#                    [ 0x07, 0x04, 0x09, 0x0a, 0x0b, 0x0f, 0x03, 0x05, 0x02, 0x0c, 0x0e, 0x0d, 0x06, 0x00, 0x01, 0x08, ],
#                    [ 0x06, 0x0f, 0x02, 0x08, 0x0d, 0x03, 0x0c, 0x0b, 0x05, 0x04, 0x0e, 0x09, 0x00, 0x0a, 0x01, 0x07, ],
#                    [ 0x00, 0x07, 0x04, 0x03, 0x02, 0x0a, 0x0c, 0x09, 0x06, 0x01, 0x0d, 0x08, 0x0b, 0x0f, 0x05, 0x0e, ],
#                    [ 0x0b, 0x08, 0x0f, 0x09, 0x06, 0x0c, 0x03, 0x0e, 0x00, 0x04, 0x05, 0x01, 0x0d, 0x07, 0x0a, 0x02, ],
#                    [ 0x08, 0x05, 0x0d, 0x06, 0x0a, 0x01, 0x0f, 0x0e, 0x03, 0x09, 0x04, 0x07, 0x0b, 0x02, 0x0c, 0x00, ],
#                    [ 0x0b, 0x0f, 0x09, 0x05, 0x0d, 0x04, 0x0c, 0x01, 0x03, 0x00, 0x06, 0x0e, 0x0a, 0x02, 0x07, 0x08, ],
#                    [ 0x0b, 0x02, 0x00, 0x0c, 0x09, 0x0e, 0x05, 0x06, 0x0a, 0x01, 0x0f, 0x03, 0x08, 0x04, 0x0d, 0x07, ],
#                    [ 0x06, 0x0b, 0x03, 0x09, 0x02, 0x0e, 0x0f, 0x00, 0x0d, 0x08, 0x01, 0x0a, 0x0c, 0x05, 0x04, 0x07, ] ]


#                  mov  and   or    call  add   sub   xor   mul   div   push   pop    
staticOpcodes = [[ 0x0b, 0x6a, 0xb6, 0x92, 0xf0, 0x3f, 0x82, 0x34, 0x32, 0x08, 0xdf, ],
                 [ 0x3a, 0x7a, 0x2c, 0x9c, 0x17, 0xa9, 0xa8, 0x6a, 0x9b, 0xe5, 0xdc, ],
                 [ 0xae, 0xc4, 0x09, 0x18, 0xe3, 0x7d, 0x85, 0xf8, 0x8d, 0x0f, 0x3d, ],
                 [ 0x10, 0x24, 0xc3, 0xb6, 0x53, 0xe0, 0x46, 0x4f, 0xd8, 0x00, 0xfc, ],
                 [ 0xcd, 0x0a, 0xcb, 0x14, 0xa1, 0x27, 0x38, 0x32, 0x17, 0x98, 0xff, ],
                 [ 0xf9, 0x02, 0x3a, 0x77, 0x31, 0x5d, 0x55, 0xce, 0xe7, 0xbb, 0x8d, ],
                 [ 0xb4, 0xe9, 0x24, 0xe5, 0xcb, 0xd6, 0x35, 0xfd, 0xd4, 0x58, 0xbd, ],
                 [ 0x3c, 0x16, 0x52, 0x14, 0xea, 0x71, 0x01, 0x12, 0x17, 0xcb, 0x6f, ],
                 [ 0x61, 0xdf, 0x86, 0x3c, 0x11, 0x1e, 0x81, 0x5e, 0x90, 0xc9, 0xb1, ],
                 [ 0xd3, 0x5c, 0x6d, 0x1c, 0x42, 0x97, 0x03, 0x8f, 0x58, 0x01, 0xf9, ],
                 [ 0xf1, 0xe3, 0x65, 0x4e, 0x15, 0xd5, 0x7f, 0xbd, 0xc6, 0xb8, 0x03, ],
                 [ 0x2e, 0xe4, 0xa7, 0xcc, 0xfe, 0x32, 0x1d, 0x39, 0xe6, 0x9d, 0x02, ],
                 [ 0x33, 0xa6, 0x87, 0x6a, 0x56, 0x53, 0x9c, 0xe5, 0x51, 0x16, 0x3f, ],
                 [ 0x51, 0xd1, 0x4a, 0x0b, 0xa0, 0x30, 0x79, 0xf5, 0x0c, 0xde, 0xd0, ],
                 [ 0xfa, 0xae, 0xaf, 0x53, 0x56, 0x2c, 0xa5, 0xe2, 0xf2, 0x7f, 0x16, ],]

#                     reg     constaddr     const   regaddr         
staticOpTypes = [   [ 0x02,     0x0c,       0x03,   0x00, ],
                    [ 0x0a,     0x01,       0x0f,   0x0c, ],
                    [ 0x07,     0x06,       0x08,   0x0e, ],
                    [ 0x0c,     0x0f,       0x03,   0x07, ],
                    [ 0x01,     0x08,       0x03,   0x09, ],
                    [ 0x08,     0x0e,       0x0b,   0x05, ],
                    [ 0x0f,     0x00,       0x08,   0x0d, ],
                    [ 0x0a,     0x0d,       0x0b,   0x0f, ],
                    [ 0x0d,     0x0f,       0x0e,   0x08, ],
                    [ 0x05,     0x06,       0x0a,   0x0e, ],
                    [ 0x07,     0x0a,       0x0f,   0x0e, ],
                    [ 0x01,     0x05,       0x0f,   0x0e, ],
                    [ 0x04,     0x0a,       0x01,   0x07, ],
                    [ 0x03,     0x05,       0x09,   0x00, ],
                    [ 0x08,     0x0d,       0x06,   0x0c, ],]

valuesForRotors = (POINTER(c_ubyte) * 16)((c_ubyte * 16)( 0x0b, 0x07, 0x05, 0x0e, 0x0f, 0x06, 0x00, 0x0d, 0x02, 0x04, 0x0c, 0x0a, 0x09, 0x01, 0x08, 0x03 ),
                                          (c_ubyte * 16)( 0x00, 0x0b, 0x01, 0x09, 0x07, 0x08, 0x0c, 0x0f, 0x04, 0x06, 0x0d, 0x05, 0x03, 0x0a, 0x0e, 0x02 ),
                                          (c_ubyte * 16)( 0x09, 0x06, 0x0c, 0x00, 0x01, 0x0f, 0x0e, 0x04, 0x0b, 0x0a, 0x05, 0x08, 0x03, 0x0d, 0x02, 0x07 ),
                                          (c_ubyte * 16)( 0x09, 0x02, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x0d, 0x0a, 0x0e, 0x0b, 0x05, 0x08, 0x0c, 0x04, 0x06 ),
                                          (c_ubyte * 16)( 0x01, 0x0a, 0x06, 0x02, 0x0d, 0x0e, 0x09, 0x00, 0x03, 0x0c, 0x05, 0x04, 0x0b, 0x08, 0x07, 0x0f ),
                                          (c_ubyte * 16)( 0x07, 0x05, 0x0b, 0x0a, 0x02, 0x0f, 0x04, 0x09, 0x01, 0x06, 0x03, 0x0d, 0x0c, 0x08, 0x00, 0x0e ),
                                          (c_ubyte * 16)( 0x0a, 0x03, 0x0c, 0x08, 0x0d, 0x04, 0x0b, 0x05, 0x07, 0x06, 0x00, 0x0e, 0x01, 0x02, 0x09, 0x0f ),
                                          (c_ubyte * 16)( 0x0f, 0x04, 0x0d, 0x00, 0x05, 0x0c, 0x06, 0x0a, 0x02, 0x08, 0x07, 0x01, 0x0b, 0x09, 0x03, 0x0e ),
                                          (c_ubyte * 16)( 0x07, 0x04, 0x09, 0x0a, 0x0b, 0x0f, 0x03, 0x05, 0x02, 0x0c, 0x0e, 0x0d, 0x06, 0x00, 0x01, 0x08 ),
                                          (c_ubyte * 16)( 0x06, 0x0f, 0x02, 0x08, 0x0d, 0x03, 0x0c, 0x0b, 0x05, 0x04, 0x0e, 0x09, 0x00, 0x0a, 0x01, 0x07 ),
                                          (c_ubyte * 16)( 0x00, 0x07, 0x04, 0x03, 0x02, 0x0a, 0x0c, 0x09, 0x06, 0x01, 0x0d, 0x08, 0x0b, 0x0f, 0x05, 0x0e ),
                                          (c_ubyte * 16)( 0x0b, 0x08, 0x0f, 0x09, 0x06, 0x0c, 0x03, 0x0e, 0x00, 0x04, 0x05, 0x01, 0x0d, 0x07, 0x0a, 0x02 ),
                                          (c_ubyte * 16)( 0x08, 0x05, 0x0d, 0x06, 0x0a, 0x01, 0x0f, 0x0e, 0x03, 0x09, 0x04, 0x07, 0x0b, 0x02, 0x0c, 0x00 ),
                                          (c_ubyte * 16)( 0x0b, 0x0f, 0x09, 0x05, 0x0d, 0x04, 0x0c, 0x01, 0x03, 0x00, 0x06, 0x0e, 0x0a, 0x02, 0x07, 0x08 ),
                                          (c_ubyte * 16)( 0x0b, 0x02, 0x00, 0x0c, 0x09, 0x0e, 0x05, 0x06, 0x0a, 0x01, 0x0f, 0x03, 0x08, 0x04, 0x0d, 0x07 ),
                                          (c_ubyte * 16)( 0x06, 0x0b, 0x03, 0x09, 0x02, 0x0e, 0x0f, 0x00, 0x0d, 0x08, 0x01, 0x0a, 0x0c, 0x05, 0x04, 0x07 ) )