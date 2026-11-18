#!/bin/sh
# Exercise 42sh. The script rebuilds with sanitizers and fails on the first mismatch.

set -u

ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
cd "$ROOT" || exit 1

make ASAN=1 re >/tmp/42sh-build.log 2>&1 || {
  echo "build failed"
  cat /tmp/42sh-build.log
  exit 1
}

BIN=$ROOT/42sh
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
HOME=$TMP/home
mkdir -p "$HOME"
export HOME
export ASAN_OPTIONS=detect_leaks=1:halt_on_error=1

FAIL=0

check() {
  name=$1
  exp_status=$2
  exp_out=$3
  exp_err=$4
  cmd=$5

  "$BIN" -c "$cmd" </dev/null >"$TMP/out" 2>"$TMP/err"
  status=$?
  printf '%s' "$exp_out" >"$TMP/exp_out"
  printf '%s' "$exp_err" >"$TMP/exp_err"
  if [ "$status" -ne "$exp_status" ] ||
     ! cmp -s "$TMP/out" "$TMP/exp_out" ||
     ! cmp -s "$TMP/err" "$TMP/exp_err"; then
    echo "FAIL $name"
    echo "  status: got $status want $exp_status"
    echo "  stdout:"; cat -A "$TMP/out"
    echo "  want out:"; printf '%s' "$exp_out" | cat -A
    echo "  stderr:"; cat -A "$TMP/err"
    echo "  want err:"; printf '%s' "$exp_err" | cat -A
    FAIL=1
  else
    echo "ok   $name"
  fi
}

feed() {
  name=$1
  exp_status=$2
  exp_out=$3
  exp_err=$4
  script=$5

  printf '%s' "$script" | "$BIN" >"$TMP/out" 2>"$TMP/err"
  status=$?
  printf '%s' "$exp_out" >"$TMP/exp_out"
  printf '%s' "$exp_err" >"$TMP/exp_err"
  if [ "$status" -ne "$exp_status" ] ||
     ! cmp -s "$TMP/out" "$TMP/exp_out" ||
     ! cmp -s "$TMP/err" "$TMP/exp_err"; then
    echo "FAIL $name"
    echo "  status: got $status want $exp_status"
    echo "  stdout:"; cat -A "$TMP/out"
    echo "  want out:"; printf '%s' "$exp_out" | cat -A
    echo "  stderr:"; cat -A "$TMP/err"
    echo "  want err:"; printf '%s' "$exp_err" | cat -A
    FAIL=1
  else
    echo "ok   $name"
  fi
}

check echo 0 "hello
" "" "echo hello"
check echo-n 0 "hello" "" "echo -n hello"
check echo-words 0 "hello world
" "" "echo hello world"
check quotes 0 "a b
a  b
" "" "echo \"a b\"; echo 'a  b'"
check single-quote-literal 0 "\$HOME
" "" "echo '\$HOME'"
check semicolon-in-quotes 0 "a;b
c
" "" "echo \"a;b\"; echo c"
check escape 0 "a|b
" "" "echo a\\|b"
check false 1 "" "" "false"
check and-or 0 "yes
recovered
" "" "true && echo yes; false && echo no; false || echo recovered"
check precedence 0 "yes
" "" "true || echo no && echo yes"
check pipe 0 "hello
" "" "echo hello | cat"
check pipe-status 0 "" "" "false | true"
check pipe-fail 1 "" "" "true | false"
check status-var 0 "1
" "" "false; echo \$?"
check home 0 "$HOME
" "" "echo \$HOME"
check braces 0 "$HOME
" "" "echo \${HOME}"
check assign 0 "Ada
Ada/x
" "" "setenv NAME Ada; echo \"\$NAME\"; echo \$NAME/x"
check unset 1 "" "NAME: Undefined variable.
" "setenv NAME Ada; unsetenv NAME; echo \$NAME"
check not-found 1 "" "missing: Command not found.
" "missing"
check directory 1 "" "/tmp: Is a directory.
" "/tmp"
check dot 1 "" ".: Command not found.
" "."
check syntax 1 "" "Invalid null command.
" "|"
check quote-error 1 "" "Unmatched quote.
" "echo \"unterminated"
check redirect-missing 1 "" "Missing name for redirect.
" "echo foo >"
check ambiguous 1 "" "Ambiguous output redirect.
" "echo hi > $TMP/a > $TMP/b"
check exit-stop 3 "hi
" "" "echo hi; exit 3; echo no"
check exit-bad 1 "" "exit: Badly formed number.
" "exit foo"
check exit-syntax 1 "" "exit: Expression Syntax.
" "exit 1 2"
check cd-missing 1 "" "/no/such: No such file or directory.
" "cd /no/such"
check cd-many 1 "" "cd: Too many arguments.
" "cd a b"
check empty-arg 0 "
end
" "" "echo \"\"; echo end"
check pipe-cd 0 "$ROOT
" "" "echo hi | cd /tmp; pwd"

mkdir -p "$TMP/a" "$TMP/b"
check cd-dash 0 "$TMP/a
$TMP/a
" "" "cd $TMP/a; cd $TMP/b; cd -; pwd"

printf 'one\n' >"$TMP/src.txt"
check input-redirect 0 "one
" "" "cat < $TMP/src.txt"
check output-redirect 0 "hi
more
" "" "echo hi > $TMP/out.txt; echo more >> $TMP/out.txt; cat < $TMP/out.txt"

feed heredoc 0 "here
tail
" "" "cat << EOF
here
EOF
echo tail
"

printf "alias ll 'echo aliased'\n" >"$HOME/.42shrc"
check alias 0 "aliased
ll
" "" "ll; echo ll"
check alias-glued 0 "aliased
glued
" "" "ll;echo glued"
check alias-quoted 1 "" "ll: Command not found.
" "'ll'"
check alias-pipe 0 "aliased
" "" "ll | cat"

rm -f "$HOME/.42sh_history_default"
feed history 0 "one
two
1 echo one
2 echo two
3 history
" "" "echo one
echo two
history
"

rm -f "$HOME/.42sh_history_default"
feed history-bang 0 "one
two
one
" "" "echo one
echo two
!1
"

printf 'echo from-file\n' >"$TMP/script"
"$BIN" "$TMP/script" </dev/null >"$TMP/out" 2>"$TMP/err"
status=$?
if [ "$status" -ne 0 ] || ! cmp -s "$TMP/out" - <<'EOF'
from-file
EOF
then
  echo "FAIL script-file"
  FAIL=1
else
  echo "ok   script-file"
fi

"$BIN" </dev/null >"$TMP/out" 2>"$TMP/err"
status=$?
if [ "$status" -ne 0 ] || [ -s "$TMP/out" ] || [ -s "$TMP/err" ]; then
  echo "FAIL empty-stdin"
  FAIL=1
else
  echo "ok   empty-stdin"
fi

"$BIN" -h >"$TMP/out" 2>"$TMP/err"
status=$?
if [ "$status" -ne 0 ] || ! grep -q "Usage: 42sh" "$TMP/out"; then
  echo "FAIL help"
  FAIL=1
else
  echo "ok   help"
fi

"$BIN" /no/such/42sh-script >"$TMP/out" 2>"$TMP/err"
status=$?
printf '%s' "42sh: /no/such/42sh-script: No such file or directory.
" >"$TMP/exp_err"
if [ "$status" -ne 1 ] || [ -s "$TMP/out" ] || ! cmp -s "$TMP/err" "$TMP/exp_err"; then
  echo "FAIL missing-script"
  echo "  status: $status"
  echo "  stderr:"; cat -A "$TMP/err"
  FAIL=1
else
  echo "ok   missing-script"
fi

"$BIN" -x >"$TMP/out" 2>"$TMP/err"
status=$?
if [ "$status" -ne 2 ] || ! grep -q "Usage: 42sh" "$TMP/err"; then
  echo "FAIL bad-flag"
  FAIL=1
else
  echo "ok   bad-flag"
fi

if [ "$FAIL" -ne 0 ]; then
  echo "some tests failed"
  exit 1
fi
echo "all tests passed"
exit 0
