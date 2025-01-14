#!/usr/bin/env python2

import sys
import struct
import time
import datetime
import binascii


# You can use this method to exit on failure conditions.
def bork(msg):
    sys.exit(msg)


# Some constants. You shouldn't need to change these.
MAGIC = 0x8BADF00D
VERSION = 1

if len(sys.argv) < 2:
    sys.exit("Usage: python stub.py input_file.fpff")

# Normally we'd parse a stream to save memory, but the FPFF files in this
# assignment are relatively small.
with open(sys.argv[1], 'rb') as fpff:
    data = fpff.read()

# Hint: struct.unpack will be VERY useful.
# Hint: you might find it easier to use an index/offset variable than
# hardcoding ranges like 0:8
magic, version = struct.unpack("<LL", data[0:8])
bin_timestamp = struct.unpack("<L", data[8:12])
bin_author = struct.unpack("<Q", data[12:20])
sectioncount = struct.unpack("<L", data[20:24])

timestamp = datetime.datetime.fromtimestamp(bin_timestamp[0])
author = binascii.b2a_uu(str(bin_author[0])).decode('ascii')



if magic != MAGIC:
    bork("Bad magic! Got %s, expected %s" % (hex(magic), hex(MAGIC)))

if version != VERSION:
    bork("Bad version! Got %d, expected %d" % (int(version), int(VERSION)))

print("------- HEADER -------")
print("MAGIC: %s" % hex(magic))
print("VERSION: %d" % int(version))
print("TIMESTAMP: %s" % (timestamp))
print("AUTHOR: %s" % (author))
print("SECTION COUNT: %d" % (sectioncount))

# We've parsed the magic and version out for you, but you're responsible for
# the rest of the header and the actual FPFF body. Good luck!

print("-------  BODY  -------")


