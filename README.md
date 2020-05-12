# logue

## Pre-requisites

Clone this repository, and then install the [Korg's logue SDK](https://github.com/korginc/logue-sdk):

```bash
git clone https://github.com/schollz/logue
cd logue 
git clone --depth 1 https://github.com/korginc/logue-sdk
# get CSMSIS libraries
cd logue-sdk && git submodule update --depth 1 --init
# install the logue-cli
cd ../tools/logue-cli
./get_logue_cli_linux.sh
chmod +x logue-cli-*/logue-cli
sudo mv logue-cli-*/logue-cli /usr/local/bin/
# get linux gcc compiler
cd logue-sdk/tools/gcc
./get_gcc_linux.sh
```

## Build effects

To build the effects, do the following (with `simplepan` as an example):

```
cd simplepan && make
```
