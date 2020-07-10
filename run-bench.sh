#!/bin/bash

# Requires hyperfine (https://github.com/sharkdp/hyperfine)

hyperfine -m 2 --export-markdown bench-all.md \
'gcc/binarytrees.gcc_run 21' 'gcc/binarytrees2.gcc_run 21' 'gcc/binarytrees3.gcc_run 21' 'gcc/binarytrees4.gcc_run 21' \
'java -cp java binarytrees 21' 'java -cp java binarytrees2 21' 'java -cp java binarytrees3 21' 'java -cp java binarytrees4 21' \
'go/binarytrees.go_run 21' 'go/binarytrees2.go_run 21' 'go/binarytrees3.go_run 21' 'go/binarytrees4.go_run 21' 'go/binarytrees4_1.go_run 21'

#hyperfine -m 2 --export-markdown bench-1.md 'gcc/binarytrees.gcc_run 21' java -cp java binarytrees 21' 'go/binarytrees.go_run 21'

#hyperfine -m 2 --export-markdown bench-2.md 'gcc/binarytrees2.gcc_run 21' 'java -cp java binarytrees2 21' 'go/binarytrees2.go_run 21'

#hyperfine -m 2 --export-markdown bench-3.md 'gcc/binarytrees3.gcc_run 21' 'java -cp java binarytrees3 21' 'go/binarytrees3.go_run 21'

#hyperfine -m 2 --export-markdown bench-4.md 'gcc/binarytrees4.gcc_run 21' 'java -cp java binarytrees4 21' 'go/binarytrees4.go_run 21' 'go/binarytrees4_1.go_run 21'
