FROM i686/ubuntu:latest AS stage1
RUN apt update && apt install -y git gcc curl make zip bzip2
WORKDIR /build/logue
RUN git clone https://github.com/korginc/logue-sdk
WORKDIR /build/logue/logue-sdk
RUN git submodule update --init
WORKDIR /build/logue/logue-sdk/tools/gcc
RUN bash get_gcc_linux.sh
WORKDIR /build/logue/logue-sdk/tools/logue-cli
RUN ./get_logue_cli_linux.sh
RUN ls -lSha | cat
WORKDIR /build/logue
ARG BUILDTIME=0
ARG BUILDS=""
RUN echo $BUILDTIME
RUN echo $BUILDS
COPY . . 
RUN for x in  $(echo "${BUILDS}" | tr -s " ") ; do echo "${x}" && cd "/build/logue/${x}" && make; done

FROM scratch AS export-stage
COPY --from=stage1 /build/logue/logue-sdk/tools/logue-cli/logue-cli-*/logue-cli .
COPY --from=stage1 /build/logue/*/*unit .
#RUN echo "tremelobuild$(cat tremelo/tremelo.ntkdigunit | base64 -w 0)"
#RUN echo "pitchshiftbuild$(cat pitchshift/pitchshift.mnlgxdunit | base64 -w 0)"
#RUN echo "simplepanbuild$(cat simplepan/modfx_simplepan.ntkdigunit | base64 -w 0)"
