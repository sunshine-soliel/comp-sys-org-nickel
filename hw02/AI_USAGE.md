# AI Usage

## Tool
Claude (claude.ai)

## How I used it
- Claude walked me through the homework one step at a time: unpacking
  the starter into my repo, building and running it, then each module.
- For each module (stats, reading, histogram, main), Claude gave me the
  code with a short explanation of why it was written that way. I
  entered it, compiled each file on its own, and ran regress.sh after
  main was done to check the output still matched.
- Claude wrote tests/test_stats.c and the Makefile, then
  I ran make, make test, and make clean to check them.

## Something the AI got wrong or I had to fix
- Claude's first instructions told me to cd into "path/to/comp-sys-org-nickel"
  and unzip from ~/Downloads. Those didn't work since the path was a
  placeholder, and I'm using WSL, so my Windows Downloads folder is at
  /mnt/c/Users/nicke/Downloads, not ~/Downloads. I found it from the
  error messages and the Windows path of the zip.