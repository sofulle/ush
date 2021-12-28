# check.sh

EXEC_FILE=$1
EXEC_DIR=$2
CHECK_DIR=$3

BLACK="\033[30;1m"
GREEN="\033[32;1m"
RED="\033[31;1m"
YELLOW="\033[33;1m"
BLUE="\033[34;1m"
VIOLET="\033[35;1m"
CYAN="\033[36;1m"
GRAY="\033[37;1m"

BOLD="\033[1m"
OPACITY="\033[2m"
UNDER="\033[4m"

UNSET="\033[0m"

PRMPT="[${GREEN}check${UNSET}]"

xxd ${EXEC_DIR}/${EXEC_FILE} > ${EXEC_DIR}.out
xxd ${CHECK_DIR}/${EXEC_FILE} > ${CHECK_DIR}.out

execfile="${EXEC_DIR}.out"
checkfile="${CHECK_DIR}.out"
execfile_size=$( cat $execfile | wc -c )
checkfile_size=$( cat $checkfile | wc -c )

if [ $execfile_size -lt $checkfile_size ]; then
    size=$execfile_size
else
    size=$checkfile_size
fi

printf "${PRMPT} User file uniqueness from the test file is ${YELLOW}${UNDER}"

dc -e "
3k
$( cmp -n $size -l $execfile $checkfile | wc -l )
$size
/
100*
p"

printf "${UNSET}"

rm -f ${EXEC_DIR}.out
rm -f ${CHECK_DIR}.out
