#!/usr/bin/env python3
from __future__ import (absolute_import, division, print_function,
                        unicode_literals)
import argparse
from os.path import basename, exists, isdir
import glob
from bp4dbg_data import DumpData
from bp4dbg_idxtable import DumpIndexTable
from bp4dbg_metadata import DumpMetaData


def SetupArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--infile", "-i", help="Name of the input file", required=True)
    # parser.add_argument("--printdata", "-p",
    #                    help="Dump data of this variable as well", default="")
    parser.add_argument("--verbose", "-v",
                        help="More verbosity", action="store_true")
    parser.add_argument("--no-indextable", "-x",
                        help="Do not print index table md.idx",
                        action="store_true")
    parser.add_argument("--no-metadata", "-m",
                        help="Do not print metadata md.0", action="store_true")
    parser.add_argument("--no-data", "-d",
                        help="Do not print data data.*", action="store_true")
    args = parser.parse_args()

    # default values
    args.idxFileName = ""
    args.dumpIdx = False
    args.metadataFileName = ""
    args.dumpMetadata = False
    args.dataFileName = ""
    args.dumpData = False

    return args


def CheckFileName(args):
    if (not exists(args.infile)):
        print("ERROR: File " + args.infile + " does not exist", flush=True)
        exit(1)
    if (isdir(args.infile)):
        if (not args.no_indextable):
            args.idxFileName = args.infile + "/" + "md.idx"
            args.dumpIdx = True
        if (not args.no_metadata):
            args.metadataFileName = args.infile + "/" + "md.[0-9]*"
            args.dumpMetadata = True
        if (not args.no_data):
            args.dataFileName = args.infile + "/" + "data.[0-9]*"
            args.dumpData = True
        return

    name = basename(args.infile)
    if (name.startswith("data.")):
        args.dataFileName = args.infile
        args.dumpData = True

    elif (name == "md.idx"):
        args.idxFileName = args.infile
        args.dumpIdx = True

    elif (name.startswith("md.")):
        args.metadataFileName = args.infile
        args.dumpMetadata = True


def DumpIndexTableFile(args):
    indexFileList = glob.glob(args.idxFileName)
    if len(indexFileList) > 0:
        DumpIndexTable(indexFileList[0])
    else:
        print("There is  no BP4 Index Table file as " + args.idxFileName)


def DumpMetadataFiles(args):
    mdFileList = glob.glob(args.metadataFileName)
    if len(mdFileList) > 0:
        for fname in mdFileList:
            DumpMetaData(fname)
    else:
        print("There are no BP4 Metadata files in   " + args.metadataFileName)


def DumpDataFiles(args):
    dataFileList = glob.glob(args.dataFileName)
    if len(dataFileList) > 0:
        for fname in dataFileList:
            DumpData(fname)
    else:
        print("There are no BP4 Data files in       " + args.dataFileName)


if __name__ == "__main__":

    args = SetupArgs()
    CheckFileName(args)
    # print(args)

    if (args.dumpIdx):
        DumpIndexTableFile(args)

    if (args.dumpMetadata):
        DumpMetadataFiles(args)

    if (args.dumpData):
        DumpDataFiles(args)
