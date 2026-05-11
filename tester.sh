#!/bin/bash

# ============================================================
# test_codexion.sh — Tests automatiques pour codexion
# Usage: bash test_codexion.sh
# ============================================================

BIN="./codexion"
PASS=0
FAIL=0
TOTAL=0

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
CYAN="\033[0;36m"
RESET="\033[0m"

# ============================================================
# HELPERS
# ============================================================

ok() {
    echo -e "${GREEN}[OK]${RESET} $1"
    PASS=$((PASS + 1))
    TOTAL=$((TOTAL + 1))
}

ko() {
    echo -e "${RED}[KO]${RESET} $1"
    FAIL=$((FAIL + 1))
    TOTAL=$((TOTAL + 1))
}

section() {
    echo ""
    echo -e "${YELLOW}══════════════════════════════════════════${RESET}"
    echo -e "${YELLOW}  $1${RESET}"
    echo -e "${YELLOW}══════════════════════════════════════════${RESET}"
}

run_with_spinner() {
    local timeout_s=$1
    shift
    local frames=('⠋' '⠙' '⠹' '⠸' '⠼' '⠴' '⠦' '⠧' '⠇' '⠏')
    local start elapsed i
    local fifo=/tmp/spinner_fifo_$$

    mkfifo "$fifo"
    start=$(date +%s)

    # Lance la commande, sort dans le fifo ET dans le fichier
    timeout "$timeout_s" "$@" 2>&1 | tee "$fifo" > /tmp/spinner_out.txt &
    local cmd_pid=$!

    # Thread lecteur : affiche les logs à droite
    (
        while IFS= read -r line; do
            printf "\033[s\033[45G\033[2K${CYAN}${line:0:80}${RESET}\033[u" >&2
        done < "$fifo"
    ) &
    local reader_pid=$!

    # Spinner à gauche
    i=0
    while kill -0 $cmd_pid 2>/dev/null; do
        elapsed=$(( $(date +%s) - start ))
        remaining=$(( timeout_s - elapsed ))
        printf "\r${YELLOW}${frames[$((i % 10))]}${RESET} %ds/%ds (-%ds)  " \
            "$elapsed" "$timeout_s" "$remaining" >&2
        sleep 0.1
        i=$((i + 1))
    done

    wait $cmd_pid
    local ret=$?
    kill $reader_pid 2>/dev/null
    wait $reader_pid 2>/dev/null

    printf "\r%-60s\r" " " >&2
    rm -f "$fifo"

    cat /tmp/spinner_out.txt
    rm -f /tmp/spinner_out.txt
    return $ret
}

# ============================================================
# 0. COMPILATION
# ============================================================

section "0. COMPILATION"

make re 2>/tmp/compile_err.txt 1>/dev/null
if [ $? -eq 0 ] && [ -f "$BIN" ]; then
    ok "Compilation avec -Wall -Wextra -Werror"
else
    ko "Compilation échouée — arrêt des tests"
    echo ""
    echo -e "${RED}── Erreurs de compilation ──${RESET}"
    cat /tmp/compile_err.txt
    echo -e "${RED}────────────────────────────${RESET}"
    rm -f /tmp/compile_err.txt
    exit 1
fi

# ============================================================
# 1. ARGUMENTS INVALIDES
# ============================================================

section "1. ARGUMENTS INVALIDES"

test_invalid() {
    local desc="$1"
    shift
    out=$("$BIN" "$@" 2>&1)
    ret=$?
    if [ $ret -ne 0 ]; then
        ok "$desc → rejeté correctement"
    else
        ko "$desc → devrait être rejeté (exit 0 mais args invalides)"
    fi
}

test_invalid "Pas d'arguments"
test_invalid "Trop peu d'arguments" 3 2000 200
test_invalid "Scheduler invalide" 3 2000 200 200 200 3 10 random
test_invalid "Coders négatifs" -1 2000 200 200 200 3 10 fifo
test_invalid "Burnout négatif" 3 -200 200 200 200 3 10 fifo
test_invalid "Cooldown négatif" 3 2000 200 200 200 3 -10 fifo
test_invalid "Non-entier" 3 abc 200 200 200 3 10 fifo
test_invalid "0 coders" 0 2000 200 200 200 3 10 fifo

# ============================================================
# 2. CAS SIMPLE — 1 CODER
# ============================================================

section "2. CAS SIMPLE — 1 CODER"

echo -e "${YELLOW}▶ Test burnout 1 coder...${RESET}"
out=$(run_with_spinner 5 "$BIN" 1 800 200 200 200 3 10 fifo 2>&1)
ret=$?
if echo "$out" | grep -q "burned out"; then
    ok "1 coder — burnout détecté"
else
    ko "1 coder — burnout non détecté"
fi

# ============================================================
# 3. CAS NORMAUX — FIFO
# ============================================================

section "3. CAS NORMAUX — FIFO"

test_normal_fifo() {
    local desc="$1"
    local args="$2"
    local timeout_s="$3"

    echo -e "${YELLOW}▶ $desc...${RESET}"
    out=$(run_with_spinner "$timeout_s" $BIN $args)
    ret=$?

    if [ $ret -eq 124 ]; then
        ko "$desc → timeout (deadlock?)"
        return
    fi

    bad=$(echo "$out" | grep -v -E "^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$")
    if [ -n "$bad" ]; then
        ko "$desc → format de log incorrect: $bad"
        return
    fi

    ok "$desc"
}

test_normal_fifo "FIFO / 3 coders / burnout 2000"   "3 2000 200 200 200 3 10 fifo" 15
test_normal_fifo "FIFO / 5 coders / burnout 3000"   "5 3000 200 200 200 3 10 fifo" 20
test_normal_fifo "FIFO / 2 coders / burnout 1000"   "2 1000 150 150 150 3 10 fifo" 10
test_normal_fifo "FIFO / 10 coders / burnout 5000"  "10 5000 200 200 200 3 10 fifo" 30

# ============================================================
# 4. CAS NORMAUX — EDF
# ============================================================

section "4. CAS NORMAUX — EDF"

test_normal_edf() {
    local desc="$1"
    local args="$2"
    local timeout_s="$3"

    echo -e "${YELLOW}▶ $desc...${RESET}"
    out=$(run_with_spinner "$timeout_s" $BIN $args)
    ret=$?

    if [ $ret -eq 124 ]; then
        ko "$desc → timeout (deadlock?)"
        return
    fi

    bad=$(echo "$out" | grep -v -E "^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$")
    if [ -n "$bad" ]; then
        ko "$desc → format de log incorrect: $bad"
        return
    fi

    ok "$desc"
}

test_normal_edf "EDF / 3 coders / burnout 2000"   "3 2000 200 200 200 3 10 edf" 15
test_normal_edf "EDF / 5 coders / burnout 3000"   "5 3000 200 200 200 3 10 edf" 20
test_normal_edf "EDF / 2 coders / burnout 1000"   "2 1000 150 150 150 3 10 edf" 10
test_normal_edf "EDF / 10 coders / burnout 5000"  "10 5000 200 200 200 3 10 edf" 30

# ============================================================
# 5. BURNOUT — VÉRIFICATION
# ============================================================

section "5. BURNOUT — VÉRIFICATION"

test_burnout() {
    local desc="$1"
    local args="$2"
    local timeout_s="$3"

    echo -e "${YELLOW}▶ $desc...${RESET}"
    out=$(run_with_spinner "$timeout_s" $BIN $args)
    ret=$?

    if [ $ret -eq 124 ]; then
        ko "$desc → timeout"
        return
    fi

    if ! echo "$out" | grep -q "burned out"; then
        ko "$desc → pas de burnout détecté"
        return
    fi

    burnout_id=$(echo "$out" | grep "burned out" | head -1 | awk '{print $2}')
    burnout_ts=$(echo "$out" | grep "burned out" | head -1 | awk '{print $1}')
    last_compile_ts=$(echo "$out" | grep "^[0-9]* $burnout_id is compiling" | tail -1 | awk '{print $1}')

    if [ -n "$last_compile_ts" ] && [ "$burnout_ts" -ge "$last_compile_ts" ]; then
        ok "$desc → burnout après dernière compile (ts: $last_compile_ts → $burnout_ts)"
    else
        ok "$desc → burnout détecté pour coder $burnout_id"
    fi
}

test_burnout "Burnout / 3 coders / fifo" "3 2000 200 200 200 3 10 fifo" 15
test_burnout "Burnout / 3 coders / edf"  "3 2000 200 200 200 3 10 edf"  15

# ============================================================
# 6. STRESS TEST
# ============================================================

section "6. STRESS TEST"

test_stress() {
    local desc="$1"
    local args="$2"
    local timeout_s="$3"

    echo -e "${YELLOW}▶ $desc...${RESET}"
    out=$(run_with_spinner "$timeout_s" $BIN $args)
    ret=$?

    if [ $ret -eq 124 ]; then
        ko "$desc → timeout (deadlock?)"
        return
    fi

    lines=$(echo "$out" | wc -l)
    ok "$desc → terminé ($lines lignes de log)"
}

test_stress "Stress / 20 coders / fifo" "20 3000 100 100 100 3 10 fifo" 40
test_stress "Stress / 20 coders / edf"  "20 3000 100 100 100 3 10 edf"  40

# ============================================================
# 7. RÉPÉTABILITÉ — PLUSIEURS RUNS
# ============================================================

section "7. RÉPÉTABILITÉ"

echo -e "${YELLOW}▶ 5 runs successifs sans deadlock...${RESET}"
repeat_ok=0
for i in $(seq 1 5); do
    out=$(run_with_spinner 10 $BIN 3 1500 200 200 200 3 10 fifo)
    ret=$?
    if [ $ret -ne 124 ]; then
        repeat_ok=$((repeat_ok + 1))
    fi
done

if [ $repeat_ok -eq 5 ]; then
    ok "Répétabilité → 5/5 runs sans timeout"
else
    ko "Répétabilité → seulement $repeat_ok/5 runs sans timeout"
fi

# ============================================================
# 8. VALGRIND — MEMCHECK
# ============================================================

section "8. VALGRIND — MEMCHECK"

if ! command -v valgrind &>/dev/null; then
    echo -e "${YELLOW}[SKIP]${RESET} valgrind non installé"
else
    test_memcheck() {
        local desc="$1"
        local args="$2"

        echo -e "${YELLOW}▶ $desc...${RESET}"
        out=$(run_with_spinner 20 valgrind \
            --leak-check=full \
            --error-exitcode=1 \
            $BIN $args)
        ret=$?

        errors=$(echo "$out" | grep "ERROR SUMMARY" | awk '{print $4}')

        if [ "$errors" = "0" ]; then
            ok "$desc → 0 erreurs mémoire"
        else
            ko "$desc → $errors erreurs mémoire détectées"
            echo "$out" | grep "ERROR SUMMARY"
        fi
    }

    test_memcheck "Memcheck / 3 coders / fifo" "3 2000 200 200 200 3 10 fifo"
    test_memcheck "Memcheck / 3 coders / edf"  "3 2000 200 200 200 3 10 edf"
fi

# ============================================================
# 9. HELGRIND — DATA RACES
# ============================================================

section "9. HELGRIND — DATA RACES"

if ! command -v valgrind &>/dev/null; then
    echo -e "${YELLOW}[SKIP]${RESET} valgrind non installé"
else
    test_helgrind() {
        local desc="$1"
        local args="$2"

        echo -e "${YELLOW}▶ $desc...${RESET}"
        out=$(run_with_spinner 20 valgrind \
            --tool=helgrind \
            --error-exitcode=1 \
            $BIN $args)
        ret=$?

        errors=$(echo "$out" | grep "ERROR SUMMARY" | awk '{print $4}')

        if [ "$errors" = "0" ]; then
            ok "$desc → 0 erreurs Helgrind"
        else
            ko "$desc → $errors erreurs Helgrind détectées"
            echo "$out" | grep -A3 "Possible data race\|Lock order" | head -30
        fi
    }

    test_helgrind "Helgrind / 3 coders / fifo" "3 2000 200 200 200 3 10 fifo"
    test_helgrind "Helgrind / 3 coders / edf"  "3 2000 200 200 200 3 10 edf"
fi

# ============================================================
# 10. FORMAT DES LOGS
# ============================================================

section "10. FORMAT DES LOGS"

test_log_format() {
    local desc="$1"
    local args="$2"

    echo -e "${YELLOW}▶ $desc...${RESET}"
    out=$(run_with_spinner 15 $BIN $args)
    ret=$?

    if [ $ret -eq 124 ]; then
        ko "$desc → timeout"
        return
    fi

    bad=$(echo "$out" | grep -v -E "^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$")
    if [ -n "$bad" ]; then
        ko "$desc → lignes mal formatées:"
        echo "$bad" | head -5
    else
        ok "$desc → format correct"
    fi

    # Timestamps croissants
    prev=0
    ok_ts=1
    while IFS= read -r line; do
        ts=$(echo "$line" | awk '{print $1}')
        if [ -n "$ts" ] && [ "$ts" -lt "$prev" ]; then
            ko "$desc → timestamps non croissants ($prev → $ts)"
            ok_ts=0
            break
        fi
        [ -n "$ts" ] && prev=$ts
    done <<< "$out"
    [ $ok_ts -eq 1 ] && ok "$desc → timestamps croissants"
}

test_log_format "Format / 3 coders / fifo" "3 2000 200 200 200 3 10 fifo"
test_log_format "Format / 3 coders / edf"  "3 2000 200 200 200 3 10 edf"

# ============================================================
# RÉSUMÉ
# ============================================================

section "RÉSUMÉ"
echo -e "Total : $TOTAL | ${GREEN}OK : $PASS${RESET} | ${RED}KO : $FAIL${RESET}"
echo ""

if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}✓ Tous les tests passent !${RESET}"
    exit 0
else
    echo -e "${RED}✗ $FAIL test(s) échoué(s)${RESET}"
    exit 1
fi
