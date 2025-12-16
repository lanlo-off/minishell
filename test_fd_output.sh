#!/bin/bash
# Test des file descriptors de sortie (stdout/stderr) avec et sans redirections
# Usage: ./test_fd_output.sh

MINISHELL=./minishell
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

PASSED=0
FAILED=0
TMPDIR=$(mktemp -d)

pass() { echo -e "${GREEN}✓ $1${NC}"; ((PASSED++)); }
fail() { echo -e "${RED}✗ $1${NC}"; ((FAILED++)); }
info() { echo -e "${YELLOW}$1${NC}"; }

cleanup() {
    rm -rf "$TMPDIR"
}
trap cleanup EXIT

# Fonction pour comparer stdout
test_stdout() {
    local cmd="$1"
    local description="$2"
    local bash_out="$TMPDIR/bash_stdout_$$"
    local mini_out="$TMPDIR/mini_stdout_$$"
    local mini_raw="$TMPDIR/mini_raw_$$"
    
    echo -e "${YELLOW}Test:${NC} $description"
    echo -e "Command: ${YELLOW}$cmd${NC}"
    
    # Exécuter avec bash
    bash -c "$cmd" > "$bash_out" 2>/dev/null
    
    # Exécuter avec minishell et filtrer les prompts
    (echo "$cmd"; echo "exit") | $MINISHELL > "$mini_raw" 2>/dev/null
    # Filtrer: enlever les lignes de prompt et la commande echo
    grep -v "^AU SUIVANT> " "$mini_raw" | grep -v "^exit$" > "$mini_out"
    
    # Comparer
    if diff -q "$bash_out" "$mini_out" >/dev/null 2>&1; then
        pass "stdout identique"
    else
        fail "stdout différent"
        echo "  Bash output (${bash_out}):"
        od -c "$bash_out" | head -3
        echo "  Minishell output (${mini_out}):"
        od -c "$mini_out" | head -3
        echo "  Diff:"
        diff -u "$bash_out" "$mini_out" || true
    fi
    rm -f "$bash_out" "$mini_out" "$mini_raw"
    echo ""
}

# Fonction pour tester redirection vers fichier
test_redirect_to_file() {
    local cmd="$1"
    local description="$2"
    local bash_file="$TMPDIR/bash_file_$$"
    local mini_file="$TMPDIR/mini_file_$$"
    
    echo -e "${YELLOW}Test:${NC} $description"
    echo -e "Command: ${YELLOW}$cmd (remplace OUTPUT par fichier)${NC}"
    
    # Préparer les commandes avec les bons fichiers
    local bash_cmd="${cmd//OUTPUT/$bash_file}"
    local mini_cmd="${cmd//OUTPUT/$mini_file}"
    
    # Exécuter
    bash -c "$bash_cmd" 2>/dev/null
    (echo "$mini_cmd"; echo "exit") | $MINISHELL >/dev/null 2>&1
    
    # Comparer les fichiers créés
    if [ -f "$bash_file" ] && [ -f "$mini_file" ]; then
        if diff -q "$bash_file" "$mini_file" >/dev/null 2>&1; then
            pass "fichier identique"
        else
            fail "fichier différent"
            echo "  Bash file (${bash_file}):"
            od -c "$bash_file" | head -3
            echo "  Minishell file (${mini_file}):"
            od -c "$mini_file" | head -3
            echo "  Diff:"
            diff -u "$bash_file" "$mini_file" || true
        fi
    elif [ ! -f "$bash_file" ] && [ ! -f "$mini_file" ]; then
        pass "aucun fichier créé (comportement identique)"
    else
        fail "fichier créé uniquement par l'un des shells"
        [ -f "$bash_file" ] && echo "  Bash créé: ${bash_file}"
        [ -f "$mini_file" ] && echo "  Minishell créé: ${mini_file}"
    fi
    rm -f "$bash_file" "$mini_file"
    echo ""
}

# Fonction pour tester les pipes
test_pipe() {
    local cmd="$1"
    local description="$2"
    local bash_out="$TMPDIR/bash_pipe_$$"
    local mini_out="$TMPDIR/mini_pipe_$$"
    local mini_raw="$TMPDIR/mini_pipe_raw_$$"
    
    echo -e "${YELLOW}Test:${NC} $description"
    echo -e "Command: ${YELLOW}$cmd${NC}"
    
    # Exécuter
    bash -c "$cmd" > "$bash_out" 2>/dev/null
    (echo "$cmd"; echo "exit") | $MINISHELL > "$mini_raw" 2>/dev/null
    # Filtrer les prompts
    grep -v "^AU SUIVANT> " "$mini_raw" | grep -v "^exit$" > "$mini_out"
    
    # Comparer
    if diff -q "$bash_out" "$mini_out" >/dev/null 2>&1; then
        pass "pipe output identique"
    else
        fail "pipe output différent"
        echo "  Bash (${bash_out}):"
        od -c "$bash_out" | head -3
        echo "  Minishell (${mini_out}):"
        od -c "$mini_out" | head -3
        echo "  Diff:"
        diff -u "$bash_out" "$mini_out" || true
    fi
    rm -f "$bash_out" "$mini_out" "$mini_raw"
    echo ""
}

echo "========================================="
echo "  Test des File Descriptors de Sortie   "
echo "========================================="
echo ""

info "=== Tests de stdout basique ==="
test_stdout "echo hello" "echo simple"
test_stdout "echo hello world" "echo avec espaces"
test_stdout "/bin/ls -1" "ls output"

info "=== Tests de redirections stdout vers fichier ==="
test_redirect_to_file "echo test > OUTPUT" "redirection simple >"
test_redirect_to_file "/bin/ls -1 / > OUTPUT" "redirection ls"

info "=== Tests de pipes simples ==="
test_pipe "echo hello | cat" "pipe echo -> cat"
test_pipe "echo 'abc' | grep b" "pipe echo -> grep"
test_pipe "/bin/ls / | wc -l" "pipe ls -> wc"
test_pipe "echo test | cat | cat" "double pipe"

info "=== Tests de pipes avec redirections ==="
test_redirect_to_file "echo hello | cat > OUTPUT" "pipe + redirection"
test_redirect_to_file "echo test | grep test > OUTPUT" "pipe grep + redirection"
test_pipe "cat < /etc/hosts | grep -v '#' | head -3" "redirection input + pipes"

info "=== Tests de redirections multiples ==="
test_redirect_to_file "> OUTPUT echo test" "redirection avant commande"

info "=== Tests edge cases ==="
test_stdout "echo" "echo vide"
test_stdout "echo ''" "echo empty string"
test_redirect_to_file "echo test > OUTPUT > OUTPUT" "double redirection (dernier gagne)"

# Résumé
echo "========================================="
echo -e "Results: ${GREEN}$PASSED passed${NC}, ${RED}$FAILED failed${NC}"
echo "========================================="

if [ $FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi
