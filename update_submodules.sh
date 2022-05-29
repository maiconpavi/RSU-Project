#!/bin/bash
# Execute com bash: bash merge_rep.sh
SCRIPT=$(realpath $0)
SCRIPTPATH=$(dirname $SCRIPT)

(cd $SCRIPTPATH && git add .)
(cd $SCRIPTPATH && git commit -m "Commit")
(cd $SCRIPTPATH && git push)
(cd $SCRIPTPATH && git submodule update --init --recursive --force --checkout --remote)
(cd $SCRIPTPATH && git add .)
(cd $SCRIPTPATH && git commit -m "Commit")
(cd $SCRIPTPATH && git push)


