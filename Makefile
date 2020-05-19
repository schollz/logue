
.PHONY: simplepan tremelo pitchshift clipper tldy distortion

all: clean
	mkdir -p build
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="simplepan tremelo pitchshift clipper hipass tldy" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli

test:
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | head -n1 | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	echo $$INPORT

simplepan:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="simplepan" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -s 0 -u build/modfx_simplepan.ntkdigunit

tremelo:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="tremelo" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -s 1 -u build/tremelo.ntkdigunit

pitchshift:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="pitchshift" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -s 0 -u build/pitchshift.mnlgxdunit

hipass:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="hipass" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -s 0 -u build/hipass.ntkdigunit

distortion:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="distortion" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
# 	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
# 	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
# 	./build/logue-cli load -i $$INPORT -o $$OUTPORT -s 0 -u build/hipass.ntkdigunit

clipper:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="clipper" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -s 1 -u build/clipper.ntkdigunit

buildtldy:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="tldy" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli

tldy: buildtldy
	# https://github.com/korginc/logue-sdk/issues/37#issuecomment-627982850
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -s 1 -u build/tldy.ntkdigunit -d > load.log 
	tail -n1 load.log| sed 's/,//g' | sed 's/}//g' | sed 's/{//g' | sed 's/>//g' | sed 's/^ *//g' > load.sysex
	amidi -p hw:2,0,0 -S `cat load.sysex`
	rm load.log 
	rm load.sysex


upload: simplepan tremelo pitchshift hipass tldy

clean:
	rm -rf build
	rm -f load.log 
	rm -f load.sysex

reset: all
	INPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`; \
	OUTPORT=`build/logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`; \
	./build/logue-cli clear -i $$INPORT -o $$OUTPORT -a -m revfx; \
	./build/logue-cli clear -i $$INPORT -o $$OUTPORT -a -m modfx; \
	./build/logue-cli clear -i $$INPORT -o $$OUTPORT -a -m delfx; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -u build/hipass.ntkdigunit; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -u build/pitchshift.mnlgxdunit; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -u build/tremelo.ntkdigunit; \
	./build/logue-cli load -i $$INPORT -o $$OUTPORT -u build/modfx_simplepan.ntkdigunit; \
	./build/logue-cli probe -i $$INPORT -o $$OUTPORT -s 6 -m modfx; \
	./build/logue-cli probe -i $$INPORT -o $$OUTPORT -s 6 -m delfx; \
	./build/logue-cli probe -i $$INPORT -o $$OUTPORT -s 6 -m revfx
