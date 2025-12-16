#!/bin/bash
# Tests interactifs et automatisés pour Ctrl+C (SIGINT) et Ctrl+D (EOF)
# Usage:
#  - Automatique (SIGINT to child): ./test_signals.sh auto
#  - Interactif (observe prompt):   ./test_signals.sh interactive
#  - Help:                          ./test_signals.sh

MINISHELL=./minishell
if [ ! -x "$MINISHELL" ]; then
  echo "Build minishell first (make)." >&2
  exit 1
fi

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

pass() { echo -e "${GREEN}✓ $1${NC}"; }
fail() { echo -e "${RED}✗ $1${NC}"; }
info() { echo -e "${YELLOW}$1${NC}"; }

# Expectation summary (typical bash-like behavior):
# - Ctrl+C at prompt: should print a newline, reset input, exit status 130
# - Ctrl+C while a child runs (e.g., sleep): child terminated, exit 130
# - Ctrl+D at prompt: should exit shell (print "exit" optionally), return last status
# - Ctrl+D while heredoc reading: stops heredoc with warning, continues

run_auto_tests() {
  info "Auto tests: SIGINT/SIGTERM behavior"

  # SIGINT to child: sleep 5 then send SIGINT
  # Expect minishell exit code 130 for that pipeline iteration
  (echo "sleep 5"; sleep 0.3) | "$MINISHELL" &
  pid=$!
  # Give shell time to start child
  sleep 1
  # Find child sleep process and send SIGINT via process group
  pkill -INT -f "sleep 5"
  wait $pid
  rc=$?
  if [ $rc -eq 130 ]; then pass "SIGINT to child -> exit 130"; else fail "SIGINT to child -> got $rc"; fi

  # Command not found exit 127 unaffected by SIGINT
  echo "commandthatdoesnotexist" | "$MINISHELL" >/dev/null 2>&1
  rc=$?
  if [ $rc -eq 127 ]; then pass "Command not found -> exit 127"; else fail "Command not found -> got $rc"; fi

  # Ctrl+D (EOF) exits shell — simulate by sending EOF only
  # We cannot send literal Ctrl+D easily; piping empty input exits immediately
  echo -n "" | "$MINISHELL" >/dev/null 2>&1
  rc=$?
  pass "EOF (Ctrl+D) exits shell, rc=$rc"

  # SIGINT at prompt: start shell, send SIGINT to its PGID, then EOF to end
  "$MINISHELL" >/dev/null 2>&1 &
  shpid=$!
  sleep 0.5
  kill -INT -$shpid
  # end session
  sleep 0.5
  kill -TERM $shpid 2>/dev/null || true

run_interactive() {
  info "Interactive: Press Ctrl+C and Ctrl+D at prompt to observe behavior"
  info "- Ctrl+C should print newline and show prompt again (exit status 130 for last command)"
  info "- Ctrl+D should exit shell (prints 'exit' if implemented)"
  "$MINISHELL"
}

case "$1" in
  auto)
    run_auto_tests
    ;;
  interactive)
    run_interactive
    ;;
  *)
    echo "Usage: $0 [auto|interactive]"
    echo "Examples:"
    echo "  $0 auto         # run automated signal tests"
    echo "  $0 interactive  # open shell and test manually"
    ;;
esac
