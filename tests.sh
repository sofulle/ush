# /bin/zsh
# tests.sh

EXEC_FILE=$1
EXEC_DIR=$2
TEST_DIR=$3

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

PRMPT="[${RED}test${UNSET}]"

count=0
for test in ${TEST_DIR}/cases/*
do
    count=$(($count+1))
done


printf "${PRMPT} Deleting old user output files\t\t"
rm -f ${TEST_DIR}/output/user/*
printf "${RED}deleted${UNSET}\n"

printf "${PRMPT} Deleting old test output files\t\t"
rm -f ${TEST_DIR}/output/test/*
printf "${RED}deleted${UNSET}\n"


id=0
printf "${PRMPT} Creating user output files\t\t"
while [ $id -lt $count ]
do
    ${EXEC_DIR}/${EXEC_FILE} < ${TEST_DIR}/cases/test${id} 2> ${TEST_DIR}/output/user/test${id} >> ${TEST_DIR}/output/user/test${id}
    id=$(($id+1))
done
printf "${GREEN}created${UNSET}\n"

id=0
printf "${PRMPT} Creating test output files\t\t"
while [ $id -lt $count ]
do
    zsh ${TEST_DIR}/cases/test${id} > ${TEST_DIR}/output/test/test${id}
    id=$(($id+1))
done
printf "${GREEN}created${UNSET}\n"


id=0
while [ $id -lt $count ]
do
    printf "${PRMPT} Testing user vs test output for ${UNDER}test${id}${UNSET} \t"

    result=$(diff ${TEST_DIR}/output/user/test${id} ${TEST_DIR}/output/test/test${id} | grep "^>" | wc -l)

    if [ $result -eq 0 ]
    then
        printf "${GREEN}OK${UNSET}\n"
    else
        printf "${RED}FAILED${UNSET}\n"
        printf "${OPACITY}"
        cat -e ${TEST_DIR}/cases/test${id}
        printf "${UNSET}"
        diff -y ${TEST_DIR}/output/user/test${id} ${TEST_DIR}/output/test/test${id}
    fi

    id=$(($id+1))
done
