#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Release/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/VideoWriter.o \
	${OBJECTDIR}/StereoImage.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/TelemetryData.o \
	${OBJECTDIR}/VCDException.o \
	${OBJECTDIR}/DataloggerRecord.o \
	${OBJECTDIR}/Datalogger.o \
	${OBJECTDIR}/ImpactPredictor.o \
	${OBJECTDIR}/RedDotExperiment.o \
	${OBJECTDIR}/TelemetryCalculator.o \
	${OBJECTDIR}/VideoReader.o \
	${OBJECTDIR}/StereoCamera.o \
	${OBJECTDIR}/StereoSampleCode.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Release/GNU-Linux-x86/applicationtest_1

dist/Release/GNU-Linux-x86/applicationtest_1: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o dist/Release/GNU-Linux-x86/applicationtest_1 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/VideoWriter.o: VideoWriter.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/VideoWriter.o VideoWriter.cc

${OBJECTDIR}/StereoImage.o: StereoImage.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/StereoImage.o StereoImage.cc

${OBJECTDIR}/main.o: main.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/main.o main.cc

${OBJECTDIR}/TelemetryData.o: TelemetryData.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/TelemetryData.o TelemetryData.cc

${OBJECTDIR}/VCDException.o: VCDException.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/VCDException.o VCDException.cc

${OBJECTDIR}/DataloggerRecord.o: DataloggerRecord.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/DataloggerRecord.o DataloggerRecord.cc

${OBJECTDIR}/Datalogger.o: Datalogger.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/Datalogger.o Datalogger.cc

${OBJECTDIR}/ImpactPredictor.o: ImpactPredictor.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/ImpactPredictor.o ImpactPredictor.cc

${OBJECTDIR}/RedDotExperiment.o: RedDotExperiment.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/RedDotExperiment.o RedDotExperiment.cc

${OBJECTDIR}/TelemetryCalculator.o: TelemetryCalculator.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/TelemetryCalculator.o TelemetryCalculator.cc

${OBJECTDIR}/VideoReader.o: VideoReader.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/VideoReader.o VideoReader.cc

${OBJECTDIR}/StereoCamera.o: StereoCamera.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/StereoCamera.o StereoCamera.cc

${OBJECTDIR}/StereoSampleCode.o: StereoSampleCode.c 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -o ${OBJECTDIR}/StereoSampleCode.o StereoSampleCode.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/applicationtest_1

# Subprojects
.clean-subprojects:
