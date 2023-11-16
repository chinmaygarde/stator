# Copyright 2013 The Flutter Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import sys
import os
import argparse
import subprocess
import errno

def make_directories(path):
  try:
    os.makedirs(path)
  except OSError as exc:
    if exc.errno == errno.EEXIST and os.path.isdir(path):
      pass
    else:
      raise

def main():
  parser = argparse.ArgumentParser()

  parser.add_argument(
    '--source',
    type=str,
    required=True,
    help="The location of the source file."
  )
  parser.add_argument(
    '--outdir',
    type=str,
    required=True,
    help="The directory to place the generated files in."
  )

  args = parser.parse_args()

  make_directories(args.outdir)

  command = [
    'flex',
    '--noline',
    os.path.abspath(args.source)
  ]

  subprocess.check_output(command, cwd=args.outdir)

if __name__ == '__main__':
  sys.exit(main())
