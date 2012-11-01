PART=LM4F120H5QR

VARIANT=cm4f

ROOT=../../..

include ${ROOT}/makedefs

VPATH=../drivers
VPATH+=../../../utils

IPATH=..
IPATH+=../../..

all: ${COMPILER}
all: ${COMPILER}/project1.axf

clean:
	@rm -rf ${COMPILER} ${wildcard *~}

${COMPILER}:
	@mkdir -p ${COMPILER}

${COMPILER}/project1.axf: ${COMPILER}/project1.o
${COMPILER}/project1.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/project1.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a
${COMPILER}/project1.axf: project1.ld
SCATTERgcc_project1=project1.ld
ENTRY_project1=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA2

ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
