# 42sh

`42sh` is a small Unix shell. It reads commands from a terminal, from a script, or from a single `-c` string, then runs builtins and external programs with the usual control operators.

## Build

```sh
make
make test
make clean
```

`make test` rebuilds with AddressSanitizer and UndefinedBehaviorSanitizer and runs `tests/test_shell.sh`. The compiler flags are `-std=gnu11 -Wall -Wextra -Werror -pedantic`.

The binary is `./42sh`.

## Usage

```text
42sh              read commands from the terminal, or from stdin
42sh file         run a script
42sh -c command   run one command line
42sh -h           print this usage
```

A prompt is printed only when standard input is a terminal. The prompt is the last two directories of the working directory, for example `src/42sh> `.

The exit status of `42sh` is the status of the last command. `exit n` stops the shell with status `n` and does not run anything after it on the same line.

## Language

A line can hold several commands:

| Syntax | Meaning |
| --- | --- |
| `cmd1; cmd2` | run `cmd2` after `cmd1` |
| `cmd1 && cmd2` | run `cmd2` only when `cmd1` succeeds |
| `cmd1 \|\| cmd2` | run `cmd2` only when `cmd1` fails |
| `cmd1 \| cmd2` | pipe standard output of `cmd1` into `cmd2` |

`&&` and `||` are left-associative and have the same precedence, so `true || echo no && echo yes` prints `yes`.

The status of a pipeline is the status of its last command.

### Quoting and escaping

Single quotes keep every character literal, including `$`. Double quotes group words and still expand `$VAR`, `${VAR}`, and `$?`. A backslash outside quotes, or inside double quotes, escapes the next character.

```sh
echo 'a  b'      # a  b
echo "$HOME"     # value of HOME
echo a\|b        # a|b
```

An undefined variable is an error and the command does not run:

```text
NAME: Undefined variable.
```

### Redirections

Each simple command accepts one input redirection and one output redirection.

| Syntax | Meaning |
| --- | --- |
| `< file` | read standard input from `file` |
| `> file` | write standard output to `file`, truncating it |
| `>> file` | append standard output to `file` |
| `<< WORD` | read standard input until a line containing only `WORD` |

The here-document body is not expanded. The delimiter is literal after quotes are removed, so `<< 'EOF'` and `<< EOF` behave the same.

Missing files, ambiguous redirections, and a missing redirection target are reported on standard error and the command is not run.

## Builtins

| Command | Behavior |
| --- | --- |
| `echo [-n] args...` | print arguments separated by spaces; `-n` omits the newline |
| `cd [path]` | change directory; no argument, `~`, or `--` uses `HOME`; `-` uses `OLDPWD` and prints the new directory |
| `pwd` | external `/bin/pwd` (or whatever `PATH` finds) |
| `env` | print the shell environment |
| `setenv NAME value` | set or replace a variable; with no arguments, print the environment |
| `unsetenv NAME` | remove a variable |
| `exit [n]` | leave the shell; `n` defaults to 0 |
| `history` | print the session history, oldest first |
| `!n` | rerun history entry `n` |
| `!!` | rerun the newest entry |
| `source file` | load aliases from `file` |
| `profile name` | switch the history file to `$HOME/.42sh_history_<name>` |

`cd`, `setenv`, `unsetenv`, `exit`, `source`, and `profile` change the shell. Inside a pipeline they run in a subshell, so they do not change the parent shell. `echo`, `env`, and `history` always run in a child, which means their output can be redirected.

Variable names start with a letter or `_` and then contain letters, digits, or `_`.

## Startup files

When `HOME` is set, the shell reads:

- `$HOME/.42shrc` for aliases, one per line: `alias name value` or `alias name 'value with spaces'`
- `$HOME/.42sh_history_default` for the command history

Aliases expand only in command position, not inside arguments or quotes.

```sh
alias ll 'echo aliased'
ll          # prints: aliased
echo ll     # prints: ll
```

## Signals

In a terminal, Ctrl-C cancels the current input line and returns to the prompt. A running program receives the default `SIGINT`. A segmentation fault or floating-point exception is reported on standard error, and the status is `128` plus the signal number.

## What this shell does not do

- job control (`&`, `fg`, `bg`)
- pathname globs (`*.c`)
- command substitution (`` `cmd` `` or `$(cmd)`)
- file-descriptor redirections such as `2>` or `&>`
- here-document body expansion
- joining a quoted newline into one word when reading line by line

## Layout

`src/main.c` is the read-eval loop. `src/syntax.c` classifies quotes, escapes, and operators. The command line is split into `;` lists, then `&&` / `||` lists, then pipelines, then words and redirections. `src/execute_interpipe.c` forks the pipeline. Builtins that must affect the shell stay in the parent when they are not part of a pipeline.

## Authors

Original work by Baptiste Veyssiere, Nathan Scutari, Ilyas Semmaoui, and vigner_g (2016). This tree keeps that shell and fixes its build, status handling, parsing, and memory bugs on a current compiler.
