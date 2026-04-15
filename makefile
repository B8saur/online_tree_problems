TEXTFILE = lp.txt
BINARY = a.out
RESULT = result.out
is:
	g++ independent.cpp -O3 -o ${BINARY}
	./${BINARY} < input.in > ${TEXTFILE}
	cat ${TEXTFILE} | lp_solve > ${RESULT}
	grep "Value" ${RESULT}

vc:
	g++ cover.cpp -O3 -o ${BINARY}
	./${BINARY} < input.in > ${TEXTFILE}
	cat ${TEXTFILE} | lp_solve > ${RESULT}
	grep "Value" ${RESULT}

all: create
	cat ${TEXTFILE} | lp_solve

create:
	./is_presenter_lp.py > ${TEXTFILE}

clean:
	@rm ${TEXTFILE} || true
	@rm ${BINARY} || true
	@rm ${RESULT} || true
