#!/bin/bash

# Script de test pour comparer les codes de sortie de minishell avec bash
# Usage: ./test_exit_codes.sh

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

MINISHELL="./minishell"
PASSED=0
FAILED=0

# Fonction pour tester une commande
test_exit_code() {
    local cmd="$1"
    local description="$2"
    
    echo -e "${YELLOW}Test:${NC} $description"
    echo -e "Command: ${YELLOW}$cmd${NC}"
    
    # Test avec bash
    bash -c "$cmd" >/dev/null 2>&1
    local bash_exit=$?
    
    # Test avec minishell
    echo "$cmd" | $MINISHELL >/dev/null 2>&1
    local minishell_exit=$?
    
    # Comparaison
    if [ $bash_exit -eq $minishell_exit ]; then
        echo -e "${GREEN}✓ PASS${NC} (bash: $bash_exit, minishell: $minishell_exit)"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC} (bash: $bash_exit, minishell: $minishell_exit)"
        ((FAILED++))
    fi
    echo ""
}

# Vérifier que minishell existe
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: $MINISHELL not found. Run 'make' first.${NC}"
    exit 1
fi

echo "========================================="
echo "  Minishell Exit Code Comparison Tests  "
echo "========================================="
echo ""

# Tests de commandes réussies
test_exit_code "echo hello" "Simple echo"
test_exit_code "ls" "ls (success)"
test_exit_code "pwd" "pwd command"
test_exit_code "/bin/true" "true command"

# Tests de commandes échouées
test_exit_code "/bin/false" "false command (exit 1)"
test_exit_code "ls /nonexistent" "ls with invalid path"
test_exit_code "cat /nonexistent_file" "cat with invalid file"

# Tests de commandes inexistantes
test_exit_code "commandthatdoesnotexist" "Command not found (should be 127)"
test_exit_code "/nonexistent/binary" "Non-existent binary path"

# Tests avec redirections
test_exit_code "echo test > /dev/null" "Redirect output (success)"
test_exit_code "cat < /nonexistent" "Redirect from non-existent file"
test_exit_code "echo test > /invalid/path/file" "Redirect to invalid path"

# Tests avec pipes
test_exit_code "echo hello | cat" "Simple pipe (success)"
test_exit_code "ls | grep test" "Pipe with grep"
test_exit_code "false | true" "Pipe: false | true"
test_exit_code "true | false" "Pipe: true | false"

# Tests avec builtins
test_exit_code "cd /tmp" "cd to valid directory"
test_exit_code "cd /nonexistent_dir" "cd to invalid directory"
test_exit_code "export TEST=value" "export variable"
test_exit_code "unset PATH" "unset variable"

# Tests syntaxe invalide (à adapter selon votre gestion)
test_exit_code "|" "Pipe at start (syntax error)"
test_exit_code "echo |" "Pipe at end (syntax error)"
#test_exit_code "echo || cat" "double pipe -> pas besoin de le gerer"
test_exit_code "echo <" "redir without word"

# Résumé
echo "========================================="
echo -e "Results: ${GREEN}$PASSED passed${NC}, ${RED}$FAILED failed${NC}"
echo "========================================="

if [ $FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi
