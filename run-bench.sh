#!/bin/bash

# Requires hyperfine (https://github.com/sharkdp/hyperfine)

hyperfine -m 2 --export-markdown bench-all.md \
'gcc/binarytree.gcc_run 21' 'gcc/binarytree2.gcc_run 21' 'gcc/binarytree3.gcc_run 21' 'gcc/binarytree4.gcc_run 21' \
'java -cp java binarytree 21' 'java -cp java binarytree2 21' 'java -cp java binarytree3 21' 'java -cp java binarytree4 21' \
'go/binarytree.go_run 21' 'go/binarytree2.go_run 21' 'go/binarytree3.go_run 21' 'go/binarytree4.go_run 21' 'go/binarytree4_1.go_run 21'

#hyperfine -m 2 --export-markdown bench-1.md 'gcc/binarytree.gcc_run 21' java -cp java binarytree 21' 'go/binarytree.go_run 21'

#hyperfine -m 2 --export-markdown bench-2.md 'gcc/binarytree2.gcc_run 21' 'java -cp java binarytree2 21' 'go/binarytree2.go_run 21'

#hyperfine -m 2 --export-markdown bench-3.md 'gcc/binarytree3.gcc_run 21' 'java -cp java binarytree3 21' 'go/binarytree3.go_run 21'

#hyperfine -m 2 --export-markdown bench-4.md 'gcc/binarytree4.gcc_run 21' 'java -cp java binarytree4 21' 'go/binarytree4.go_run 21' 'go/binarytree4_1.go_run 21'
