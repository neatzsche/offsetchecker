FROM ubuntu:latest

RUN mkdir /challenge

COPY binary/ challenge/

RUN apt-get update \
    && apt-get install -y  gcc-multilib  \
    && chmod +x /challenge/offsetcheck \
    && chmod +x /challenge/run.sh

EXPOSE 1337


CMD /challenge/offsetcheck