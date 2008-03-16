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
OBJECTDIR=build/Debug/GNU-Linux-x86

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
	${OBJECTDIR}/StereoCamera.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/opt/opencv/lib -L/opt/lib -Wl,-rpath /opt/lib -lhighgui -lcv -lcvaux -lcxcore -ldb_cxx-4.5 -ldbxml -ldb -lxqilla

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/GNU-Linux-x86/applicationtest_1

dist/Debug/GNU-Linux-x86/applicationtest_1: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o dist/Debug/GNU-Linux-x86/applicationtest_1 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/VideoWriter.o: VideoWriter.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/VideoWriter.o VideoWriter.cc

${OBJECTDIR}/StereoImage.o: StereoImage.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/StereoImage.o StereoImage.cc

${OBJECTDIR}/main.o: main.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/main.o main.cc

${OBJECTDIR}/TelemetryData.o: TelemetryData.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/TelemetryData.o TelemetryData.cc

${OBJECTDIR}/VCDException.o: VCDException.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/VCDException.o VCDException.cc

${OBJECTDIR}/DataloggerRecord.o: DataloggerRecord.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/DataloggerRecord.o DataloggerRecord.cc

${OBJECTDIR}/Datalogger.o: Datalogger.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/Datalogger.o Datalogger.cc

${OBJECTDIR}/ImpactPredictor.o: ImpactPredictor.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/ImpactPredictor.o ImpactPredictor.cc

${OBJECTDIR}/RedDotExperiment.o: RedDotExperiment.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/RedDotExperiment.o RedDotExperiment.cc

${OBJECTDIR}/TelemetryCalculator.o: TelemetryCalculator.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/TelemetryCalculator.o TelemetryCalculator.cc

${OBJECTDIR}/VideoReader.o: VideoReader.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/VideoReader.o VideoReader.cc

${OBJECTDIR}/StereoCamera.o: StereoCamera.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/opt/opencv/include/opencv -I/opt/include -o ${OBJECTDIR}/StereoCamera.o StereoCamera.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/applicationtest_1

# Subprojects
.clean-subprojects:
