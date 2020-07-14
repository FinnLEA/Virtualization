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