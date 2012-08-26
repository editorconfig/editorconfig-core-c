#!/bin/sh

# Make a source archive for the current EditorConfig version.

if [ $# -ne 1 ]; then
  echo "Usage:   "$0" VERSION"
  echo ""
  echo "e.g. "$0" 0.10.0"
  echo
  exit
fi

curl -L https://raw.github.com/Kentzo/git-archive-all/master/git-archive-all | python - editorconfig-$*.tar.gz
curl -L https://raw.github.com/Kentzo/git-archive-all/master/git-archive-all | python - editorconfig-$*.zip
