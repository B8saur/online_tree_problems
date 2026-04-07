TEXTFILE = lp.txt
BINARY = a.out
is:
	g++ presenter.cpp -O3 -o ${BINARY}
	./${BINARY} < input.in > ${TEXTFILE}
	cat ${TEXTFILE} | lp_solve

all: create
	cat ${TEXTFILE} | lp_solve

create:
	./is_presenter_lp.py > ${TEXTFILE}

clean:
	@rm ${TEXTFILE} || true
	@rm ${BINARY} || true
